//
// Created by root on 2021/3/27.
//

#include "server_interface/Server_Interface.h"

#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    Server_Interface *server = new Server_Interface(8131, nullptr, 0);
    server->Open();
    server->Run();
    string msg_send;
    bool isExit = false;
    while (!isExit) {

        pthread_mutex_lock(&server->lock_recv);
        while (server->queue_recv.empty()) {
            pthread_cond_wait(&server->cond_recv, &server->lock_recv);
        }
        //task
        //pop all msg process
        while (!server->queue_recv.empty()) {
            Server_Interface::Msg msg = server->queue_recv.front();
            server->queue_recv.pop();
            cout << "recv:" << msg.buf << endl;

            //send recv
            server->Send(msg);
        }
        pthread_mutex_unlock(&server->lock_recv);
    }
    delete server;

    return 0;
}
