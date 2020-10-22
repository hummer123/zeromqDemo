#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFF_LEN    128

int main()
{
    char buffer[BUFF_LEN] = {0};
    int iMajor = 0;
    int iMinor = 0;
    int iPatch = 0; 
    int rc = 0;
    zmq_msg_t stReqMsg;
    zmq_msg_t stRepMsg;

    zmq_version(&iMajor, &iMinor, &iPatch);
    printf("====> ZMQ Version: %d.%d.%d\n", iMajor, iMinor, iPatch);

    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    rc = zmq_bind(responder, "tcp://*:5555");
    assert(0 == rc);

    rc = zmq_bind(responder, "tcp://*:5578");
    assert(0 == rc);

    while(1)
    {
        zmq_msg_init(&stReqMsg);
        zmq_msg_recv(&stReqMsg, responder, 0);
        printf("===> Received: [%s]\n", (char *)zmq_msg_data(&stReqMsg));

        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "Replay: %s", (char *)zmq_msg_data(&stReqMsg));
        zmq_msg_close(&stReqMsg);
        
        sleep(5);
        printf("======> send:[%s]\n", buffer);
        zmq_msg_init_data(&stRepMsg, (void *)buffer, strlen(buffer), NULL, NULL);
        zmq_msg_send(&stRepMsg, responder, 0);
        zmq_msg_close(&stRepMsg);

        printf("\n");
    }
    
    zmq_close(responder);
    zmq_ctx_destroy(context);

    return 0;
}

