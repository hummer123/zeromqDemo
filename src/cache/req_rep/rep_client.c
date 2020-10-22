#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "demo_comm.h"

#define BUFF_LEN    1024

int main()
{
    int iRet = 0;
    int  request_nbr = 0;
    char buffer[BUFF_LEN] = {0};
    zmq_msg_t stReqMsg;
    zmq_msg_t stRepMsg;

    printf("Connecting to hello world server...\n");
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5578");

    for (request_nbr = 0; request_nbr != 3; request_nbr++)
    {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "Client A: %d", request_nbr);
        iRet = zmq_msg_init_data(&stReqMsg, buffer, strlen(buffer), NULL, NULL);
        RESULT_ASSERT(0 == iRet);

        printf("===> Send: [%s]\n", (char *)zmq_msg_data(&stReqMsg));
        iRet = zmq_msg_send(&stReqMsg, requester, 0);
        RESULT_ASSERT(-1 != iRet);
        zmq_msg_close(&stReqMsg);

        iRet = zmq_msg_init(&stRepMsg);
        RESULT_ASSERT(0 == iRet);
		iRet = zmq_msg_recv(&stRepMsg, requester, 0);
        RESULT_ASSERT(-1 != iRet);
        printf("======> Received: [%s]\n", (char *)zmq_msg_data(&stRepMsg));
        zmq_msg_close(&stRepMsg);

        printf("\n");
    }

    zmq_close(requester);
    zmq_ctx_destroy(context);

    return 0;
}

