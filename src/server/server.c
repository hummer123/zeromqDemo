#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define BUFF_LEN    128

int main()
{
    char buffer[BUFF_LEN] = {0};
    int iMajor = 0;
    int iMinor = 0;
    int iPatch = 0; 
    int iRet = 0;
    int zipcode = 0;
    int temperature = 0;
    int relhumidity = 0;

    zmq_version(&iMajor, &iMinor, &iPatch);
    printf("====> ZMQ Version: %d.%d.%d\n", iMajor, iMinor, iPatch);

    void *context = zmq_ctx_new();
    void *stPublisher = zmq_socket(context, ZMQ_PUB);
    iRet = zmq_bind(stPublisher, "tcp://*:5556");
    assert(0 == iRet);
    //iRet = zmq_bind(stPublisher, "ipc://weather.ipc");
    //assert(0 == iRet);

    srand((unsigned)time(NULL));
    while(1)
    {
        zipcode     = rand() % 10 + 10080;
        temperature = rand() % 200;
        relhumidity = rand() % 50;

        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%05d %d %d", zipcode, temperature, relhumidity);
        zmq_send(stPublisher, buffer, strlen(buffer), 0);
        printf("===> send[%s]\n", buffer);

        usleep(1000 * 100);
    }
    zmq_close(stPublisher);
    zmq_ctx_destroy(context);

    return 0;
}

