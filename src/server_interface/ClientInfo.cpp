//
// Created by root on 2021/3/28.
//

#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#include "server_interface/ClientInfo.h"
namespace clientInfo {

    ClientInfo::ClientInfo(int msgid, struct sockaddr_in clientAddr, int client_fd) {
        this->msgid = msgid;
        this->clientAddr = clientAddr;
        this->fd = client_fd;
        this->clientType = 0;
        bzero(this->extraData, sizeof(this->extraData));
        gettimeofday(&this->receive_time, nullptr);
        this->rb = RingBuffer_New(MAX_RB);
        this->dataLen = 0;
        this->status = Start;
    }

    ClientInfo::~ClientInfo() {
        RingBuffer_Delete(this->rb);
        bzero(this->extraData, sizeof(this->extraData));
    }

}