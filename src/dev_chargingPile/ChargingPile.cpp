//
// Created by lining on 5/28/21.
//

#include <cstring>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include "dev_chargingPile/ChargingPile.h"
#include "log/log.h"

using namespace std;

bool ChargingPile::Get_Len(RingBuffer *rb, unsigned int *dataLen) {
    uint8_t buf[1024 * 512];
    int readLen = 0;
    int len = 0;
    bool ret = false;

    memset(buf, 0, sizeof(buf));
    readLen = 4;//数据长度占4字节
    if (rb->available_for_read >= readLen) {
        RingBuffer_Read(rb, buf, readLen);

        memcpy(&len, &buf[readLen - 4], sizeof(int));

        *dataLen = len;
        if (len > 8192) {
            //默认最大数据长度不超过8192
            ret = false;
            return ret;
        }
        ret = true;
    }
    return ret;
}

bool ChargingPile::Get_Data(RingBuffer *rb, uint8_t *out, unsigned int len) {
    bool ret = false;

    if (rb->available_for_read >= len) {
        RingBuffer_Read(rb, out, len);
        ret = true;
    }
    return ret;
}

void ChargingPile::ProcessSubServerRecv(Server_Interface &server) {
    char dataBuf[1024 * 512];
    memset(dataBuf, 0, sizeof(dataBuf));

    //服务端要一直循环处理所有客户端的信息
    cout << "ChargingPile:" << __FUNCTION__ << " run" << endl;
    while (server.isRun) {
        usleep(1000 * 10);
        if (server.vector_client.empty()) {
            continue;
        }

        pthread_mutex_lock(&server.lock_vector_client);
        while (server.vector_client.empty()) {
            pthread_cond_wait(&server.cond_vector_client, &server.lock_vector_client);
        }

        //遍历所有连接的client，如果有信息，处理
        for (int i = 0; i < server.vector_client.size(); i++) {
            auto iter = server.vector_client.at(i);
            switch (iter->status) {
                case Start:
                    if (server.isAddHeadLen) {
                        iter->status = GetHead;
                    } else {
                        iter->status = GetHeadInfo;
                    }
                    break;
                case GetHead:
                    if (Get_Len(iter->rb, &iter->dataLen)) {
                        server.vector_client.at(i)->status = GetHeadInfo;
                    }
                    break;
                case GetHeadInfo:
                    if (Get_Data(iter->rb, (uint8_t *) dataBuf, iter->dataLen)) {
                        iter->status = GetData;
                    }
                    break;
                case GetData: {
                    //到这里就获取了真正的信息
                    //1.更新接收时间
                    gettimeofday(&iter->receive_time, nullptr);
                    iter->msgid++;
                    if (iter->msgid > MAX_MSGID) {
                        iter->msgid = 0;
                    }

                    //2.将获得的信息压入接收queue
                    Server_Interface::Msg msg;
                    //buf
                    memset(msg.buf, 0, sizeof(msg.buf));
                    memcpy(msg.buf, dataBuf, server.vector_client.at(i)->dataLen);
                    //buf len
                    msg.len = server.vector_client.at(i)->dataLen;
                    //client fd
                    msg.client_fd = server.vector_client.at(i)->fd;

                    //try lock_recv
                    pthread_mutex_lock(&server.lock_recv);
                    server.queue_recv.push(msg);
                    pthread_cond_broadcast(&server.cond_recv);
                    pthread_mutex_unlock(&server.lock_recv);

                    iter->status = End;
                }
                    break;
                case End:
                    iter->status = Start;
                    break;
            }
        }
        pthread_mutex_unlock(&server.lock_vector_client);
    }
    cout << "ChargingPile:" << __FUNCTION__ << " exit" << endl;
}

int ChargingPile::SendToDev(string extraData, uint8_t *buf, uint32_t len) {

    int ret = -1;
    //从已接入的客户端数组中，获取与标志信息
    if (server->vector_client.empty()) {
        cout << "no client accept" << endl;
        return ret;
    }

    ClientInfo *client = nullptr;
    for (int i = 0; i < server->vector_client.size(); i++) {
        auto iter = server->vector_client.at(i);
        CPMark *cpMark = (CPMark *) iter->extraData;
        if (cpMark->dev_sn == extraData) {
            client = iter;
        }
    }
    if (client == nullptr) {
        LogError("dev:%s not exist,drop msg:%s", extraData.c_str(), buf);
        ret = -1;
    } else {
        //设备找到了，组织发送
        Server_Interface::Msg msg;
        msg.client_fd = client->fd;
        bzero(msg.buf, sizeof(msg.buf));
        memcpy(msg.buf, buf, len);
        msg.len = len;
        ret = server->Send(msg);
    }

    return ret;
}
