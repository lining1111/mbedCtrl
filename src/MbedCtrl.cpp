//
// Created by lining on 5/28/21.
//

#include <unistd.h>
#include "MbedCtrl.h"

MbedCtrl::MbedCtrl() {

}

MbedCtrl::~MbedCtrl() {

}

int MbedCtrl::Open() {
    return 0;
}

int MbedCtrl::Run() {
    isRun = true;

    thread_processCloudRecv = thread(ThreadProcessCloud, this);
    thread_processCloudRecv.detach();

    thread_processChargingPileRecv = thread(ThreadProcessChargingPile, this);
    thread_processChargingPileRecv.detach();

    thread_keep = thread(ThreadKeep, this);
    thread_keep.detach();


    return 0;
}

int MbedCtrl::Close() {
    return 0;
}

void MbedCtrl::ThreadProcessChargingPile(void *p) {
    auto mbedCtrl = (MbedCtrl *) p;
    ChargingPile *chargingPile = mbedCtrl->chargingPile;
    Rabbitmq_Interface *cloud = mbedCtrl->cloud;
    cout << "MbedCtrl:" << __FUNCTION__ << " run" << endl;
    while (!mbedCtrl->isRun) {
        //获取充电桩接收的信息
        usleep(1000 * 10);
        if (chargingPile->server->queue_recv.empty()) {
            continue;
        }
        pthread_mutex_lock(&chargingPile->server->lock_recv);
        while (chargingPile->server->queue_recv.empty()) {
            pthread_cond_wait(&chargingPile->server->cond_recv, &chargingPile->server->lock_recv);
        }

        while (!chargingPile->server->queue_recv.empty()) {
            Server_Interface::Msg msg;
            msg = chargingPile->server->queue_send.front();
            chargingPile->server->queue_send.pop();

            cout << "chargingPile recv:" << msg.buf << endl;

            //TODO
            //1.判断消息类型，如果是上报的设备标识信息，则记录到本地，不上传上游
            //2.如果消息是设备操作的回送消息，则上报上游。

        }

        pthread_mutex_unlock(&chargingPile->server->lock_vector_client);

    }
    cout << "MbedCtrl:" << __FUNCTION__ << " exit" << endl;
}

void MbedCtrl::ThreadProcessCloud(void *p) {
    auto mbedCtrl = (MbedCtrl *) p;
    ChargingPile *chargingPile = mbedCtrl->chargingPile;
    Rabbitmq_Interface *cloud = mbedCtrl->cloud;
    cout << "MbedCtrl:" << __FUNCTION__ << " run" << endl;
    while (!mbedCtrl->isRun) {
        //获取上游的信息
        usleep(1000 * 10);
        if (cloud->queue_consumer.empty()) {
            continue;
        }
        pthread_mutex_lock(&cloud->lock_consumer);
        while (cloud->queue_consumer.empty()) {
            pthread_cond_wait(&cloud->cond_consumer, &cloud->lock_consumer);
        }
        while (!cloud->queue_consumer.empty()) {
            Rabbitmq_Interface::Msg msg;
            msg = cloud->queue_consumer.front();
            cloud->queue_consumer.pop();
            cout << "cloud recv:" << msg.buf << endl;
            //TODO
        }

        pthread_mutex_unlock(&cloud->lock_consumer);
    }
    cout << "MbedCtrl:" << __FUNCTION__ << " exit" << endl;
}

void MbedCtrl::ThreadKeep(void *p) {
    auto mbedCtrl = (MbedCtrl *) p;
    ChargingPile *chargingPile = mbedCtrl->chargingPile;
    Rabbitmq_Interface *cloud = mbedCtrl->cloud;
    cout << "MbedCtrl:" << __FUNCTION__ << " run" << endl;
    while (!mbedCtrl->isRun) {
        usleep(1000 * 10);
        if (!chargingPile->server->isRun){
            //重启server
        }

        if (!cloud->isRun){
            cloud->Close();
            if (cloud->Open()==0){
                cloud->Run();
            }
        }
    }
    cout << "MbedCtrl:" << __FUNCTION__ << " exit" << endl;
}
