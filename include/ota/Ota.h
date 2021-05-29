//
// Created by lining on 2021/5/21.
//

/*
 * ota Over-the-Air Technology 空中下载技术
 * @正常的更新流程
 * 1.通过各种途径获取文件及文件完整性验证码
 * 2.验证文件
 * 3.将原始文件进行备份,文件名+.bak+日期
 * 4.将下载文件替换原始文件
 * 5.写入更新记录
 * @失败补偿机制
 * 失败后，将备份文件按最指定的方式还原出来(指定日期，或者最新日期)
 * */



#ifndef _OTA_H
#define _OTA_H

#include <string>
#include <curl/curl.h>
#include <map>

using namespace std;

class Ota {
    enum FetchWay{
        Ftp = 0,//ftp download
        Https,// https download
    };
public:
    string usr;
    string passwd;
    CURL *curl = nullptr;
    long lastruntime = 0;
    long fileSize = 0;//文件总大小
    long fileProgress = 0;//已经下载大小
    long  timeout = 0;//超时限制 download
    int tryMax = 3;//最大尝试次数 upload

public:
    Ota();
    ~Ota();

public:
    int Upload(const string filePath, const string url);
    int Download(const string filePath, const string url);

    //完整性验证
    int FileVerifySHA1(string fileName, string digest);

    //从sha1文本中获取摘要对
    int GetFileDigest(string digestFileName, map<string,string> &map_digest);

    //按照日期备份文件
    string BackupFileByDate(string fileName);

};


#endif //_OTA_H
