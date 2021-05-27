//
// Created by root on 2021/3/23.
//

#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <netdb.h>
#include <arpa/inet.h>

#ifdef arm
#include <json/json.h>
#endif

#ifdef arm64

#include <json/json.h>

#endif

#ifdef x86

#include <jsoncpp/json/json.h>

#endif

using namespace std;


const string config_path = "/boot/systemset.json";

string GetRealIpByDomainName(string _name) {
    string ip;
    struct hostent *host;

    int tryCount = 0;
    const int maxTryCount = 10;
    do {
        tryCount++;
        host = gethostbyname(_name.c_str()/*"www.kuaiban365.com"*/);
        if (host == nullptr) {
            cout << "error in gethostbyname!" << endl;
        } else {
            ip = inet_ntoa(*(struct in_addr *) host->h_addr_list[0]);
        }
        if (tryCount > maxTryCount) {
            break;
        }
        sleep(6);
    } while (ip.empty());

    return ip;
}


//sysSet www.kuaiban365.com 192.168.1.100
int main(int argc, char **argv) {

    if (argc < 3) {
        printf("Usage:<%s> num(>=1)", argv[0]);
        return 0;
    }


    // get ip from www
    string ip;
    ip = GetRealIpByDomainName(argv[1]);

    Json::Value root;
    Json::FastWriter fw;


    root["serverip"] = ip;
    root["serverport"] = 18888;
    root["CID"] = "0005";
    root["localip"] = argv[2];


    if (ip.empty() == 0) {
        ofstream ofs;
        ofs.open(config_path);
        ofs << fw.write(root);
        ofs.close();

        printf("write is over \n");

    }

    return 0;

}