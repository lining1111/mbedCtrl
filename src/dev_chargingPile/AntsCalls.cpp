//
// Created by lining on 5/31/21.
//

#include <cstring>
#include "dev_chargingPile/AntsCalls.h"

namespace antsCalls {


    /**
     * Bcc 异或值
     * @param data in 输入数据
     * @param len in 输入数据长度
     * @return 异或值
     */
    uint8_t Bcc(uint8_t *data, int len) {
        uint8_t value = 0x00;
        for (int i = 0; i < len; i++) {
            value ^= *(data + i);
        }
        return value;
    }

    /**
     * 数据组包
     * @param head in 发送数据包头
     * @param data in 具体发送的数据
     * @param dataLen in 具体发送数据的长度
     * @param buf out 组包完成后数据
     * @param len out 组包完成后数据长度
     * @return 组包完成后的数据长度
     */
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

    /**
     * 数据解包
     * @param buf in 接收数据
     * @param len in 接收数据长度
     * @param head 接收数据头部，包括命令码等，详见 CommonHead
     * @param data out 实际数据
     * @param dataLen out 实际数据长度
     * @param tail out 接收数据尾部， 包括bcc值，详见 CommonTail
     * @return 解析接收的数据长度，一个完整包的长度
     */
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

    /**
     * 读取设备每个端口的状态
     * @param buf out 组包数据
     * @param len out 组包数据长度
     * @return 组包数据长度
     * @example send EE 09 01 00 00 00 00 00 00 00 08
     *          recv 66 13 01 00 00 00 00 00 00 0A 02 01 01 01 01 01 01 01 01 01 1B （1 端口正在充电，其他端口空闲 ）
     */
    int SetInfo_GetAllPortsStatus(uint8_t *buf, uint8_t *len) {
        int index = 0;

        CommonHead head;

        head.sop = 0xee;
        head.cmd = GetAllPortsStatus;

        Pack(head, nullptr, 0, buf, len);
        index += *len;

        return index;
    }

    /**
     * 组包 当用户付款以后，通知主设备付款的信息
     * @param buf out 组包的数据
     * @param len out 组包的数据长度
     * @param req in 端口设置具体信息 详见 SetPorkWork_Req
     * @return 组包的数据长度
     * @example send EE 0D 02 00 00 00 00 00 00 01 00 00 00 64 6A （选择第一端口，充电时间 100 分钟
     *               EE 0D 02 00 00 00 00 00 00 01 00 00 00 0a 04 第一路10分钟
     *               EE 0D 02 00 00 00 00 00 00 02 00 00 00 0a 07 第二路10分钟
     *          recv 66 0A 02 00 00 00 00 00 00 01 01 08 (开始充电)
     */
    int SetInfo_SetPortWork(uint8_t *buf, uint8_t *len, SetPorkWork_Req req) {
        int ret = 0;
        CommonHead head;
        head.sop = 0xee;
        head.cmd = SetPortWork;
        ret = Pack(head, (uint8_t *) &req, sizeof(req), buf, len);

        return ret;
    }

    /**
     * 解包 设置端口工作后的回复数据
     * @param bck out 回复数据结构体 详见 SetPortWork_Bck
     * @param buf in 接收数据
     * @param len in 接收数据的长度
     * @return 解包的数据长度
     */
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

    /**
     * 使用投币方式充电后, 设备上报投币相关的信息。在投完币选择充电端口开始充电后上传数据，只投币不选择端口不上传信息
     * @param bck out 详见 CoinInfo_Bck
     * @param buf in 接收数据
     * @param len in 接收数据长度
     * @return 解析数据的长度
     * @example recv EE 0A 03 30 30 30 30 30 30 01 01 09 （1 号端口充电 1 元）
     */
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

    /**
     * 提交充电结束状态（如果端口是刷卡充电且要求退费的上传卡号与退款金额）
     * 当某个端口的电动车已经充电结束（不管是已经充满自动停止，或是用户手动拔掉电源），
     * 调用该接口通知服务器。
     * 注意：调用该接口以后，系统会在后台根据用户的充电实际情况（比如本来可以充电 100
     * 分钟，用户只用了 50 分钟）做相应的退款（管理员可以设置到底是退款还是不退款）。
     * 调用了该接口，该次交易即已经结束，用户再插上线路，肯定是不能再充电的
     * @param bck out 获取的回复数据
     * @param buf in 需要解析的包
     * @param len in 需要解析的包长度
     * @return 已经解析的包长度
     */
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

    /**
     * 获取充电完成状态的回复信息
     * @param buf out 回复信息
     * @param len out 回复信息的长度
     * @return 信息包长度
     */
    int SetInfo_RecvChargedStatusSuccess(uint8_t *buf, uint8_t *len) {
        int ret = 0;
        CommonHead head;
        head.sop = 0xee;
        head.cmd = ChargedStatus;
        uint8_t result[1] = {0x01};

        ret = Pack(head, (uint8_t *) result, sizeof(result), buf, len);

        return ret;
    }

    /**
     * 查询端口当前的充电状态
     * @param buf out 发送包数据
     * @param len out 发送包数据长度
     * @param port in 需要查询的端口
     * @return 发生数据包长度
     */
    int SetInfo_GetPortStatus(uint8_t *buf, uint8_t *len, uint8_t port) {
        int ret = 0;
        CommonHead head;
        head.sop = 0xee;
        head.cmd = GetPortStatus;

        ret = Pack(head, &port, sizeof(port), buf, len);

        return ret;
    }

    /**
     * 解包 查询端口当前的充电状态
     * @param bck out 解包数据
     * @param buf in 数据包
     * @param len in 数据包长度
     * @return 已经解析的数据包长度
     */
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
