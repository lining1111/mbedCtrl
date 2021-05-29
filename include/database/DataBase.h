//
// Created by lining on 2021/3/29.
//

#ifndef _DATABASE_H
#define _DATABASE_H

#ifdef arm64

#include <mysql.h>
#include <errmsg.h>

#else
#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#endif

#include <string>
#include <thread>

using namespace std;

class DataBase {
public:
    const string default_serverIp = "127.0.0.1";
    const int default_serverPort = 3306;
    const string default_table = "parking_v3";
    const string default_usr = "root";
    const string default_passwd = "admin";
public:
    MYSQL mysql;
    bool isRun = false;
    int sql_err = 0;

    string serverIp;
    int serverPort;
    string table;
    string usr;
    string passwd;


    thread thread_keep;
public:
    DataBase();

    DataBase(string ip, int port, string table, string usr, string passwd);

    ~DataBase();


    int Open();

    int Run();

    int Close();

    static void Keep_Fun(void *p);

    int Database_Query(string sql_cmd, MYSQL_RES **res);

    int CheckResult(MYSQL_RES *res, bool isCheckLine);
};


#endif //_DATABASE_H
