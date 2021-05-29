//
// Created by root on 2021/2/27.
//

#include <fstream>
#include <iostream>
#include <zlog.h>
#include <dirent.h>
#include "log/log.h"

zlog_category_t *m_zlog_category;

string logConfigInitByIndex(const string org, int index) {
    //example ./logExample.conf
    //[global]
    //strict logInit = true
    //buffer min = 1024
    //buffer max = 2MB
    //rotate lock_queue_recv file = /tmp/zlog%d.lock_queue_recv
    //file perms = 600
    //default format = "[%d][%V]%m%n"
    //[rules]
    //*.*     "log/beeComm%d-%d(%Y-%m-%d).log", 10MB ~ "log/beeComm%d-%d(%Y-%m-%d).#2s.log"
    //log_conf.* "log/beeComm%d-%d(%Y-%m-%d).log";
    const string compare1 = "beeComm%d";
    const string compare2 = "zlog%d.lock_queue_recv";

    string fileSaveName = "./beeComm" + to_string(index) + ".conf";
    string content = "";

    ifstream fs(org.c_str());
    if (fs.good()) {
        while (!fs.eof()) {
            string line;
            getline(fs, line);
            int pos = -1;
            //compare1
            while ((pos = line.find(compare1, pos + 1)) != string::npos) {
                string replace = "beeComm" + to_string(index);
                line.replace(pos, compare1.length(), replace);
            }
            //compare2
            while ((pos = line.find(compare2, pos + 1)) != string::npos) {
                string replace = "zlog" + to_string(index) + ".lock_queue_recv";
                line.replace(pos, compare2.length(), replace);
            }

            content.append(line);
            if (!line.empty()) {
                content.append("\n");
            }
        }
        content.erase(content.length() - 1);
        fs.close();
        //写入文件
        ofstream out;

        out.open(fileSaveName);
        out.flush();
        out << content;
        out.flush();
        out.close();
    } else {
        std::cout << "can't Open file:" << org << std::endl;
        return "error";
    }
    return fileSaveName;
}

int logInit(int index) {
    int ret = 0;

    string confPath = "";
    confPath = logConfigInitByIndex(configExamplePath, index);
    if (confPath != "error") {
        zlog_fini();
        int rc = zlog_init(confPath.c_str());
        if (rc == -1) {
            std::cout << "logInit failed" << std::endl;
            zlog_fini();
            ret = -1;
        } else {
            //检测dir是否存在，不存在则创建
            DIR *dir = opendir(logPath.c_str());
            if (dir == nullptr) {
                string cmd = "mkdir -p " + logPath;
                system(cmd.c_str());
            }
            m_zlog_category = zlog_get_category(configName.c_str());
            ret = 0;
        }
    } else {
        ret = -1;
    }
    return ret;
}

//void LogInfo(const char *szlog, ...) {
//    char log[2048] = {0};
//    va_list args;
//    va_start(args, szlog);
//    vsprintf(log, szlog, args);
//    va_end(args);
//    zlog_info(m_zlog_category, log);
//    return;
//}
//
//void LogError(const char *szlog, ...) {
//    char log[2048] = {0};
//    va_list args;
//    va_start(args, szlog);
//    vsprintf(log, szlog, args);
//    va_end(args);
//    zlog_error(m_zlog_category, log);
//    return;
//}