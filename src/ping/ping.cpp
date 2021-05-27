//
// Created by mylord on 2019/9/26.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netinet/in.h>
#include "ping/ping.h"

const char *hostname; //被ping的主机
int datalen = DEFAULT_LEN; //ICMP消息携带的数据长度
char sendbuf[BUFSIZE];
char recvbuf[BUFSIZE];
int nsent; //发送的ICMP消息序号
int nrecv;
pid_t pid; //ping程序的进程pid
struct timeval recvtime; //收到ICMP应答的时间戳
int sockfd; //发送和接收原始套接字
struct sockaddr_in dest; //被ping主机的ip
struct sockaddr_in from; //发送ping应答消息的主机ip

struct sigaction act_alarm;
struct sigaction act_int;

//设置的时间是一个结构体，倒计时设置，重复倒时，超时值设为1秒
struct itimerval val_alarm;

/***********************************************
 *
 *函数名字	: live_check
 *作者		: niujianglong
 *功能		: 通过icmp数据包检测摄像头是否在线
 *参数		: ip：摄像头IP地址
 *返回		: 0：不在线
 1：在线
 *创建时间	: 2018-08-03 14:29:10
 *
 ***********************************************/
IpLive live_check(const char *ip) {
    val_alarm.it_interval.tv_sec = 0;
    val_alarm.it_interval.tv_usec = 0;
    val_alarm.it_value.tv_sec = 0;
    val_alarm.it_value.tv_usec = 1;
    struct hostent *host; //该结构体属于include<netdb.h>
    int ret;

    if (ip == nullptr) {
        printf("ip NULL\n");
        return ip_notReach;
    }

//	LogInfo("check ip:%s", ip);
    if ((host = gethostbyname(ip)) == nullptr) {
        //gethostbyname()返回对应于给定主机名的包含主机名字和地址信息的结构指针,
        perror("can not understand the	host name");        //理解不了输入的地址
        return ip_notReach;
    }

    hostname = ip;        //取出地址名

    memset(sendbuf, 0, BUFSIZE);
    memset(recvbuf, 0, BUFSIZE);
    memset(&dest, 0, sizeof dest);//将dest中前sizeof(dest)个字节替换为0并返回s,此处为初始化,给最大内存清零
    dest.sin_family = PF_INET;    //PF_INET为IPV4，internet协议，在<netinet/in.h>中，地址族
    dest.sin_port = ntohs(0);    //端口号,ntohs()返回一个以主机字节顺序表达的数。
    dest.sin_addr = *(struct in_addr *) host->h_addr_list[0];//host->h_addr_list[0]是地址的指针.返回IP地址，初始化

    if ((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        //PF_INEI套接字协议族，SOCK_RAW套接字类型，IPPROTO_ICMP使用协议，调用socket函数来创建一个能够进行网络通信的套接字。这里判断是否创建成功
        perror("raw socket created	error");
        //exit(1);
        return ip_notReach;
    }

    //设置接受和发生超时
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout,
               sizeof(struct timeval));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout,
               sizeof(struct timeval));

    ret = setuid(getuid());    //getuid()函数返回一个调用程序的真实用户ID,setuid()是让普通用户可以以root用户的角色运行只有root帐号才能运行的程序或命令。
    pid = getpid();    //getpid函数用来取得目前进程的进程识别码
    set_sighandler();    //对信号处理
    //printf("Ping %s(%s): %d bytes data in ICMP packets.\n",ip,inet_ntoa(dest.sin_addr),datalen);

    if ((setitimer(ITIMER_REAL, &val_alarm, NULL)) == -1)        //定时函数
        printf("set timer fails.\n");

    ret = recv_reply();        //接收ping应答
    if (ret == 0) {
//		LogInfo("device_ip:%s is live", ip);
        close(sockfd);
        return ip_reach;
    } else {
        printf("device_ip:%s is not live\n", ip);
        close(sockfd);
        return ip_notReach;
    }

    return ip_notReach;
}

/***********************************************
 *
 *函数名字	: main(int argc,char **argv)
 *作者		: niujianglong
 *功能		:程序入口函数,间隔一定时间来检测摄像头在线情况
 *参数		:
 *返回		:
 *创建时间	: 2018-08-03 14:34:09
 *
 ***********************************************/
#if 0
int main(int argc,char **argv)
{

    live_check("192.168.1.75");
}
#endif

//发送ping消息
void send_ping() {
    struct icmp *pIcmp;
    int len;
    pIcmp = (struct icmp *) (sendbuf);  //字符串指针
    pIcmp->icmp_type = ICMP_ECHO;    //初始化ICMP消息类型type
    pIcmp->icmp_code = 0;    //初始化消息代码code
    pIcmp->icmp_id = pid;   //把进程标识码初始给icmp_id
    pIcmp->icmp_seq = nsent++;  //发送的ICMP消息序号赋值给icmp序号
    gettimeofday((struct timeval *) pIcmp->icmp_data, NULL); // 获取当前时间

    len = ICMP_LEN;
    pIcmp->icmp_cksum = 0;    //初始化
    pIcmp->icmp_cksum = checksum((uint8_t *) pIcmp, len);  //计算校验和

//	printf("The send pack checksum is:0x%x\n",icmp_hdr->checksum);
    sendto(sockfd, sendbuf, len, 0, (struct sockaddr *) &dest, sizeof(dest)); //经socket传送数据
}

//接收程序发出的ping命令的应答
int recv_reply() {
    int n;
    socklen_t len;
    int errno;

    n = nrecv = 0;
    len = sizeof(from);   //发送ping应答消息的主机IP

    while (nrecv < 1) {
        if ((n = recvfrom(sockfd, recvbuf, sizeof recvbuf, 0,
                          (struct sockaddr *) &from, &len)) < 0) { //经socket接收数据,如果正确接收返回接收到的字节数，失败返回0.
            if (errno == EINTR) {  //EINTR表示信号中断
                continue;
            }
            printf("recvfrom error\n");
        }

        gettimeofday(&recvtime, NULL);   //记录收到应答的时间

        if (handle_pkt() < 0) {    //接收到错误的ICMP应答信息
            return -1;
            //continue;
        }
        nrecv++;
    }

    //get_statistics(nsent,nrecv);     //统计ping命令的检测结果
    return 0;
}

//计算校验和
uint16_t checksum(uint8_t *buf, int len) {
    uint32_t sum = 0;
    uint16_t *cbuf;

    cbuf = (uint16_t *) buf;

    while (len > 1) {
        sum += *cbuf++;
        len -= 2;
    }

    if (len)
        sum += *(uint8_t *) cbuf;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return ~sum;
}

//ICMP应答消息处理
int handle_pkt() {
    struct ip *pIp;
    struct icmp *icmp;

    int ip_hlen;
    uint16_t ip_datalen; //ip数据长度
    //double rtt; // 往返时间
    //struct timeval *sendtime;

    pIp = (struct ip *) recvbuf;

    ip_hlen = pIp->ip_hl << 2;
    ip_datalen = ntohs(pIp->ip_len) - ip_hlen;

    icmp = (struct icmp *) (recvbuf + ip_hlen);

    uint16_t sum = (uint16_t) checksum((uint8_t *) icmp, ip_datalen);
    if (sum) { //计算校验和
        return -1;
    }

    if (icmp->icmp_id != (typeof(icmp->icmp_id)) pid) {
        return -1;
    }
    if (icmp->icmp_type != ICMP_ECHOREPLY) {
        return -1;
    }

#if 0
    sendtime=(struct timeval *)icmp->icmp_timestamp;	//发送时间
    rtt=((&recvtime)->tv_sec-sendtime->tv_sec)*1000+((&recvtime)->tv_usec-sendtime->tv_usec)/1000.0;//	往返时间
    //打印结果
    printf("%d	byte from %s:icmp_seq=%u ttl=%d	rtt=%.3f ms\n",
            ip_datalen,		//IP数据长度
            inet_ntoa(from.sin_addr),			//目的ip地址
            icmp->icmp_seq,			//icmp报文序列号
            pIp->ttl,			//生存时间
            rtt);	//往返时间
#endif

    return 0;
}

//设置信号处理程序
void set_sighandler() {
    act_alarm.sa_handler = alarm_handler;
    if (sigaction(SIGALRM, &act_alarm, NULL) ==
        -1) //sigaction()会依参数signum指定的信号编号来设置该信号的处理函数。参数signum指所要捕获信号或忽略的信号，&act代表新设置的信号共用体，NULL代表之前设置的信号处理结构体。这里判断对信号的处理是否成功。
        printf("SIGALRM handler setting fails.\n");

    act_int.sa_handler = int_handler;
    if (sigaction(SIGINT, &act_int, NULL) == -1)
        printf("SIGALRM handler setting fails.\n");
}

//统计ping命令的检测结果
void get_statistics(int nsent, int nrecv) {
    printf("--- %s ping statistics ---\n", inet_ntoa(dest.sin_addr)); //将网络地址转换成“.”点隔的字符串格式。
    printf("%d packets transmitted, %d received, %0.0f%% packet loss\n",
            nsent, nrecv, 1.0 * (nsent - nrecv) / nsent * 100);
}

//错误报告
void bail(const char *on_what) {
    fputs(strerror(errno),
          stderr);    //:向指定的文件写入一个字符串（不写入字符串结束标记符‘\0’）。成功写入一个字符串后，文件的位置指针会自动后移，函数返回值为0；否则返回EOR(符号常量，其值为-1)。
    fputs(":", stderr);
    fputs(on_what, stderr);
    fputc('\n', stderr); //送一个字符到一个流中
    exit(1);
}

//SIGINT（中断信号）处理程序
void int_handler(int sig) {
    //get_statistics(nsent,nrecv);	//统计ping命令的检测结果
    close(sockfd);  //关闭网络套接字
    exit(1);
}

//SIGALRM（终止进程）处理程序
void alarm_handler(int signo) {
    send_ping();    //发送ping消息
}

