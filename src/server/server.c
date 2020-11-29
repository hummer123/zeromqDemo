#include "demo_comm.h"
#include "pthread.h"

void *server_rep(void *args)
{
    char *pcData = NULL;
    void *pvContext = zmq_ctx_new();
    void *pvRep = zmq_socket(pvContext, ZMQ_REP);
    zmq_connect(pvRep, "ipc://backed.ipc");

    while(1)
    {
        pcData = s_recv(pvRep);
        printf("=========> recv: %s\n", pcData);
        free(pcData);

        s_send(pvRep, "Hello world");
        sleep(1);
    }

    zmq_close(pvRep);
    zmq_ctx_destroy(pvContext);
    return NULL;
}


int main()
{
    int iRet   = 0;
    int iMajor = 0;
    int iMinor = 0;
    int iPatch = 0;
    int isMore = 0;
    zmq_msg_t stMsg;
    pthread_t stPthread;

    zmq_version(&iMajor, &iMinor, &iPatch);
    printf("====> ZMQ Version: %d.%d.%d\n", iMajor, iMinor, iPatch);

    void *pvContext = zmq_ctx_new();
    void *pvDealer = zmq_socket(pvContext, ZMQ_DEALER);
    void *pvRouter = zmq_socket(pvContext, ZMQ_ROUTER);
    iRet = zmq_bind(pvDealer, "ipc://backed.ipc");
    assert(0 == iRet);
    iRet = zmq_bind(pvRouter, "tcp://*:5555");
    assert(0 == iRet);

    zmq_pollitem_t items[] = {{pvDealer, 0, ZMQ_POLLIN, 0}, 
                              {pvRouter, 0, ZMQ_POLLIN, 0}};

    memset(&stPthread, 0, sizeof(stPthread));
    pthread_create(&stPthread, NULL, server_rep, NULL);

    while(1)
    {
        iRet = zmq_poll(items, 2, -1);
        if (-1 == iRet)
        {
            break;
        }

        if (items[0].revents & ZMQ_POLLIN)
        {
            printf("------- DEALER 2 ROUTER -------\n");
            s_forward(pvDealer, pvRouter);
        }

        if (items[1].revents & ZMQ_POLLIN)
        {
            printf("------- ROUTER 2 DEALER -------\n");
            s_forward(pvRouter, pvDealer);
        }
    }
    
    zmq_close(pvRouter);
    zmq_close(pvDealer);
    zmq_ctx_destroy(pvContext);

    return 0;
}


