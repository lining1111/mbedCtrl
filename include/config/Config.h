//
// Created by lining on 2021/3/23.
//

#ifndef _CONFIG_H
#define _CONFIG_H

#include <string>

using namespace std;

class Config {
public:
    const string default_server_ip = "www.kuaiban365.com";
    const unsigned int default_server_port = 18888;

    const string config_path = "/boot/systemset.json";
    const string config_server_ip = "serverip";
    const string config_server_port = "serverport";
    const string config_cid = "CID";
    const string config_local_ip = "localip";
public:
    string server_ip;
    unsigned int server_port;
    string cid;
    string local_ip;

public:
    int GetConfig(string config_path);

    int SetConfig(string config_path);


};


#endif //_CONFIG_H
