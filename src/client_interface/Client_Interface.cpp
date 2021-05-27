//
// Created by root on 2021/3/23.
//

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include "client_interface/Client_Interface.h"

Client_Interface::Client_Interface(string server_ip, unsigned int server_port, ProcessRecv pProcessRecv,
                                   long heartBeatTimeval) {
    this->server_port = server_port;
    this->server_ip = server_ip;
    this->UserProcessRecv = pProcessRecv;
    this->heartBeatTimeval = heartBeatTimeval;
    isRun = false;
}

Client_Interface::~Client_Interface() {
    this->Close();
}

int Client_Interface::Open() {
    rb = RingBuffer_New(RecvSize);
    if (this->server_ip.empty()) {
        cout << "server ip empty" << endl;
        return -1;
    }

    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1) {
        cout << "create socket failed" << endl;
        return -1;
    } else {
        int opt = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(struct timeval));
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(struct timeval));

        int recvBufSize = 32 * 1024;
        int sendBufSize = 32 * 1024;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *) &recvBufSize, sizeof(int));
        setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *) &sendBufSize, sizeof(int));
    }

    struct sockaddr_in server_addr;
    int ret = 0;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->server_port);

    server_addr.sin_addr.s_addr = inet_addr(this->server_ip.c_str());
    ret = connect(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr));

    timeval tv_now;
    gettimeofday(&tv_now, nullptr);

    if (ret == -1) {
        cout << "connect server:" << this->server_ip << " port: " << to_string(this->server_port) << " failed"
             << ",at " << ctime((time_t *) &tv_now.tv_sec) << endl;
        close(sockfd);
        return -1;
    }

    cout << "connect server:" << this->server_ip << " port: " << to_string(this->server_port) << " success"
         << ",at " << ctime((time_t *) &tv_now.tv_sec) << endl;

    this->sockfd = sockfd;
    isRun = true;

    return 0;
}

int Client_Interface::Run() {

    if (this->isRun == false) {
        cout << "server not connect" << endl;
        return -1;
    }

    //start pthread
    thread_recv = thread(ThreadRecv, this);
    thread_recv.detach();

    thread_processRecv = thread(ThreadProcessRecv, this);
    thread_processRecv.detach();

    thread_processSend = thread(ThreadProcessSend, this);
    thread_processSend.detach();

    thread_checkStatus = thread(ThreadCheckStatus, this);
    thread_checkStatus.detach();

    return 0;
}

int Client_Interface::Close() {
    isRun = false;

    if (sockfd > 0) {
        close(sockfd);
    }
    RingBuffer_Delete(rb);

    return 0;
}

void Client_Interface::ThreadRecv(void *p) {

    if (p == nullptr) {
        return;
    }
    auto client = (Client_Interface *) p;

    char buf[1024 * 512] = {0};
    int nread = 0;

    cout << "Client_Interface " << __FUNCTION__ << " run" << endl;
    while (client->isRun) {
        usleep(1000 * 10);
        memset(buf, 0, sizeof(buf));
        nread = recv(client->sockfd, buf, sizeof(buf), 0);
        if (nread < 0) {
            if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                continue;
            }
            cout << "recv info error,errno:" << to_string(errno) << endl;
            close(client->sockfd);
            timeval tv_now;
            gettimeofday(&tv_now, nullptr);
            cout << "close sock:" << to_string(client->sockfd) << ",at " << ctime((time_t *) &tv_now.tv_sec)
                 << endl;

            client->isRun = false;
        } else if (nread > 0) {
            cout << "recv info" << endl;
            //update receive_time
            gettimeofday(&client->receive_time, nullptr);
            //把接受数据放到rb_cameraRecv
            RingBuffer_Write(client->rb, buf, nread);
        }
    }
    cout << "Client_Interface " << __FUNCTION__ << " exit" << endl;
}

void Client_Interface::ThreadProcessRecv(void *p) {
    if (p == nullptr) {
        return;
    }
    auto client = (Client_Interface *) p;

    if (client->UserProcessRecv == nullptr) {
        return;
    }

    cout << "Client_Interface " << __FUNCTION__ << " run" << endl;
    while (client->isRun) {
        client->UserProcessRecv(*client);
    }
    cout << "Client_Interface " << __FUNCTION__ << " exit" << endl;
}

void Client_Interface::ThreadProcessSend(void *p) {
    if (p == nullptr) {
        return;
    }
    auto client = (Client_Interface *) p;

    char buf_send[1024 * 512] = {0};
    int len_send = 0;

    cout << "Client_Interface " << __FUNCTION__ << " run" << endl;
    while (client->isRun) {
        //try lock_recv
        pthread_mutex_lock(&client->lock_send);
        while (client->queue_send.empty()) {
            pthread_cond_wait(&client->cond_send, &client->lock_send);
        }
        //task pop all msg
        while (!client->queue_send.empty()) {
            Msg msg = client->queue_send.front();
            client->queue_send.pop();
            bzero(buf_send, sizeof(buf_send));
            len_send = 0;
            if (client->isAddHeadLen) {
                Head head;
                head.len = msg.len;
                memcpy(buf_send + len_send, &head, sizeof(Head));
                len_send += sizeof(Head);
            }
            memcpy(buf_send + len_send, msg.buf, msg.len);
            len_send += msg.len;
            int ret = send(client->sockfd, buf_send, len_send, 0);
            if (ret != len_send) {
                if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                    continue;
                }
                cout << "msg:" << msg.buf << "send fail,errno:" << to_string((errno)) << endl;
                close(client->sockfd);
                timeval tv_now;
                gettimeofday(&tv_now, nullptr);
                cout << "close sock:" << to_string(client->sockfd) << ",at " << ctime((time_t *) &tv_now.tv_sec)
                     << endl;
                client->isRun = false;
            } else {
                cout << "msg:" << msg.buf << "send ok" << endl;
            }
        }
        pthread_mutex_unlock(&client->lock_send);
    }
    cout << "Client_Interface " << __FUNCTION__ << " exit" << endl;
}

void Client_Interface::ThreadCheckStatus(void *p) {
    if (p == nullptr) {
        return;
    }
    auto client = (Client_Interface *) p;

    cout << "Client_Interface " << __FUNCTION__ << " run" << endl;
    while (client->isRun) {
        sleep(client->checkStatus_timeval);

        timeval tv_now;

        gettimeofday(&tv_now, nullptr);
        if ((tv_now.tv_sec - client->receive_time.tv_sec) >= 2 * client->heartBeatTimeval) {
            //long time no receive
            client->isRun = false;
            cout << "long time no receive " << endl;
            cout << "now: " << ctime((time_t *) &tv_now.tv_sec) << endl;
            cout << "last receive: " << ctime((time_t *) &client->receive_time.tv_sec) << endl;
        }
    }
    cout << "Client_Interface " << __FUNCTION__ << " exit" << endl;
}

int Client_Interface::Send(Msg msg) {
    if (msg.len <= 0) {
        return -1;
    }
    //try send lock_queue_recv
    pthread_mutex_lock(&this->lock_send);
    this->queue_send.push(msg);
    pthread_cond_broadcast(&this->cond_send);
    pthread_mutex_unlock(&this->lock_send);

    return msg.len;
}

int Client_Interface::SendToBase(const char *buf, int len) {
    Client_Interface::Msg msg;
    bzero(msg.buf, sizeof(msg.buf));
    memcpy(msg.buf, buf, len);
    msg.len = len;
    return Send(msg);
}
