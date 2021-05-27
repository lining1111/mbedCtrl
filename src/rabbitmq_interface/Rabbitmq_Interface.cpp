//
// Created by root on 2021/3/25.
//

#include <cstring>
#include "rabbitmq_interface/Rabbitmq_Interface.h"

Rabbitmq_Interface::Rabbitmq_Interface(string community_id) {
    this->config = default_config;
    this->community_id = community_id;
    isRun = false;
}

Rabbitmq_Interface::Rabbitmq_Interface(RabbitmqConfig rabbitmqConfig, string community_id) {
    this->config = rabbitmqConfig;
    this->community_id = community_id;
    isRun = false;
}

Rabbitmq_Interface::~Rabbitmq_Interface() {
    if (isRun) {
        this->Close();
    }
    isRun = false;
}

int Rabbitmq_Interface::Open() {
    //1. conn socket
    conn = amqp_new_connection();
    socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        cout << "creating amqp socket fail" << endl;
        return -1;
    }
    //2. get accountInfo
    string account_addr_url = "amqp://";

    account_addr_url.append(config.usrname);
    account_addr_url.append(":");

    account_addr_url.append(config.usrpasswd);
    account_addr_url.append("@");

    account_addr_url.append(config.hostname);
    account_addr_url.append(":");

    account_addr_url.append(to_string(config.hostport));
    cout << account_addr_url << endl;

    if (amqp_parse_url((char *) account_addr_url.c_str(), &accountInfo) != AMQP_STATUS_OK) {
        cout << "amqp url parse fail" << endl;
        return -2;
    }

    //3. open
    if (amqp_socket_open(socket, accountInfo.host, accountInfo.port)) {
        cout << "amqp socket open fail,host:" << accountInfo.host << ",port:" << to_string(accountInfo.port) << endl;
        return -3;
    }
    //4. login
    amqp_rpc_reply_t login_result = amqp_login(conn, config.vhost.c_str(), 0/*no limit*/,
                                               AMQP_DEFAULT_FRAME_SIZE, 0, AMQP_SASL_METHOD_PLAIN,
                                               accountInfo.user, accountInfo.password);
    if (login_result.reply_type != AMQP_RESPONSE_NORMAL) {
        cout << "amqp login fail,error:" << amqp_error_string2(login_result.library_error) << endl;
        return -4;
    }
    //5. open channel
    amqp_channel_open_ok_t *channel_open_result = amqp_channel_open(conn, config.channel);
    amqp_rpc_reply_t reply_result = amqp_get_rpc_reply(conn);
    if (reply_result.reply_type != AMQP_RESPONSE_NORMAL) {
        cout << "amqp open channel fail,error:" << amqp_error_string2(reply_result.library_error) << endl;
        return -5;
    }
    //6. bind
    if (community_id.empty()) {
        cout << "bind fail community empty" << endl;
        return -6;
    }
    GetBindInfo_Pay();

    amqp_queue_bind_ok_t *bind_ret = amqp_queue_bind(conn, config.channel,
                                                     amqp_cstring_bytes(queuename.c_str()),
                                                     amqp_cstring_bytes(bind_exchange.c_str()),
                                                     amqp_cstring_bytes(bind_routingkey.c_str()),
                                                     amqp_empty_table);
    amqp_rpc_reply_t bind_result = amqp_get_rpc_reply(conn);
    if (bind_result.reply_type != AMQP_RESPONSE_NORMAL) {
        cout << "bind fail" << endl;
        return -6;
    }

    //7.Consume消息等待配置
    cout << "Consume消息等待配置..." << endl;

    amqp_basic_consume(conn, config.channel, amqp_cstring_bytes(queuename.c_str()),
                       amqp_cstring_bytes(consumer_tag.c_str()),
                       0, 0, 0,
                       amqp_empty_table);
    amqp_rpc_reply_t consume_result = amqp_get_rpc_reply(conn);
    if (consume_result.reply_type != AMQP_RESPONSE_NORMAL) {
        cout << "consumer config fail" << endl;
        return -7;
    }

    isRun = true;
    return 0;
}

int Rabbitmq_Interface::Close() {
    isRun = false;
    amqp_channel_close(conn, config.channel, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
    return 0;
}

int Rabbitmq_Interface::GetBindInfo_Pay() {

//    queuename = "central.pay.";
//    queuename.append(community_id);
    queuename = "central_queue";

    //bind
//    bind_exchange = "send.central.exchange";
    bind_exchange = "msg.exchange";//server must exsit

//    bind_routingkey = "send.pay.";
//    bind_routingkey.append(community_id);
    bind_routingkey = "test";//self define

    //send in fact cloud use one
//    send_exchange = "receive.central.exchange";
//    send_routinggkey = "receive.pay.data";
    send_exchange = "msg.exchange";
    send_routinggkey = "test";//sel define

    consumer_tag = "/root/pay/pay_pro";//self

    return 0;
}

int Rabbitmq_Interface::Run() {
    if (!isRun) {
        return -1;
    }

    thread_consumer = thread(ThreadConsumer, this);
    thread_consumer.detach();

    thread_publisher = thread(ThreadPublisher, this);
    thread_publisher.detach();

    return 0;
}

/*
 * mq recv
 * */
int Rabbitmq_Interface::Consume_Fun(Rabbitmq_Interface::Msg &msg) {
    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;

    cout << "MQ消息等待..." << endl;
    amqp_maybe_release_buffers(conn);
    res = amqp_consume_message(conn, &envelope, nullptr, 0);

    if (AMQP_RESPONSE_NORMAL != res.reply_type) {
        cout << "MQ消息错误!!!!!" << endl;
        printf("Delivery %u || exchange %d:%s || routingkey %d:%s\n",
               (unsigned) envelope.delivery_tag, (int) envelope.exchange.len,
               (char *) envelope.exchange.bytes, (int) envelope.routing_key.len,
               (char *) envelope.routing_key.bytes);
        return -1;
    }
    if (envelope.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) {
        //printf("Content-type: %d:%s\n",
        // (int)envelope.message.properties.content_type.len,
        // (char *)envelope.message.properties.content_type.bytes);
    }
    cout << "rabbit receive begin" << endl;
    cout << string((char *) envelope.message.body.bytes) << endl;

    //copy
    memcpy(msg.buf, envelope.message.body.bytes, envelope.message.body.len);
    msg.len = envelope.message.body.len;

    amqp_destroy_envelope(&envelope);
    cout << "rabbit receive end" << endl;
    amqp_basic_ack(conn, config.channel,
                   envelope.delivery_tag, false);

    return 0;
}


void Rabbitmq_Interface::ThreadConsumer(void *p) {
    if (p == nullptr) {
        return;
    }
    Rabbitmq_Interface *rabbitmqInterface = (Rabbitmq_Interface *) p;

    cout << "thread consumer run" << endl;
    while (rabbitmqInterface->isRun) {
        pthread_mutex_lock(&rabbitmqInterface->lock_consumer);

        Rabbitmq_Interface::Msg msg;
        bzero(msg.buf, sizeof(msg.buf));
        rabbitmqInterface->Consume_Fun(msg);

        rabbitmqInterface->queue_consumer.push(msg);

        pthread_cond_broadcast(&rabbitmqInterface->cond_consumer);
        pthread_mutex_unlock(&rabbitmqInterface->lock_consumer);
    }

    cout << "thread consumer exit" << endl;
}

/*
 * mq send
 * */
int Rabbitmq_Interface::Publish_Fun(string msg_body) {
    if (!isRun) {
        return -1;
    }
    //1. 判断channel状态是否打开
    amqp_rpc_reply_t reply_result = amqp_get_rpc_reply(conn);
    if (reply_result.reply_type != AMQP_RESPONSE_NORMAL) {
        cout << "amqp response fail" << endl;
        return -1;
    }

    //2. publish
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2; /* persistent delivery mode */

    int ret = amqp_basic_publish(conn, config.channel,
                                 amqp_cstring_bytes(send_exchange.c_str()),
                                 amqp_cstring_bytes(send_routinggkey.c_str()),
                                 0, 0, &props,
                                 amqp_cstring_bytes(msg_body.c_str()));
    if (ret == AMQP_STATUS_OK) {
        cout << msg_body << " publish ok" << endl;
    } else {
        cout << msg_body << " publish fail" << endl;
        return -2;
    }
    return 0;
}

void Rabbitmq_Interface::ThreadPublisher(void *p) {
    if (p == nullptr) {
        return;
    }
    Rabbitmq_Interface *rabbitmqInterface = (Rabbitmq_Interface *) p;

    cout << "thread publisher run" << endl;
    while (rabbitmqInterface->isRun) {

        //copy
        pthread_mutex_lock(&rabbitmqInterface->lock_publisher);

        while (rabbitmqInterface->queue_publisher.empty()) {
            pthread_cond_wait(&rabbitmqInterface->cond_publisher, &rabbitmqInterface->lock_publisher);
        }

        while (!rabbitmqInterface->queue_publisher.empty()) {
            Rabbitmq_Interface::Msg msg;
            msg = rabbitmqInterface->queue_publisher.front();
            rabbitmqInterface->queue_publisher.pop();

            //publish
            rabbitmqInterface->Publish_Fun(msg.buf);
        }

        pthread_mutex_unlock(&rabbitmqInterface->lock_publisher);
    }

    cout << "thread publisher exit" << endl;

}

int Rabbitmq_Interface::Publish(Rabbitmq_Interface::Msg msg) {
    if (msg.len <= 0) {
        return -1;
    }
    //try send lock_queue_recv
    pthread_mutex_lock(&this->lock_publisher);
    this->queue_publisher.push(msg);
    pthread_cond_broadcast(&this->cond_publisher);
    pthread_mutex_unlock(&this->lock_publisher);

    return msg.len;
}

int Rabbitmq_Interface::PublishToBase(char *buf, int len) {
    Msg msg;
    bzero(msg.buf, sizeof(msg.buf));
    memcpy(msg.buf, buf, len);
    msg.len = len;
    return Publish(msg);

}
