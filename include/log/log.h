//
// Created by lining on 2021/2/27.
//

#ifndef _LOG_H
#define _LOG_H

#include <string>
#include <zlog.h>

#ifdef __cplusplus
extern "C"
{
#endif

using namespace std;


const string configExamplePath = "./logExample.conf";
const string configName = "log_conf";
const string logPath = "log";

extern zlog_category_t *m_zlog_category;
extern string logConfigInitByIndex(const string org, int index);
extern int logInit(int index);

#define LogInfo(format, ...)\
    zlog_info(m_zlog_category,format,##__VA_ARGS__)

#define LogError(format, ...)\
    zlog_error(m_zlog_category,format,##__VA_ARGS__)


#ifdef __cplusplus
}
#endif



#endif //_LOG_H
