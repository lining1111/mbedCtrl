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
    Rabbitmq_Interface *cloud= nullptr;//上游控制接口
    ChargingPile *chargingPile= nullptr;//充电桩接口
public:
    bool isRun=false;
    thread thread_processChargingPileRecv;//充电桩业务处理线程
    thread thread_processCloudRecv;//上游业务处理线程
    thread thread_keep;//重连线程

public:
    MbedCtrl();
    ~MbedCtrl();
    int Open();
    int Run();
    int Close();

private:
    static void ThreadProcessChargingPile(void *p);
    static void ThreadProcessCloud(void *p);
    static void ThreadKeep(void *p);

};


#endif //MBEDCTRL_MBEDCTRL_H
