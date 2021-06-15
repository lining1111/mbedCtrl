//
// Created by root on 2021/3/27.
//

#include "client_interface/Client_Interface.h"

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cjson/cJSON.h>
#include <common/common_json.h>

using namespace std;
using namespace common_json;

int main(int argc, char **argv) {
    Client_Interface *client = new Client_Interface("39.103.32.81", 29081, nullptr, 5);
    client->Open();
    client->Run();
//    if (client->isRun) {
//        //发送一次心跳，作为识别信息
//        //try lock_send
//        string msg_send;
//        string ip;
//        if (argc > 1) {
//            ip = argv[1];
//        } else {
//            ip = "127.0.0.1";
//        }
//
//        Json::FastWriter fastWriter;
//        Json::Value root;
//        //root code
//        root[common(code)] = 100;
//        //root msgid
//        root[common(msgid)] = 1;
//
//        //data ip 127.0.0.1
//        root[common(data)][common(ip)] = ip;
//
//        msg_send = fastWriter.write(root);
//
//        //task
//        client->SendToBase(msg_send.c_str(), msg_send.size());
//
//    } else {
//        delete client;
//        return 0;
//    }

    string msg_send;
    bool isExit = false;
    while (!isExit) {
        cout << "please enter(q:quit):" << endl;
        cin >> msg_send;

        if (msg_send == "q") {
            isExit = true;
            continue;
        } else if (msg_send == "heartbeat") {

            uint8_t buf_send[26] = {0xEE, 0x18, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x38, 0x36, 0x35, 0x35, 0x35, 0x33, 0x30, 0x35, 0x39, 0x37, 0x34, 0x35, 0x35, 0x34,
                                    0x39, 0x00, 0xDD};
            Client_Interface::Msg msg;
            bzero(msg.buf, sizeof(msg.buf));
            memcpy(msg.buf, buf_send, sizeof(buf_send));
            msg.len = sizeof(buf_send);
            client->Send(msg);

            sleep(1);
        }

        //task
        Client_Interface::Msg msg;
        bzero(msg.buf, sizeof(msg.buf));
        memcpy(msg.buf, msg_send.c_str(), strlen(msg_send.c_str()) + 1);
        msg.len = strlen(msg_send.c_str()) + 1;
        client->Send(msg);

        sleep(1);

//        pthread_mutex_lock(&client->lock_recv);
//        while (client->queue_recv.empty()) {
//            pthread_cond_wait(&client->cond_recv, &client->lock_recv);
//        }
//        //task
//        //pop all msg process
//        while (client->queue_recv.empty()) {
//            Client_Interface::Msg msg = client->queue_recv.front();
//            client->queue_recv.pop();
//            cout << "recv:" << msg.buf << endl;
//        }
//        pthread_mutex_unlock(&client->lock_recv);

    }
    delete client;

    return 0;
}
