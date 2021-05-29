//
// Created by lining on 5/28/21.
//

#include <unistd.h>
#include <fstream>
#include <cassert>
#include "MbedCtrl.h"
#include "log/log.h"

using namespace std;

MbedCtrl::MbedCtrl(string configFilePath) {
    this->configFilePath = configFilePath;
    if (!configFilePath.empty()) {
        GetConfig(configFilePath);
    }

    cloud = new Rabbitmq_Interface(cloudConfig, community_id);
    chargingPile = new Server_Interface(this->chargingPileServerPort,
                                        ChargingPile::ProcessSubServerRecv, this->chargingPileHeartBeatTv);
}

MbedCtrl::~MbedCtrl() {
    delete cloud;
    delete chargingPile;
}

int MbedCtrl::Open() {

    if (cloud->Open() == 0) {
        cloud->Run();
    }

    if (chargingPile->Open() == 0) {
        chargingPile->Run();
    }
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

void MbedCtrl::SetConfig(string filePath) {
    if (filePath.empty()) {
        cout << "config file path empty" << endl;
    }

    ofstream writer(filePath);
    if (writer.is_open()) {
        //依次将配置中的值写入config
        // cloud
        //cloud_hostname
        {
            string str = ConfigKey(cloud_hostname);
            str.append(" ");
            str.append(this->cloudConfig.hostname);
            writer << str << endl;
        }
        //cloud_hostport
        {
            string str = ConfigKey(cloud_hostport);
            str.append(" ");
            str.append(to_string(this->cloudConfig.hostport));
            writer << str << endl;
        }
        //cloud_usrname
        {
            string str = ConfigKey(cloud_usrname);
            str.append(" ");
            str.append(this->cloudConfig.usrname);
            writer << str << endl;
        }
        //cloud_usrpasswd
        {
            string str = ConfigKey(cloud_usrpasswd);
            str.append(" ");
            str.append(this->cloudConfig.usrpasswd);
            writer << str << endl;
        }
        //cloud_vhost
        {
            string str = ConfigKey(cloud_vhost);
            str.append(" ");
            str.append(this->cloudConfig.vhost);
            writer << str << endl;
        }
        //cloud_channel
        {
            string str = ConfigKey(cloud_channel);
            str.append(" ");
            str.append(to_string(this->cloudConfig.channel));
            writer << str << endl;
        }
        //cloud_community
        {
            string str = ConfigKey(cloud_community);
            str.append(" ");
            str.append(this->community_id);
            writer << str << endl;
        }
        //chargingPile
        //chargingPile_serverPort
        {
            string str = ConfigKey(chargingPile_serverPort);
            str.append(" ");
            str.append(to_string(this->chargingPileServerPort));
            writer << str << endl;
        }
        //chargingPile_heartBeat
        {
            string str = ConfigKey(chargingPile_heartBeat);
            str.append(" ");
            str.append(to_string(this->chargingPileHeartBeatTv));
            writer << str << endl;
        }

        writer.flush();
        writer.close();
    }

}

void MbedCtrl::GetConfig(string filePath) {
    if (filePath.empty()) {
        cout << "config file path empty" << endl;
    }

    map<string, string> config;
    ifstream reader(filePath);
    assert(reader);

    if (reader.is_open()) {
        while (!reader.eof()) {
            string line;
            getline(reader, line);
            if (line.empty()){
            continue;
            }
            if (line.find("#")==0){
                //以#开头的认为是注释
                continue;
            }

            const auto parse = [](const std::string &str) {
                std::string tmp, key, value;
                for (size_t i = 0, len = str.length(); i < len; ++i) {
                    const char ch = str[i];
                    if (ch == ' ') {
                        if (i > 0 && str[i - 1] != ' ' && key.empty()) {
                            key = tmp;
                            tmp.clear();
                        }
                    } else {
                        tmp.push_back(ch);
                    }
                    if (i == len - 1) {
                        value = tmp;
                    }
                }
                return std::make_pair(key, value);
            };

            auto kv = parse(line);
            config[kv.first] = kv.second;
        }
        reader.close();
    }

    // cloud
    //cloud_hostname
    {
        auto iter = config.find(ConfigKey(cloud_hostname));
        if (iter != config.end()) {
            //find
            this->cloudConfig.hostname = iter->second;
        } else {
            //default value;
            this->cloudConfig.hostname = "127.0.0.1";
        }
    }
    //cloud_hostport
    {
        auto iter = config.find(ConfigKey(cloud_hostport));
        if (iter != config.end()) {
            //find
            this->cloudConfig.hostport = atoi(iter->second.c_str());
        } else {
            //default value;
            this->cloudConfig.hostport = 15672;
        }
    }
    //cloud_usrname
    {
        auto iter = config.find(ConfigKey(cloud_usrname));
        if (iter != config.end()) {
            //find
            this->cloudConfig.usrname = iter->second;
        } else {
            //default value;
            this->cloudConfig.usrname = "guest";
        }
    }
    //cloud_usrpasswd
    {
        auto iter = config.find(ConfigKey(cloud_usrpasswd));
        if (iter != config.end()) {
            //find
            this->cloudConfig.usrpasswd = iter->second;
        } else {
            //default value;
            this->cloudConfig.usrpasswd = "guest";
        }
    }
    //cloud_vhost
    {
        auto iter = config.find(ConfigKey(cloud_vhost));
        if (iter != config.end()) {
            //find
            this->cloudConfig.vhost = iter->second;
        } else {
            //default value;
            this->cloudConfig.vhost = "/";
        }
    }
    //cloud_channel
    {
        auto iter = config.find(ConfigKey(cloud_channel));
        if (iter != config.end()) {
            //find
            this->cloudConfig.channel = atoi(iter->second.c_str());
        } else {
            //default value;
            this->cloudConfig.channel = 1;
        }
    }
    //cloud_community
    {
        auto iter = config.find(ConfigKey(cloud_community));
        if (iter != config.end()) {
            //find
            this->community_id = iter->second;
        } else {
            //default value;
            this->community_id = "";
        }
    }
    //chargingPile
    //chargingPile_serverPort
    {
        auto iter = config.find(ConfigKey(chargingPile_serverPort));
        if (iter != config.end()) {
            //find
            this->chargingPileServerPort = atoi(iter->second.c_str());
        } else {
            //default value;
            this->chargingPileServerPort = 8131;
        }
    }
    //chargingPile_heartBeat
    {
        auto iter = config.find(ConfigKey(chargingPile_heartBeat));
        if (iter != config.end()) {
            //find
            this->chargingPileHeartBeatTv = atoi(iter->second.c_str());
        } else {
            //default value;
            this->chargingPileHeartBeatTv = 0;
        }
    }

}

void MbedCtrl::ThreadProcessChargingPile(void *p) {
    auto mbedCtrl = (MbedCtrl *) p;
    auto chargingPile = mbedCtrl->chargingPile;
    auto cloud = mbedCtrl->cloud;
    cout << "MbedCtrl:" << __FUNCTION__ << " run" << endl;
    while (!mbedCtrl->isRun) {
        //获取充电桩接收的信息
        usleep(1000 * 10);
        if (chargingPile->queue_recv.empty()) {
            continue;
        }
        pthread_mutex_lock(&chargingPile->lock_recv);
        while (chargingPile->queue_recv.empty()) {
            pthread_cond_wait(&chargingPile->cond_recv, &chargingPile->lock_recv);
        }

        while (!chargingPile->queue_recv.empty()) {
            Server_Interface::Msg msg;
            msg = chargingPile->queue_send.front();
            chargingPile->queue_send.pop();

            cout << "chargingPile recv:" << msg.buf << endl;

            //TODO
            //1.判断消息类型，如果是上报的设备标识信息，则记录到本地，不上传上游
            //2.如果消息是设备操作的回送消息，则上报上游。

        }

        pthread_mutex_unlock(&chargingPile->lock_vector_client);

    }
    cout << "MbedCtrl:" << __FUNCTION__ << " exit" << endl;
}

void MbedCtrl::ThreadProcessCloud(void *p) {
    auto mbedCtrl = (MbedCtrl *) p;
    auto chargingPile = mbedCtrl->chargingPile;
    auto cloud = mbedCtrl->cloud;
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
    auto chargingPile = mbedCtrl->chargingPile;
    auto cloud = mbedCtrl->cloud;
    cout << "MbedCtrl:" << __FUNCTION__ << " run" << endl;
    while (!mbedCtrl->isRun) {
        usleep(1000 * 10);
        if (!chargingPile->isRun) {
            //重启server
            chargingPile->Close();
            if (chargingPile->Open() == 0) {
                chargingPile->Run();
                LogInfo("chargingPile reopen");
            }
        }

        if (!cloud->isRun) {
            cloud->Close();
            if (cloud->Open() == 0) {
                cloud->Run();
                LogInfo("cloud reopen");
            }
        }
    }
    cout << "MbedCtrl:" << __FUNCTION__ << " exit" << endl;
}
