//
// Created by mylord on 2019/9/26.
//

#ifndef _PING_H
#define _PING_H

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C"
{
#endif


#define ICMP_DATA_LEN 56        //ICMP默认数据长度
#define ICMP_HEAD_LEN 8            //ICMP默认头部长度
#define ICMP_LEN  (ICMP_DATA_LEN + ICMP_HEAD_LEN)

#define BUFSIZE 1500     //发送缓存最大值
#define DEFAULT_LEN 56   //ping 消息数据默认大小

typedef enum {
    ip_notReach = 0, ip_reach = 1
} IpLive;

IpLive live_check(const char *ip);
void send_ping();    //发送ping消息
int recv_reply();    //接收ping应答
uint16_t checksum(uint8_t *buf, int len);    //计算校验和
int handle_pkt();    //ICMP应答消息处理
void set_sighandler();    //设置信号处理程序
void get_statistics(int, int);    //统计ping命令的检测结果
void bail(const char *);    //错误报告
void int_handler(int);    //SIGINT处理程序
void alarm_handler(int);    //SIGALRM处理程序


#ifdef __cplusplus
}
#endif


#endif //_PING_H
