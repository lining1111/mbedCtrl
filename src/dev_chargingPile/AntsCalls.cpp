//
// Created by lining on 5/31/21.
//

#include <cstring>
#include "dev_chargingPile/AntsCalls.h"

namespace antsCalls {

    uint8_t Bcc(uint8_t *data, int len) {
        uint8_t value = 0x00;
        for (int i = 0; i < len; i++) {
            value ^= *(data + i);
        }
        return value;
    }

    int Pack(CommonHead head, uint8_t *data, uint8_t dataLen, uint8_t *buf, uint8_t *len) {
        int index = 0;
        CommonTail tail;

        //1.head
        head.len = (sizeof(CommonHead) - 1) + dataLen + sizeof(CommonTail);//数据头去掉sop +数据长度+数据包尾
        //copy head to buf
        memcpy(buf + index, &head, sizeof(head));
        index += sizeof(head);
        //2.data
        if (data != nullptr && dataLen > 0) {
            memcpy(buf + index, data, dataLen);
            index += dataLen;
        }

        //3.tail
        tail.sum = Bcc(&buf[1], index);
        //copy tail to buf
        memcpy(buf + index, &tail, sizeof(tail));
        index += sizeof(tail);

        *len = index;

        return index;
    }

    int Unpack(uint8_t *buf, uint8_t len, CommonHead &head, uint8_t *data, uint8_t *dataLen, CommonTail &tail) {
        int index = 0;
        //1.head
        memcpy(&head, buf + index, sizeof(head));
        index += sizeof(head);
        //2.data dataLen;
        *dataLen = head.len - (1 + 6 + 1);//减去cmd session_id sum长度
        memcpy(data, buf + index, *dataLen);
        index += *dataLen;
        //3.tail
        memcpy(&tail, buf + index, sizeof(tail));
        index += sizeof(tail);

        return index;
    }

    int SetInfo_GetAllPortsStatus(uint8_t *buf, uint8_t *len) {
        int index = 0;

        CommonHead head;

        head.sop = 0xee;
        head.cmd = GetAllPortsStatus;

        Pack(head, nullptr, 0, buf, len);
        index += *len;

        return index;
    }

    int SetInfo_SetPortWork(uint8_t *buf, uint8_t *len, SetPorkWork_Req req) {
        int ret = 0;
        CommonHead head;
        head.sop = 0xee;
        head.cmd = SetPortWork;
        ret = Pack(head, (uint8_t *) &req, sizeof(req), buf, len);

        return ret;
    }

    int GetInfo_SetPortWork(SetPortWork_Bck &bck, uint8_t *buf, uint8_t len) {
        int ret = 0;
        CommonHead head;
        CommonTail tail;
        uint8_t data[MaxRecvData] = {0};
        uint8_t dataLen = 0;
        bzero(data, sizeof(data));
        ret = Unpack(buf, len, head, data, &dataLen, tail);

        //Bck
        memcpy(&bck, data, sizeof(bck));

        return ret;
    }

    int GetInfo_CoinInfo(CoinInfo_Bck &bck, uint8_t *buf, uint8_t len) {
        int ret = 0;
        CommonHead head;
        CommonTail tail;
        uint8_t data[MaxRecvData] = {0};
        uint8_t dataLen = 0;
        bzero(data, sizeof(data));
        ret = Unpack(buf, len, head, data, &dataLen, tail);

        //Bck
        memcpy(&bck, data, sizeof(bck));

        return ret;
    }

    int GetInfo_ChargedStatus(ChargedStatus_Bck &bck, uint8_t *buf, uint8_t len) {
        int ret = 0;
        CommonHead head;
        CommonTail tail;
        uint8_t data[MaxRecvData] = {0};
        uint8_t dataLen = 0;
        bzero(data, sizeof(data));
        ret = Unpack(buf, len, head, data, &dataLen, tail);

        //Bck
        memcpy(&bck, data, sizeof(bck));

        return ret;
    }

    int SetInfo_RecvChargedStatusSuccess(uint8_t *buf, uint8_t *len) {
        int ret = 0;
        CommonHead head;
        head.sop = 0xee;
        head.cmd = ChargedStatus;
        uint8_t result[1] = {0x01};

        ret = Pack(head, (uint8_t *) result, sizeof(result), buf, len);

        return ret;
    }

    int SetInfo_GetPortStatus(uint8_t *buf, uint8_t *len, uint8_t port) {
        int ret = 0;
        CommonHead head;
        head.sop = 0xee;
        head.cmd = GetPortStatus;

        ret = Pack(head, &port, sizeof(port), buf, len);

        return ret;
    }

    int GetInfo_GetPortStatus(GetPortStatus_Bck &bck, uint8_t *buf, uint8_t len) {
        int ret = 0;
        CommonHead head;
        CommonTail tail;
        uint8_t data[MaxRecvData] = {0};
        uint8_t dataLen = 0;
        bzero(data, sizeof(data));
        ret = Unpack(buf, len, head, data, &dataLen, tail);

        //Bck
        memcpy(&bck, data, sizeof(bck));

        return ret;
    }


}
