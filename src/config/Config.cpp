//
// Created by root on 2021/3/23.
//

#ifdef arm
#include <json/json.h>
#endif

#ifdef arm64

#include <json/json.h>

#endif

#ifdef x86

#include <jsoncpp/json/json.h>

#endif

#include <fstream>
#include <iostream>

#include "config/Config.h"

using namespace std;

int Config::GetConfig(string config_path) {
    if (config_path.empty()) {
        cout << "config path empty" << endl;
        return -1;
    }
    Json::Value root;
    Json::Reader reader;

    ifstream ifstream1;

    ifstream1.open(config_path);

    if (reader.parse(ifstream1, root) == false) {
        cout << "config get fail" << endl;
        return -1;
    }

    server_ip = root[config_server_ip].asString();
    server_port = root[config_server_port].asInt();
    cid = root[config_cid].asString();
    local_ip = root[config_local_ip].asString();

    ifstream1.close();
    return 0;
}

int Config::SetConfig(string config_path) {
    if (config_path.empty()) {
        cout << "config path empty" << endl;
        return -1;
    }

    Json::Value root;
    Json::FastWriter writer;

    root[config_server_ip] = server_ip;
    root[config_server_port] = server_port;
    root[config_cid] = cid;
    root[config_local_ip] = local_ip;

    ofstream ofstream1;
    ofstream1.open(config_path);
    ofstream1 << writer.write(root);
    ofstream1.close();

    return 0;
}
