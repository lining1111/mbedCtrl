//
// Created by root on 2021/3/28.
//

#ifndef MY_RPI_DOOR_CLIENTINFO_H
#define MY_RPI_DOOR_CLIENTINFO_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include "ringBuffer/RingBuffer.h"
#include <string>

using namespace std;

namespace clientInfo {
    typedef enum {
        Start = 0, GetHead, GetHeadInfo, GetData, End
    } RecvStatus;


#define MAX_RB (1024*512)

    class ClientInfo {

    public:
        int msgid = 0;
        struct sockaddr_in clientAddr;
        int fd;
        int clientType = 0;
        unsigned char extraData[1024 * 8];//特性数据
        unsigned int dataLen;
        timeval receive_time;
        RecvStatus status = Start;
        RingBuffer *rb;
    public:
        ClientInfo(int msgid, struct sockaddr_in clientAddr, int client_fd);

        ~ClientInfo();

    };
}

#endif //MY_RPI_DOOR_CLIENTINFO_H
