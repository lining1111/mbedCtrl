//
// Created by root on 2021/3/26.
//

#include "server_interface/Server_Interface.h"
#include <iostream>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

using namespace std;
using namespace clientInfo;

Server_Interface::Server_Interface(ProcessRecv pProcessRecv, long heartBeatTimeval) {
    this->port = default_port;
    this->UserProcessRecv = pProcessRecv;
    this->heartBeatTimeval = heartBeatTimeval;
    isRun = false;
}

Server_Interface::Server_Interface(unsigned int port, ProcessRecv pProcessRecv, long heartBeatTimeval) {
    this->port = port;
    this->UserProcessRecv = pProcessRecv;
    this->heartBeatTimeval = heartBeatTimeval;
    isRun = false;
}

Server_Interface::~Server_Interface() {
    this->Close();
}

int setnonblocking(int sockfd) {
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}

int Server_Interface::Open() {
    //bind
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    cout << "server sock:" << to_string(sockfd) << endl;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(this->port);
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(struct timeval));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(struct timeval));

    int recvBufSize = 32 * 1024;
    int sendBufSize = 32 * 1024;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *) &recvBufSize, sizeof(int));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *) &sendBufSize, sizeof(int));

    //将套接字绑定到服务器的网络地址上
    if (bind(sockfd, (struct sockaddr *) &server_addr, (socklen_t) sizeof(server_addr)) < 0) {
        cout << "bind error" << endl;
        return -1;
    }
    //listen
    listen(sockfd, max_listen);
    //创建一个epoll句柄

    epoll_fd = epoll_create(MAX_EVENTS);
    if (epoll_fd == -1) {
        cout << "epoll create fail" << endl;
        return -1;
    }
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sockfd;
    // 向epoll注册server_sockfd监听事件
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        cout << "epoll_ctl server fd failed" << endl;
        close(sockfd);
        return -1;
    }
    this->listen_sock = sockfd;

    return 0;
}

int Server_Interface::Run() {
    if (listen_sock <= 0) {
        return -1;
    }

    isRun = true;
    thread_acceptClient = thread(ThreadAcceptClient, this);
    thread_acceptClient.detach();

    if (UserProcessRecv) {
        thread_processRecv = thread(ThreadProcessRecv, this);
        thread_processRecv.detach();
    }

    thread_processSend = thread(ThreadProcessSend, this);
    thread_processSend.detach();

//    thread_checkStatus = thread(ThreadCheckStatus, this);
//    thread_checkStatus.detach();

    return 0;
}

int Server_Interface::Close() {
    isRun = false;
    //close client fd array
    for (int i = 0; i < vector_client.size(); i++) {
        auto iter = vector_client.at(i);
        if (iter->fd > 0) {
            close(iter->fd);
        }
        delete iter;
    }
    vector_client.clear();
    //close server fd
    if (listen_sock > 0) {
        close(listen_sock);
    }

    return 0;
}

void Server_Interface::ThreadAcceptClient(void *p) {
    if (p == nullptr) {
        return;
    }
    auto server = (Server_Interface *) p;
    int nfds = 0;// epoll监听事件发生的个数
    struct sockaddr_in remote_addr; // 客户端网络地址结构体
    socklen_t sin_size = sizeof(struct sockaddr_in);
    int client_fd;

    cout << "Server_Interface " << __FUNCTION__ << " run" << endl;
    while (server->isRun) {
        // 等待事件发生
        nfds = epoll_wait(server->epoll_fd, server->wait_events, MAX_EVENTS, -1);

        if (nfds == -1) {
//            cout << "start epoll wait fail" << endl;
            continue;
        }
        for (int i = 0; i < nfds; i++) {

            if ((server->wait_events[i].data.fd == server->listen_sock) &&
                ((server->wait_events[i].events & EPOLLIN) == EPOLLIN)) {
                // 客户端有新的连接请求
                // 等待客户端连接请求到达
                client_fd = accept(server->listen_sock, (struct sockaddr *) &remote_addr, &sin_size);
                if (client_fd < 0) {
                    cout << "accept client fail" << endl;
                    continue;
                }
                server->AddClient(client_fd, remote_addr);
            } else if ((server->wait_events[i].data.fd > 2) &&
                       ((server->wait_events[i].events & EPOLLERR) || (server->wait_events[i].events & EPOLLHUP))) {
                //有异常发生 close client
                for (int i = 0; i < server->vector_client.size(); i++) {
                    auto iter = server->vector_client.at(i);
                    if (iter->fd == server->wait_events[i].data.fd) {
                        server->CloseClient(iter->fd);
                        break;
                    }
                }
            } else {
                // 客户端有数据发送过来
                char buf[1024 * 32] = {0};
                int len = 0;
                client_fd = server->wait_events[i].data.fd;
                len = recv(client_fd, buf, sizeof(buf), 0);
                if (len < 0) {
                    cout << "recv fail" << endl;
                    // close client
                    for (int i = 0; i < server->vector_client.size(); i++) {
                        auto iter = server->vector_client.at(i);
                        if (iter->fd == server->wait_events[i].data.fd) {
                            server->CloseClient(iter->fd);
                            break;
                        }
                    }
                } else if (len > 0) {
                    //record
                    printf("receive from client %d\n", client_fd);
                    for (int i = 0; i < server->vector_client.size(); i++) {
                        auto iter = server->vector_client.at(i);
                        if (iter->fd == client_fd) {
                            RingBuffer_Write(iter->rb, buf, len);
                            break;
                        }
                    }
                }
            }
        }
    }
    close(server->epoll_fd);
    close(server->listen_sock);
    cout << "Server_Interface " << __FUNCTION__ << " exit" << endl;
}

void Server_Interface::ThreadProcessSend(void *p) {

    if (p == nullptr) {
        return;
    }
    auto server = (Server_Interface *) p;

    char buf_send[1024 * 512] = {0};
    int len_send = 0;

    cout << "Server_Interface " << __FUNCTION__ << " run" << endl;
    while (server->isRun) {
        //try lock_recv
        pthread_mutex_lock(&server->lock_send);
        while (server->queue_send.empty()) {
            pthread_cond_wait(&server->cond_send, &server->lock_send);
        }
        //task pop all msg
        while (!server->queue_send.empty()) {
            Msg msg;
            msg = server->queue_send.front();
            server->queue_send.pop();
            bzero(buf_send, sizeof(buf_send));
            len_send = 0;
            if (server->isAddHeadLen) {
                Head head;
                head.len = msg.len;
                memcpy(buf_send + len_send, &head, sizeof(Head));
                len_send += sizeof(Head);
            }
            memcpy(buf_send + len_send, msg.buf, msg.len);
            len_send += msg.len;

            //服务端发送数据，要把数据发送给正确的客户端
            int fd_send = 0;
            for (int i = 0; i < server->vector_client.size(); i++) {
                auto iter = server->vector_client.at(i);
                if (iter->fd == msg.client_fd) {
                    fd_send = iter->fd;
                    break;
                }
            }

            if (fd_send <= 0) {
                cout << "can not find client fd:" << to_string(msg.client_fd) << "drop msg:" << msg.buf << endl;
                continue;
            }

            int ret = send(fd_send, buf_send, len_send, 0);
            if (ret != len_send) {
                if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                    continue;
                }
                cout << "msg:" << msg.buf << ",send fail,errno:" << to_string((errno)) << ",client:"
                     << to_string(msg.client_fd) << endl;
                //有异常发生 close client
                for (int i = 0; i < server->vector_client.size(); i++) {
                    auto iter = server->vector_client.at(i);
                    if (iter->fd == fd_send) {
                        server->CloseClient(iter->fd);
                        break;
                    }
                }
            } else {
                cout << "msg:" << msg.buf << ",send ok" << ",client:" << to_string(msg.client_fd) << endl;
            }
        }
        pthread_mutex_unlock(&server->lock_send);
    }
    cout << "Server_Interface " << __FUNCTION__ << " exit" << endl;
}

void Server_Interface::ThreadCheckStatus(void *p) {
    if (p == nullptr) {
        return;
    }
    auto server = (Server_Interface *) p;

    cout << "Server_Interface " << __FUNCTION__ << " run" << endl;
    while (server->isRun) {

        sleep(server->checkStatus_timeval);

        if (server->heartBeatTimeval <= 0) {
            //设置为0的话，则不进行检查
            continue;
        }

        timeval tv_now;

        gettimeofday(&tv_now, nullptr);

        for (int i = 0; i < server->vector_client.size(); i++) {
            auto iter = server->vector_client.at(i);
            if ((tv_now.tv_sec - iter->receive_time.tv_sec) >= 2 * server->heartBeatTimeval) {
                //long time no receive
                server->CloseClient(iter->fd);
                cout << "long time no receive " << endl;
                cout << "now: " << ctime((time_t *) &tv_now.tv_sec) << endl;
                cout << "last receive: " << ctime((time_t *) &iter->receive_time.tv_sec) << endl;
            }
        }
    }
    cout << "Server_Interface " << __FUNCTION__ << " exit" << endl;
}


int Server_Interface::Send(Server_Interface::Msg msg) {
    if (msg.len <= 0 || msg.client_fd <= 0) {
        return -1;
    }
    //try lock_send
    pthread_mutex_lock(&this->lock_send);
    queue_send.push(msg);
    pthread_cond_broadcast(&this->cond_send);
    pthread_mutex_unlock(&this->lock_send);

    return msg.len;
}

int Server_Interface::SendtoBase(int client_fd, string json_send) {
    Server_Interface::Msg msg_send;
    bzero(msg_send.buf, sizeof(msg_send.buf));
    memcpy(msg_send.buf, json_send.c_str(), json_send.size());
    msg_send.len = json_send.size();

    //set client fd
    msg_send.client_fd = client_fd;
    return Send(msg_send);
}


int Server_Interface::AddClient(int client_sock, struct sockaddr_in remote_addr) {

    setnonblocking(client_sock);
    // 向epoll注册client_sockfd监听事件
    this->ev.events = EPOLLIN | EPOLLET | EPOLLERR | EPOLLHUP;
    this->ev.data.fd = client_sock;
    if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, client_sock, &this->ev) == -1) {
        cout << "epoll_ctl:" << to_string(client_sock) << " register failed" << endl;
        return -1;
    }

    cout << "accept client:" + to_string(client_sock) + ",ip:" << inet_ntoa(remote_addr.sin_addr) << endl;
    //clientInfo
    ClientInfo *clientInfo = new ClientInfo(0, remote_addr, client_sock);

    //add vector
    pthread_mutex_lock(&this->lock_vector_client);
    this->vector_client.push_back(clientInfo);
    pthread_cond_broadcast(&this->cond_vector_client);
    pthread_mutex_unlock(&this->lock_vector_client);

    timeval tv_now;
    gettimeofday(&tv_now, nullptr);
    cout << "add client:" << to_string(client_sock) << ",at " << ctime((time_t *) &tv_now.tv_sec) << endl;

    return 0;
}

int Server_Interface::CloseClient(int client_sock) {

    if (epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, client_sock, &this->ev) == -1) {
        cout << "epoll_ctl:" << to_string(client_sock) << " del failed" << endl;
    }
    pthread_mutex_lock(&this->lock_vector_client);
    //vector erase
    for (int i = 0; i < vector_client.size(); i++) {
        auto iter = vector_client.at(i);
        if (iter->fd == client_sock) {
            //close sock
            if (iter->fd > 0) {
                close(iter->fd);
            }

            //delete client class release clientInfo->rb
            delete iter;

            //erase vector
            vector_client.erase(vector_client.begin() + i);
            vector_client.shrink_to_fit();

            timeval tv_now;
            gettimeofday(&tv_now, nullptr);
            cout << "close client:" << to_string(client_sock) << ",at " << ctime((time_t *) &tv_now.tv_sec)
                 << endl;
            break;
        }
    }
    pthread_cond_broadcast(&this->cond_vector_client);
    pthread_mutex_unlock(&this->lock_vector_client);

    return 0;
}

void Server_Interface::ThreadProcessRecv(void *p) {
    if (p == nullptr) {
        return;
    }
    auto server = (Server_Interface *) p;
    if (server->UserProcessRecv == nullptr) {
        return;
    }

    cout << "Server_Interface " << __FUNCTION__ << " run" << endl;
    while (server->isRun) {
        server->UserProcessRecv(*server);
    }
    cout << "Server_Interface " << __FUNCTION__ << " exit" << endl;
}


