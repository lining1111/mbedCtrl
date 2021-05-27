//
// Created by root on 2021/3/29.
//

#include <iostream>
#include "database/DataBase.h"


int main() {
    DataBase *dataBase = new DataBase();

    dataBase->Open();

    dataBase->Close();
    //test
    string sql_cmd = "select camera_ip_addr,sn,display_factory,camera_factory,in_out_type from device_info order by inet_aton(camera_ip_addr) asc";

    MYSQL_RES *res = nullptr;
    dataBase->Database_Query(sql_cmd, &res);

    if (res == nullptr) {
        cout << "error" << endl;
    }
    mysql_free_result(res);
    //test

    dataBase->Close();

    delete dataBase;
}