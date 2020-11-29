#include "demo_comm.h"
#include <pthread.h>

static void *worker_task_a(void *args)
{
    int iTotal = 0;
    char *pcData = NULL;
    char szBuf[32] = {0};

    void *pvContext = zmq_ctx_new();
    void *worker = zmq_socket(pvContext, ZMQ_REQ);
    zmq_setsockopt(worker, ZMQ_IDENTITY, "A", 1);
    zmq_connect(worker, "ipc://routing.ipc");

    while(1)
    {
        memset(szBuf, 0, sizeof(szBuf));
        snprintf(szBuf, sizeof(szBuf), "worker A: [%d]", iTotal);
        printf("===> %s\n", szBuf);
        s_send(worker, szBuf);
        iTotal++;

        pcData = s_recv(worker);
        printf("=======> Recv: %s\n", pcData);
        free(pcData);
        pcData = NULL;

        usleep(500 * 1000);
    }

    zmq_close(worker);
    zmq_ctx_destroy(pvContext);
    return NULL;
}

static void *worker_task_b(void *args)
{
    int iTotal = 0;
    char *pcData = NULL;
    char szBuf[32] = {0};

    void *pvContext = zmq_ctx_new();
    void *worker = zmq_socket(pvContext, ZMQ_REQ);
    zmq_setsockopt(worker, ZMQ_IDENTITY, "B", 1);
    zmq_connect(worker, "ipc://routing.ipc");

    while(1)
    {
        memset(szBuf, 0, sizeof(szBuf));
        snprintf(szBuf, sizeof(szBuf), "worker B: [%d]", iTotal);
        printf("###> %s\n", szBuf);
        s_send(worker, szBuf);
        iTotal++;

        pcData = s_recv(worker);
        printf("######> Recv: %s\n", pcData);
        free(pcData);
        pcData = NULL;

        sleep(1);
    }

    zmq_close(worker);
    zmq_ctx_destroy(pvContext);
    return NULL;
}



int main()
{
    pthread_t worker;
    // int iTaskNum = 0;
    char *pcAddr = NULL;
    char *pcEmpty = NULL;
    char *pcData = NULL;

    void *pvContext = zmq_ctx_new();
    void *client = zmq_socket(pvContext, ZMQ_ROUTER);
    zmq_bind(client, "ipc://routing.ipc");

    pthread_create(&worker, NULL, &worker_task_a, NULL);
    pthread_create(&worker, NULL, &worker_task_b, NULL);
    sleep(1);

    while(1)
    {
        pcAddr  = s_recv(client);
        pcEmpty = s_recv(client);
        pcData  = s_recv(client);
        printf("************> ROUTE Recv: %s\n", pcData);

        s_sendmore(client, pcAddr);
        s_sendmore(client, "");
        s_send(client, pcData);

        sleep(1);

        free(pcAddr);
        free(pcEmpty);
        free(pcData);
        pcAddr = NULL;
        pcEmpty = NULL;
        pcData = NULL;        
    }

    zmq_close(client);
    zmq_ctx_destroy(pvContext);
    return 0;
}
