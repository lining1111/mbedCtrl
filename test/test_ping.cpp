//
// Created by root on 2021/4/6.
//
#include "ping/ping.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {

    vector<int> vector1;

    vector<int> vector_arr[3];
    vector1.push_back(1);
    vector1.push_back(2);
    vector1.push_back(3);

    int tmp;

    tmp = vector1.at(0);
    vector1.at(0) = vector1.at(1);
    vector1.at(1) = tmp;

    vector_arr[0].push_back(1);
    vector_arr[0].push_back(2);

    int a = vector_arr[0][0];
    int b = vector_arr[0].at(1);
//    int len = sizeof(time_t);
//    len = sizeof(long );
//    len = sizeof(long long );
//    len = sizeof(int);

    string ip;

    ip = argv[1];

    while (true) {
        sleep(3);
        IpLive ipLive = live_check(ip.c_str());
        if (ipLive == ip_notReach) {
            cout << ip << " ping out" << endl;
        }
    }
}