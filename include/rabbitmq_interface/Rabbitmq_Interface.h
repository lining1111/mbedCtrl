//
// Created by root on 2021/3/25.
//

#ifndef MY_RPI_DOOR_RABBITMQ_INTERFACE_H
#define MY_RPI_DOOR_RABBITMQ_INTERFACE_H

#include <iostream>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <queue>
#include <pthread.h>
#include <condition_variable>
#include <thread>

using namespace std;

class Rabbitmq_Interface {

    struct RabbitmqConfig {
        string hostname;
        unsigned int hostport;
        string usrname;
        string usrpasswd;
        string vhost;
        unsigned int channel;
    };


public:
    RabbitmqConfig default_config = {
            .hostname = "127.0.0.1",
            .hostport= 5672,
            .usrname = "rabbitmq_example_usr",
            .usrpasswd = "rabbitmq_example_passwd",
            .vhost = "rabbitmq_example",
            .channel = 1,
    };
    const unsigned int max_line = 1024 * 10;//10K

public:
    RabbitmqConfig config;

    //rabbitmq conn
    amqp_socket_t *socket = nullptr;
    amqp_connection_state_t conn;
    struct amqp_connection_info accountInfo;


    //usr config
    string community_id;

    string bind_exchange;
    string bind_routingkey;

    string send_exchange;
    string send_routinggkey;

    string queuename;
    string consumer_tag;

    bool isRun = false;

    struct Msg {
        char buf[1024 * 128];
        int len;
    };
    thread thread_consumer;

    pthread_mutex_t lock_consumer = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;
    queue<Msg> queue_consumer;

    thread thread_publisher;

    pthread_mutex_t lock_publisher = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_publisher = PTHREAD_COND_INITIALIZER;
    queue<Msg> queue_publisher;

public:
    Rabbitmq_Interface(string community_id);

    Rabbitmq_Interface(RabbitmqConfig rabbitmqConfig, string community_id);

    ~Rabbitmq_Interface();

    int Open();

    int Close();

    int GetBindInfo_Pay();

    int Run();

private:
    int Consume_Fun(Msg &msg);

    static void ThreadConsumer(void *p);

    int Publish_Fun(string msg_body);

    static void ThreadPublisher(void *p);

public:
    int Publish(Msg msg);

    int PublishToBase(char *buf, int len);

};


#endif //MY_RPI_DOOR_RABBITMQ_INTERFACE_H
