#include "demo_comm.h"


#define BUFF_LEN    128

int main()
{
    char buffer[BUFF_LEN] = {0};
    int iMajor = 0;
    int iMinor = 0;
    int iPatch = 0; 
    int rc = 0;
    zmq_msg_t stPubMsg;

    zmq_version(&iMajor, &iMinor, &iPatch);
    printf("====> ZMQ Version: %d.%d.%d\n", iMajor, iMinor, iPatch);

    void *pvContext = zmq_ctx_new();
    void *pvPublisher = zmq_socket(pvContext, ZMQ_PUB);
    rc = zmq_bind(pvPublisher, "tcp://*:5555");
    assert(0 == rc);

    while(1)
    {
        zmq_msg_init_data(&stPubMsg, "A", strlen("A"), NULL, NULL);
        zmq_msg_send(&stPubMsg, pvPublisher, ZMQ_SNDMORE);
        zmq_msg_close(&stPubMsg);
        zmq_msg_init_data(&stPubMsg, "we don't want to see this", strlen("we don't want to see this"), NULL, NULL);
        zmq_msg_send(&stPubMsg, pvPublisher, 0);
        zmq_msg_close(&stPubMsg);


        zmq_msg_init_data(&stPubMsg, "B", strlen("B"), NULL, NULL);
        zmq_msg_send(&stPubMsg, pvPublisher, ZMQ_SNDMORE);
        zmq_msg_close(&stPubMsg);
        zmq_msg_init_data(&stPubMsg, "we would like to see this", strlen("we would like to see this"), NULL, NULL);
        zmq_msg_send(&stPubMsg, pvPublisher, 0);
        zmq_msg_close(&stPubMsg);

        usleep(1000 * 100);        
        printf("\n");
    }

    zmq_close(pvPublisher);
    zmq_ctx_destroy(pvContext);

    return 0;
}

