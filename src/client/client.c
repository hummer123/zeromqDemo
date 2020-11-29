#include "demo_comm.h"


int main()
{
    int iRet = 0;
    int iReqNum = 0;
    char *pcData = NULL;

    printf("client start...\n");
    void *pvCtx = zmq_ctx_new();
    void *pvReq = zmq_socket(pvCtx, ZMQ_REQ);
    iRet = zmq_connect(pvReq, "tcp://localhost:5555");
    assert(0 == iRet);

    for (iReqNum = 0; iReqNum < 1; iReqNum++)
    {
        s_send(pvReq, "REQ send hello");
        
        pcData = s_recv(pvReq);
        free(pcData);
    }

    zmq_close(pvReq);
    zmq_ctx_destroy(pvCtx);

    return 0;
}

