//
// Created by lining on 2021/3/26.
//

#ifndef _SERVER_INTERFACE_H
#define _SERVER_INTERFACE_H

#include <vector>
#include <sys/epoll.h>
#include <thread>


#include "ringBuffer/RingBuffer.h"
#include "server_interface/ClientInfo.h"
#include <queue>
#include <netdb.h>

using namespace std;
using namespace clientInfo;

//new Open Run delete
class Server_Interface {
public:
    const unsigned int default_port = 8313;
    int max_listen = 5;

    const long checkStatus_timeval = 5;
    long heartBeatTimeval = 5/*30*/;//应该和server端保持一致
#pragma pack(1)
    struct Head {
        uint32_t len;
    };
#pragma pack()
public:
    unsigned int port;
    int listen_sock = 0;
    vector<ClientInfo *> vector_client;
    pthread_mutex_t lock_vector_client = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_vector_client = PTHREAD_COND_INITIALIZER;

    //epoll
    int epoll_fd;
    struct epoll_event ev;
#define MAX_EVENTS 10
    struct epoll_event wait_events[MAX_EVENTS];
    bool isRun = false;//运行标志

    thread thread_acceptClient;
    thread thread_processRecv;
    thread thread_processSend;
    thread thread_checkStatus;

    struct Msg {
        char buf[1024 * 512];
        int len = 0;
        int client_fd;
    };
    bool isAddHeadLen = true;//json通信，是否添加长度头部

    queue<Msg> queue_recv;
    pthread_mutex_t lock_recv = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_recv = PTHREAD_COND_INITIALIZER;

    queue<Msg> queue_send;
    pthread_mutex_t lock_send = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_send = PTHREAD_COND_INITIALIZER;

public:

    //processRecv
    typedef void (*ProcessRecv)(Server_Interface &server);

    ProcessRecv UserProcessRecv;

public:
    Server_Interface(ProcessRecv pProcessRecv, long heartBeatTimeval);

    Server_Interface(unsigned int port, ProcessRecv pProcessRecv, long heartBeatTimeval);

    ~Server_Interface();

public:
    int Open();

    int Run();

    int Close();

private:
    static void ThreadAcceptClient(void *p);

    static void ThreadProcessRecv(void *p);

    static void ThreadProcessSend(void *p);

    static void ThreadCheckStatus(void *p);

    int AddClient(int client_sock, struct sockaddr_in remote_addr);

    int CloseClient(int client_sock);

public:
    //send to client
    int Send(Msg msg);

    int SendtoBase(int client_fd, string json_send);
};


#endif //_SERVER_INTERFACE_H
