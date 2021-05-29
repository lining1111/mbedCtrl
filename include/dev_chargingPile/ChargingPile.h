//
// Created by lining on 5/28/21.
//

#ifndef _CHARGINGPILE_H
#define _CHARGINGPILE_H

#include <server_interface/Server_Interface.h>

class ChargingPile {
#define MAX_MSGID 100000
public:
    uint16_t serverPort = 8131;//服务器端口 默认8131
    typedef struct{
        string dev_sn;
    }CPMark;
public:
    Server_Interface *server;//充电桩以客户端接入
public:
    ChargingPile();
    ~ChargingPile();

    static bool Get_Len(RingBuffer *rb, unsigned int *dataLen);

    static bool Get_Data(RingBuffer *rb, uint8_t *out, unsigned int len);

    //接收rb处理函数，假定消息的格式为 信息长度(4bytes)+消息
    static void ProcessSubServerRecv(Server_Interface &server);

    //发送函数
    int SendToDev(string extraData, uint8_t *buf, uint32_t len);

};


#endif //_CHARGINGPILE_H
