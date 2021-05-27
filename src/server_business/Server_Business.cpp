//
// Created by root on 2021/3/27.
//

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "server_business/Server_Business.h"

using namespace std;

Server_Business::Server_Business(Server_Interface::ProcessRecv pProcessRecv, Server_Interface::Callback pCallBack,
                                 Server_Interface::HeartBeat pHeartBeat, long heartBeatTimeval, void *pUser) {
    this->server = new Server_Interface(pProcessRecv, heartBeatTimeval);
    this->callback = pCallBack;
    this->pUser = pUser;
    if (pHeartBeat == nullptr) {
        cout << "error HeartBeat must not be null" << endl;
        return;
    }
    this->heartBeat = pHeartBeat;
    this->heartBeatTimeval = heartBeatTimeval;

    isRun = false;
}

Server_Business::Server_Business(unsigned int port, Server_Interface::ProcessRecv pProcessRecv,
                                 Server_Interface::Callback pCallBack,
                                 Server_Interface::HeartBeat pHeartBeat, long heartBeatTimeval, void *pUser) {
    this->server = new Server_Interface(port, pProcessRecv, heartBeatTimeval);
    this->callback = pCallBack;
    this->pUser = pUser;
    if (pHeartBeat == nullptr) {
        cout << "error HeartBeat must not be null" << endl;
        return;
    }
    this->heartBeat = pHeartBeat;
    this->heartBeatTimeval = heartBeatTimeval;

    isRun = false;
}

Server_Business::~Server_Business() {
    isRun = false;
    isInit = false;
    delete server;
}

int Server_Business::Open() {
    if (server == nullptr) {
        return -1;
    }
    server->Open();
    server->Run();

    return 0;
}

int Server_Business::Run() {

    isRun = true;
    thread_processRecv = thread(ThreadProcessRecv, this);
    thread_processRecv.detach();

    thread_heartbeat = thread(ThreadHeartBeat, this);
    thread_heartbeat.detach();

    return 0;
}

int Server_Business::Close() {
    isRun = false;
    if (server) {
        server->Close();
    }

    return 0;
}

void Server_Business::ThreadProcessRecv(void *p) {
    if (p == nullptr) {
        return;
    }
    auto business = (Server_Business *) p;

    cout << "Server_Business " << __FUNCTION__ << " run" << endl;
    while (business->isRun) {
        usleep(1000 * 10);
        if (!business->server->isRun) {
            continue;
        }

        //获取接口数据
        pthread_mutex_lock(&business->server->lock_recv);
        while (business->server->queue_recv.empty()) {
            pthread_cond_wait(&business->server->cond_recv, &business->server->lock_recv);
        }
        //task
        //pop all msg process
        while (!business->server->queue_recv.empty()) {
            Server_Interface::Msg msg = business->server->queue_recv.front();
            business->server->queue_recv.pop();

            //record receive time
            for (int i = 0; i < business->server->vector_client.size(); i++) {
                if (business->server->vector_client.at(i)->fd == msg.client_fd) {
                    gettimeofday(&business->server->vector_client.at(i)->receive_time, nullptr);
                }
            }

            //这是一个单纯打印接收的流程，可以根据实际(比如json中code，来进行区分)
            cout << "recv client info,len:" << msg.len << ",json:" << msg.buf << endl;
            //回环 heartbeat opened loop must close
            bool enableLoop = false;
            if (enableLoop) {
                business->server->Send(msg);
            }

            //获取数据，依次处理
            if (business->callback) {
                business->callback(*business->server, msg, business->pUser);
            }

        }
        pthread_mutex_unlock(&business->server->lock_recv);
    }
    cout << "Server_Business " << __FUNCTION__ << " exit" << endl;
}

void Server_Business::ThreadHeartBeat(void *p) {
    if (p == nullptr) {
        return;
    }
    auto business = (Server_Business *) p;

    cout << "Server_Business " << __FUNCTION__ << " run" << endl;
    while (business->isRun) {
        sleep(business->heartBeatTimeval);
        if (!business->server->isRun) {
            continue;
        }

        pthread_mutex_lock(&business->server->lock_vector_client);
        while (business->server->vector_client.empty()) {
            pthread_cond_wait(&business->server->cond_vector_client, &business->server->lock_vector_client);
        }

        //loop the client send heartbeat
        for (int i = 0; i < business->server->vector_client.size(); i++) {
            auto iter = business->server->vector_client.at(i);
            //send heartbeat
            business->heartBeat(*business->server, *iter);
        }

        pthread_mutex_unlock(&business->server->lock_vector_client);

    }
    cout << "Server_Business " << __FUNCTION__ << " exit" << endl;
}
