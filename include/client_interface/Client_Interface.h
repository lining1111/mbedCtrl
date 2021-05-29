//
// Created by lining on 2021/3/23.
//

#ifndef _CLIENT_INTERFACE_H
#define _CLIENT_INTERFACE_H

#include <string>
#include <queue>
#include <thread>

#include "ringBuffer/RingBuffer.h"

using namespace std;

//new Connect Run delete
class Client_Interface {
#pragma pack(1)
    struct Head {
        uint32_t len;
    };
#pragma pack()

public:
    string server_ip;
    unsigned int server_port;
    int sockfd = 0;
    RingBuffer *rb = nullptr;
#define RecvSize (1024*512)
    bool isRun = false;//运行标志
    timeval receive_time = {0, 0};

public:
    thread thread_recv;
    thread thread_processRecv;
    thread thread_processSend;
    thread thread_checkStatus;
    long checkStatus_timeval = 5;
    long heartBeatTimeval = 5/*30*/;//应该和server端保持一致

    typedef enum {
        Start = 0, GetHead, GetHeadInfo, GetData, End
    } RecvStatus;

    struct Msg {
        char buf[1024 * 512];
        int len = 0;
    };
    RecvStatus status = Start;
    bool isAddHeadLen = true;//json通信，是否添加长度头部
    queue<Msg> queue_recv;
    pthread_mutex_t lock_recv = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_recv = PTHREAD_COND_INITIALIZER;

    queue<Msg> queue_send;
    pthread_mutex_t lock_send = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_send = PTHREAD_COND_INITIALIZER;

public:
    //usrProcessRecv
    typedef void (*ProcessRecv)(Client_Interface &client);

    ProcessRecv UserProcessRecv;

    Client_Interface(string server_ip, unsigned int server_port, ProcessRecv pProcessRecv, long heartBeatTimeval);

    ~Client_Interface();


public:
    int Open();

    int Run();

    int Close();

private:
    static void ThreadRecv(void *p);

    static void ThreadProcessRecv(void *p);

    static void ThreadProcessSend(void *p);

    static void ThreadCheckStatus(void *p);

public:
    //send to server
    int Send(Msg msg);

    int SendToBase(const char *buf, int len);

};


#endif //_CLIENT_INTERFACE_H
