//
// Created by root on 2021/3/29.
//

#include <unistd.h>
#include <iostream>
#include "database/DataBase.h"
#include "log/log.h"

DataBase::DataBase() {
    this->serverIp = default_serverIp;
    this->serverPort = default_serverPort;
    this->table = default_table;
    this->usr = default_usr;
    this->passwd = default_passwd;
}

DataBase::DataBase(string ip, int port, string table, string usr, string passwd) {
    this->serverIp = ip;
    this->serverPort = port;
    this->table = table;
    this->usr = usr;
    this->passwd = passwd;

}

DataBase::~DataBase() {
    this->Close();
}

int DataBase::Open() {
    if (mysql_init(&mysql) == nullptr) {
        LogInfo("mysql init failed");
        return -1;
    }

    char value = 1;
    mysql_options(&mysql,MYSQL_OPT_RECONNECT,&value);


    if (mysql_real_connect(&mysql, this->serverIp.c_str(), this->usr.c_str(),
                           this->passwd.c_str(), this->table.c_str(), this->serverPort, nullptr, 0) == nullptr) {
        LogInfo("connect mysql failed,error:%s", mysql_error(&mysql));
        mysql_library_end();
        return -1;
    }
    LogInfo("mysql opened,version:%s", mysql.server_version);

    int ret = -1;
    int tryCount = 0;
    const int tryMax = 5;

    do {
        ret = mysql_query(&mysql, "set names utf8");
        tryCount++;
        if (tryCount > tryMax) {
            break;
        }
        if (ret != 0) {
            usleep(1000);
            LogError("mysql query fails,error:%s", mysql_error(&mysql));
            if ((ret == CR_SERVER_GONE_ERROR) || (ret == CR_SERVER_LOST)) {
                break;
            }
        }
    } while (ret != 0);

    isRun = true;
    return 0;
}

int DataBase::Close() {
    if (isRun) {
        mysql_close(&mysql);
        mysql_library_end();
    }
    isRun = false;
    return 0;
}

int DataBase::Database_Query(string sql_cmd, MYSQL_RES **res) {

    int ret = -1;
    int tryCount = 0;
    const int tryMax = 5;

    do {
        ret = mysql_query(&mysql, sql_cmd.c_str());
        tryCount++;
        if (tryCount > tryMax) {
            break;
        }
        if (ret != 0) {
            usleep(1000);
            LogError("mysql query fails:%s,sql stat:%s,error:%s", sql_cmd.c_str(), mysql_stat(&mysql),
                     mysql_error(&mysql));
            if ((ret == CR_SERVER_GONE_ERROR) || (ret == CR_SERVER_LOST)) {
                sql_err = ret;
                isRun = false;
                break;
            }
        }
    } while (ret != 0);

    if (ret == 0) {
        LogInfo("mysql query ok:%s", sql_cmd.c_str());
        *res = mysql_store_result(&mysql);
        return 0;
    } else {
        return -1;
    }
}

int DataBase::Run() {

    if (!isRun) {
        return -1;
    }

    thread_keep = thread(Keep_Fun, this);
    thread_keep.detach();

    return 0;
}

void DataBase::Keep_Fun(void *p) {
    if (p == nullptr) {
        return;
    }
    auto dataBase = (DataBase *) p;
    cout << "DataBase::ThreadCheckStatus run" << endl;
    while (dataBase->isRun) {
        sleep(1);
        if ((dataBase->sql_err == CR_SERVER_GONE_ERROR) || (dataBase->sql_err == CR_SERVER_LOST)) {
            //reconnect
            dataBase->Close();
            dataBase->Open();
        }
    }
    cout << "DataBase::ThreadCheckStatus exit" << endl;
}

int DataBase::CheckResult(MYSQL_RES *res, bool isCheckLine) {
    int line = 0;
    if (!res) {
        cout << "mysql result is null" << endl;
        line = -1;
    } else {
        if (isCheckLine) {
            line = mysql_num_rows(res);
        } else {
            line = 0;
        }
    }

    return line;
}
