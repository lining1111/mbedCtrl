//
// Created by lining on 5/31/21.
//

#ifndef _ANTSCALLS_H
#define _ANTSCALLS_H

#include <cstdint>

/**
 * http://mayilaidian.com/ 蚂蚁来电 Ants Calls
 * 蚂蚁来电科技底层串口协议智能计量主板 V1.0
 * （2020.03.03）
 *  控制模块采用UART串口协议与主机进行问答式通信，9600 8N1
 *  命令格式
 *  SOP LEN CMD SESSION——ID DATA SUM
 *
 *  SOP： 单字节字头。命令为 ee，应答为 66
 *  LEN： 单字节。从 CMD 到 SUM 的字节数（含 CMD 和 SUM）
 *  CMD： 单字节，命令字节
 *  SESSION_ID： 服务端回话编号，6 个字节，用户服务端识别通讯会话。如果服务端发起的会
 *              话设备应答服务端需要带上服务端下发的会话 ID。如果设备发起会话则会话 ID
 *              为“000000“
 *  DATA： 数据字节，不定长。
 *  SUM： 单字节，LEN、CMD、SESSION_ID、DATA 的异或值
 */

namespace antsCalls {
    const uint8_t default_session_id[6] = {0, 0, 0, 0, 0, 0};
#define MaxRecvData (0xff-9)///<长度字节为1字节，最大0xff 减去包头(不包括sop)和sum(一字节)
#pragma pack(1)
    typedef struct {
        uint8_t sop = 0xee;///<命令为 ee，应答为 66
        uint8_t len;///<从 CMD 到 SUM 的字节数（含 CMD 和 SUM）
        uint8_t cmd;///<命令字节,组包的时候不用特别赋值，组包程序会自动完成赋值
        uint8_t session_id[6] = {0, 0, 0, 0, 0,
                                 0};///<用户服务端识别通讯会话。如果服务端发起的会话设备应答服务端需要带上服务端下发的会话 ID。如果设备发起会话则会话 ID为“000000“
    } CommonHead;
    typedef struct {
        uint8_t sum;///<单字节，LEN、CMD、SESSION_ID、DATA 的异或
    } CommonTail;

#pragma pack()
    //命令码
    enum Cmd {
        GetAllPortsStatus = 0x01,///<服务器读取设备每个端口当前的状态
        SetPortWork = 0x02,///<当用户付款以后，通知主设备付款的信息
        CoinInfo = 0x03,///<设备上报投币相关的信息
        ChargedStatus = 0x05,///<提交充电结束状态（如果端口是刷卡充电且要求退费的上传卡号与退款金额）
        GetPortStatus = 0x06,///<查询端口当前的充电状态
    };

#pragma pack(1)

    enum PortStatus {
        Free = 0x01,///<端口空闲
        Using = 0x02,///<端口正在使用
        Forbid = 0x03,///<端口禁用
        Hitch = 0x04,///<端口故障
    };

    //具体的命令及回复结构体
    typedef struct {
        uint8_t port;///<用户选择的充电端口号，0x02 表示2号端口
        uint16_t set;///<功率档位、电量选择,目前预留：请发送 0x0000
        uint16_t time_power;///<表示发送给机器当前端口要充电时间或者充电电量：1.如果是充电时间则以分钟为单位，如 0x0064 表示 100 分钟。2.如果是充电电量则以 0.01 度（千瓦时）为单位，如 0x0064 表示 100 个单位=1 度电
    } SetPorkWork_Req;

    typedef struct {
        uint8_t port;///<用户选择的充电端口号，0x02 表示 2 号端
        uint8_t result;///<0x01，成功 0x02，充电失败
    } SetPortWork_Bck;

    typedef struct {
        uint8_t coin_money;///<投币金额
        uint8_t port;///<端口
    } CoinInfo_Bck;

    typedef struct {
        uint8_t port;///<充电端口号，0x02 表示 2 号端口。
        uint16_t time_power;///<用户剩余的充电时间或者剩余电量：1.如果是充电时间则以分钟为单位，如 0x0064 表示 100 分钟。2.如果是充电电量则以0.01 度（千瓦时）为单位，如 0x0064 表示 100 个单位=1 度电量，以分钟 0.01 度当该值为 0xFFFF 的时候，系统会全额退款，表示该次交易不成功（一般为设备损坏，或是用户无法正常充电）
        uint8_t reason;///<停止的原因： 详见 ChargedStatus_Reason
        uint16_t card_id;///<卡号 0x010 0x02 0x03 0x04 （只有在刷卡充电的情况下发送卡号），如果此端口无刷卡不发送此 4 个字节卡号与退费金额
        uint8_t refunds_money;///<退费金额 0x01 表示 0.1 元 （只有在刷离线卡 0x0001 类型充电的情况下发送金额）
        uint16_t card_type;///<卡类型 两个字节的卡类型,只有在刷卡充电的情况下发送卡类型 详见 ChargedStatus_CardType
    } ChargedStatus_Bck;

    enum ChargedStatus_Reason {
        PaidUsed = 0x00,///<购买的充电时间、电量用完了
        UsrStop = 0x01,///<用户手动停止（拔插头，或是按了停止按钮）
        FullCharge = 0x02,///<充电满了，自动停止
        DevHitch = 0x03,///<设备或是端口出现问题，被迫停止
        PowerTooLarge = 0x04,///<因充电器功率超过充电站的单路最大输出功率，切断输出
        CardRefund = 0x05,///<刷卡退费结束
        NotConnect = 0x06,///<开始充电未接充电器
        RemoteCtrl = 0x07,///<端口远程停止
        AlarmSmoke = 0x08,///<烟雾报警停止(主板 PCB- DC_Crg_10L_C4.0 以上版本)
    };

    enum ChargedStatus_CardType {
        CardOffLine = 0x0001,///<离线卡
        CardOnLine = 0xaa33,///<在线卡
        NoCard = 0x0000,///<此端口无刷卡
    };

    typedef struct {
        uint8_t port;///<充电端口号，0x02 表示2号端口。
        uint16_t time_power;///<表示这一路充电端口的剩余充电时间或者充电电量：1.如果是充电时间则以分钟为单位，如 0x0064 表示 100 分钟。2.如果是充电电量则以 0.01 度（千瓦时）为单位，如 0x0064 表示 100个单位=1 度电量，以分钟 0.01 度，0x00 表示不在充电（包括空闲、故障）
        uint16_t instantPower;///<这一路当前充电的瞬时功率，单位为 0.1W。如 0x0001 表示0.1W
    } GetPortStatus_Bck;

#pragma pack()

    /**
     * Bcc 异或值
     * @param data in 输入数据
     * @param len in 输入数据长度
     * @return 异或值
     */
    uint8_t Bcc(uint8_t *data, int len);

    /**
     * 数据组包
     * @param head in 发送数据包头
     * @param data in 具体发送的数据
     * @param dataLen in 具体发送数据的长度
     * @param buf out 组包完成后数据
     * @param len out 组包完成后数据长度
     * @return 组包完成后的数据长度
     */
    int Pack(CommonHead head, uint8_t *data, uint8_t dataLen, uint8_t *buf, uint8_t *len);

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
    int Unpack(uint8_t *buf, uint8_t len, CommonHead &head, uint8_t *data, uint8_t *dataLen, CommonTail &tail);

    /**
     * 读取设备每个端口的状态
     * @param buf out 组包数据
     * @param len out 组包数据长度
     * @return 组包数据长度
     * @example send EE 09 01 00 00 00 00 00 00 00 08
     *          recv 66 13 01 00 00 00 00 00 00 0A 02 01 01 01 01 01 01 01 01 01 1B （1 端口正在充电，其他端口空闲 ）
     */
    int SetInfo_GetAllPortsStatus(uint8_t *buf, uint8_t *len);

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
    int SetInfo_SetPortWork(uint8_t *buf, uint8_t *len, SetPorkWork_Req req);

    /**
     * 解包 设置端口工作后的回复数据
     * @param bck out 回复数据结构体 详见 SetPortWork_Bck
     * @param buf in 接收数据
     * @param len in 接收数据的长度
     * @return 解包的数据长度
     */
    int GetInfo_SetPortWork(SetPortWork_Bck &bck, uint8_t *buf, uint8_t len);

    /**
     * 使用投币方式充电后, 设备上报投币相关的信息。在投完币选择充电端口开始充电后上传数据，只投币不选择端口不上传信息
     * @param bck out 详见 CoinInfo_Bck
     * @param buf in 接收数据
     * @param len in 接收数据长度
     * @return 解析数据的长度
     * @example recv EE 0A 03 30 30 30 30 30 30 01 01 09 （1 号端口充电 1 元）
     */
    int GetInfo_CoinInfo(CoinInfo_Bck &bck, uint8_t *buf, uint8_t len);

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
    int GetInfo_ChargedStatus(ChargedStatus_Bck &bck, uint8_t *buf, uint8_t len);

    /**
     * 获取充电完成状态的回复信息
     * @param buf out 回复信息
     * @param len out 回复信息的长度
     * @return 信息包长度
     */
    int SetInfo_RecvChargedStatusSuccess(uint8_t *buf, uint8_t *len);

    /**
     * 查询端口当前的充电状态
     * @param buf out 发送包数据
     * @param len out 发送包数据长度
     * @param port in 需要查询的端口
     * @return 发生数据包长度
     */
    int SetInfo_GetPortStatus(uint8_t *buf, uint8_t *len, uint8_t port);

    /**
     * 解包 查询端口当前的充电状态
     * @param bck out 解包数据
     * @param buf in 数据包
     * @param len in 数据包长度
     * @return 已经解析的数据包长度
     */
    int GetInfo_GetPortStatus(GetPortStatus_Bck &bck, uint8_t *buf, uint8_t len);

}


#endif //_ANTSCALLS_H
