# zeroMQdemo

zeroMQ demo 包含libzmq 和 czmq

**支持的绑定关系 : **

- PUB - SUB
- REQ - REP
- REQ - ROUTER
- DEALER - REP
- DEALER - ROUTER
- DEALER - DEALER
- ROUTER - ROUTER
- PUSH - PULL
- PAIR - PAIR



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
> **REQ套接字在发送消息时会向头部添加一个空帧， 接收时又会自动移除空帧**
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
>2. 若SUB socket为自己声明了标识，则，当SUB断开连接时，PUB会保留要发送给SUB的消息。所以此时需要为PUB指定缓存阈值 ZMQ_HWM；或者PUB将消息缓存在磁盘上ZMQ_SWAP --- HWM 与 SWAP可同时生效。
>3. PUB当没有SUB与之相连时会丢弃发送发送出去的数据。
>4. 
>
>

----

### 五、关于多帧消息的发送与接收：

>**发送：**
>
>> ```
>> zmq_msg_send(&msg, socket, ZMQ_SNDMORE);
>> ...
>> zmq_msg_send(&msg, socket, ZMQ_SNDMORE);
>> ...
>> zmq_msg_send(&msg, socket, 0);	// 最后一帧
>> ```
>
>**接收（单帧or多帧均适用）：**
>
>> ```
>> int imore = 0;
>> size_t moreSize = sizeof(imore);
>> while (1)
>> {
>> 	zmq_msg_t msg;
>> 	zmq_msg_init(&msg);
>> 	zmq_msg_recv(&msg, socket, 0);
>> 	...
>> 	zmq_msg_close(&msg);
>> 	
>> 	#if 1
>> 	if (!zmq_msg_more(&msg))
>> 	{
>> 		break;
>> 	}
>> 	#else
>> 	zmq_getsocketopt(socket, ZMQ_RCVMORE, &imore, &moreSize);
>> 	if (!imore)
>> 	{
>> 		break;
>> 	}
>> 	#endif
>> 	
>> }
>> ```
>>
>> 



### 六、内置转发代理：

>ZMQ 三种内置转发代理功能：
>
>* QUEUE             --- 可用于 **请求-应答** 代理服务；
>* FORWARDER   --- 可用于 **发布-订阅** 代理服务；
>* STREAMER       --- 可用于 **管道模式** 代理服务;
>
>
>
>

### 七、ROUTE

> * **从ROUTER中读取一条消息时，ØMQ会包上一层信封，上面注明了消息的来源。**
> *  **向ROUTER写入一条消息时（包含信封），ØMQ会将信封拆开，并将消息递送给相应的对象。**
>
> DEVICE
>
> > ROUTE --- ROUTE   ===  LRU 模式
> >
> > ROUTE --- DEALER  === **DEALER 自带负载均衡**
> >
> > REQ --- ROUTE --- DEALER --- REP
> >
> > ROUTE 经典模式下是与 REQ 对接



### 八、信封：

> * **REQ套接字在发送消息时会向头部添加一个空帧，接收时又会自动移除。**
> * **从ROUTER中读取一条消息时，ØMQ会包上一层信封，上面注明了消息的来源。**
> *  **向ROUTER写入一条消息时（包含信封），ØMQ会将信封拆开，并将消息递送给相应的对象。**
> * ***ROUTER会在所有收到的消息前添加消息来源的地址。***

### 九、其他：

> zmq_msg_init_data(&stMsg, buff, strlen(buff), free, 0);	
>
> zmq_msg_init_data 为零拷贝技术，即内部使用指针指向buff。
>
> eg.
>
> ~~~
> //client：
> recvmore(worker, addr);
> recvmore(worker, "");
> recv(worker, buff);
> printf("===> %s\n", buff);  
> 
> //worker
> sendmore(client, addr)
> sendmore(client, "");
> zmq_msg_init_data(&stMsg, buff, strlen(buff), NULL, 0);
> send(client, &stMsg)
> free(buff)
> ----- 
> 1. send 发送返回时数据并不一定发送到 client；
> 2. sned 返回后立刻 free(buff), 则 client 中打印出来的可能乱码或无数据
> ~~~
>
> 

