#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "demo_comm.h"

#define BUFF_LEN    1024

int main(int argc, char *argv[])
{
    int iRet = 0;
    int  total_temp = 0;
    int  update_nbr = 0;
    char buffer[BUFF_LEN] = {0};
    int zipcode = 0;
    int temperature = 0;
    int relhumidity = 0;
    char *pcFilter = (argc > 1) ? argv[1] : "10086";

    printf("Connecting to PUB...\n");
    void *context = zmq_ctx_new();
    void *stSubscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(stSubscriber, "tcp://localhost:5556");

    /* 设置订阅信息 */
    zmq_setsockopt(stSubscriber, ZMQ_SUBSCRIBE, pcFilter, strlen(pcFilter));
    for (update_nbr = 0; update_nbr != 100; update_nbr++)
    {
        printf("===> recv msg\n");
        memset(buffer, 0, sizeof(buffer));
        iRet = zmq_recv(stSubscriber, buffer, sizeof(buffer), 0);
        RESULT_ASSERT(-1 != iRet);
        printf("===> buffer[%s]\n", buffer);

        iRet = sscanf(buffer, "%d %d %d", &zipcode, &temperature, &relhumidity);
        RESULT_ASSERT(3 == iRet);
        total_temp += temperature;
    }
    printf("===> zipCode['%s'], temp[%d]\n", pcFilter, total_temp / update_nbr);

    zmq_close(stSubscriber);
    zmq_ctx_destroy(context);
    return 0;
}

