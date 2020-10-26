#include "demo_comm.h"

#define BUFF_LEN    1024

int main()
{
    int iRet = 0;
    char buffer[BUFF_LEN] = {0};
    zmq_msg_t stSubAddr;
    zmq_msg_t stSubMsg;

    printf("Connecting to PUB...\n");
    void *pvContext = zmq_ctx_new();
    void *pvSubscriber = zmq_socket(pvContext, ZMQ_SUB);
    zmq_connect(pvSubscriber, "tcp://localhost:5555");
    zmq_setsockopt(pvSubscriber, ZMQ_SUBSCRIBE, "B", strlen("B"));

    for (; ;)
    {
        zmq_msg_init(&stSubAddr);
        zmq_msg_recv(&stSubAddr, pvSubscriber, 0);
        printf("===> Recv[%s][%lu]\n", (char *)zmq_msg_data(&stSubAddr), zmq_msg_size(&stSubAddr));

        if (zmq_msg_more(&stSubAddr))
        {
            printf("===> have more msg\n");
            zmq_msg_init(&stSubMsg);
            zmq_msg_recv(&stSubMsg, pvSubscriber, 0);
        }
        printf("===> addr[%s][%lu] - [%s]\n", 
                    (char *)zmq_msg_data(&stSubAddr),
                    zmq_msg_size(&stSubAddr), 
                    (char *)zmq_msg_data(&stSubMsg));

        zmq_msg_close(&stSubMsg);
        zmq_msg_close(&stSubAddr);

        printf("\n");
    }

    zmq_close(pvSubscriber);
    zmq_ctx_destroy(pvContext);

    return 0;
}

