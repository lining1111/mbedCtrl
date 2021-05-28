//
// Created by lining on 5/28/21.
//

#ifndef MBEDCTRL_MBEDCTRL_H
#define MBEDCTRL_MBEDCTRL_H

#include <dev_chargingPile/ChargingPile.h>
#include <rabbitmq_interface/Rabbitmq_Interface.h>
#include <common/common_json.h>
#include <common/common_database.h>
#include <config/Config.h>

#include <string>

using namespace std;


class MbedCtrl {

public:
#define ConfigKey(str) configKey_##str
    const string configKey_cloud_hostname = "cloud_hostname";
    const string configKey_cloud_hostport = "cloud_hostport";
    const string configKey_cloud_usrname = "cloud_usrname";
    const string configKey_cloud_usrpasswd = "cloud_usrpasswd";
    const string configKey_cloud_vhost = "cloud_vhost";
    const string configKey_cloud_channel = "cloud_channel";
    const string configKey_cloud_community = "cloud_community";

    const string configKey_chargingPile_serverPort = "chargingPile_serverPort";
    const string configKey_chargingPile_heartBeat = "chargingPile_heartBeat";

    string configFilePath;

public:
    Rabbitmq_Interface::RabbitmqConfig cloudConfig;
    string community_id;
    Rabbitmq_Interface *cloud = nullptr;//上游控制接口
    uint16_t chargingPileServerPort = 8183;
    long chargingPileHeartBeatTv = 30;//充电桩设备上报信息最大间隔
    Server_Interface *chargingPile = nullptr;//充电桩接口
public:
    bool isRun = false;
    thread thread_processChargingPileRecv;//充电桩业务处理线程
    thread thread_processCloudRecv;//上游业务处理线程
    thread thread_keep;//重连线程

public:
    MbedCtrl(string configFilePath);

    ~MbedCtrl();

    int Open();

    int Run();

    int Close();

public:
    void SetConfig(string filePath);

    void GetConfig(string filePath);

private:
    static void ThreadProcessChargingPile(void *p);

    static void ThreadProcessCloud(void *p);

    static void ThreadKeep(void *p);

};


#endif //MBEDCTRL_MBEDCTRL_H
