//
// Created by root on 2021/3/27.
//

#ifndef MY_RPI_DOOR_SERVER_BUSINESS_H
#define MY_RPI_DOOR_SERVER_BUSINESS_H

#include "server_interface/Server_Interface.h"

#include <cjson/cJSON.h>
#include <thread>

using namespace std;

class Server_Business {
public:
    Server_Interface *server;

    bool isInit = false;
    bool isRun = false;
    thread thread_processRecv;
    thread thread_heartbeat;

    Server_Interface::Callback callback;
    void *pUser;

    Server_Interface::HeartBeat heartBeat;
    long heartBeatTimeval = 5/*30*/;//应该和server端保持一致

public:
    Server_Business(Server_Interface::ProcessRecv pProcessRecv, Server_Interface::Callback pCallBack,
                    Server_Interface::HeartBeat pHeartBeat, long heartBeatTimeval, void *pUser);

    Server_Business(unsigned int port, Server_Interface::ProcessRecv pProcessRecv, Server_Interface::Callback pCallBack,
                    Server_Interface::HeartBeat pHeartBeat, long heartBeatTimeval, void *pUser);

    ~Server_Business();

    int Open();

    int Run();

    int Close();

private:
    static void ThreadProcessRecv(void *p);

    static void ThreadHeartBeat(void *p);

};


#endif //MY_RPI_DOOR_SERVER_BUSINESS_H
