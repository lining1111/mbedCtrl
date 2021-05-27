# mbedCtrl
上游：rabbitmq。下游：c/s

### 基本库说明
基本的开源第三方库 mysqlclient zlog crypto ssl curl freefare nfc jsoncpp rabbitmq

myCommon common/ 与上下游通信的包解析与组合

myDatabase databse/ 与数据库通信

myLog log/ 使用zlog做程序日志系统

myRingBuffer ringBuffer/ 环形buffer

myServer server_interface/ server_business/ 服务端

myClient client_interface/ 客户端

myRabbitmq rabbitmq_interface rabbitmq客户端

myOta ota/ on-the-air在线更新

config 配置文件格式string string 配置服务器端口，rabbitmq信息等

### 一次消息回环
@上游接收：上游启动rabbitmq client，监听到消息后，压入接收queue

@上游接收处理：主程序的接收处理线程，将rabbitmq的接收queue中消息取出;
解析操作，按指定的设备id，生产对应的设备操作信息
(服务器发送消息：client_fd根据设备id和client_info中extra_data对比而得);
压入服务器发送queque。

@下游发送：将服务器内的发送消息，按照与接入的client_fd进行发送。

@下游接收：服务器接收处理线程，将接收的设备回送信息，按照设备协议进行解析;
按照设备id，生成rabbitmq需要的设备回送消息，压入rabbitmq发送队列。

@上游发送：rabbitmq client将发送队列中的消息发送到rabbitmq server上。

主程序完成rabbimq接收解析，server接收解析。
myRabbitmq 完成rabbitmq client，接收，压入接收queue，无脑发送发送queue里的消息。
myServer 完成server接收，压入接收queue，无脑发送发送queue里的消息