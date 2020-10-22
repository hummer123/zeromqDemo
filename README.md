# zeroMQdemo

zeroMQ demo 包含libzmq 和 czmq



### 一、支持的协议：

* 线程间  --- inproc
* 进程间  --- ipc
* TCP
* 广播

### 二、支持的模式：

* Request-Reply  请求应答模式
* Publish-Subscribe   发布订阅模式
* Pipeline     管道模式
* Exclusive   独立对模式

### 三、Request-Reply 请求应答模式

> zmq_socket --- **ZMQ_REQ，ZMQ_REP**
>
> 1. server 绑定两个不同的端口（ZMQ_REP）
>
>    ​      --- recv msg  透传  send msg
>
> 2. client A connect 其中一个端口（ZMQ_REQ）
>
>    ​      --- send msg A  和 recv msg
>
> 3. client B connect 其中一个端口（ZMQ_REQ）
>
>    ​      --- send msg B 和 recv msg
>
> ----
>
> **问题一：client A 发送的msg A 概率性收到的却是 msg B?**
>
> ​            --- 那么Req-Rep模式下server如何保证消息的可靠性传输-收到client A的消息应答给 client A 而不是答给了client B？
>
> ​            --- zmq如何确认一个连接的对端ip及port？
>
> ----
>
> **问题二： xxx**
>
> d
>
> 
>
> 

### 四、Publish-Subscribe 发布订阅模式

>zmq_socket --- ZMQ_PUB、ZMQ_SUB
>
>**注意点：**
>
>1. 订阅端 ZMQ_SUB 必须使用zmq_setsockopt()方法来设置订阅的内容，如果不设置那么什么消息也收不到。
>
>d
>
>