#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFF_LEN    10

int main()
{
    char buffer[BUFF_LEN] = {0};

    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, "tcp://*:5555");
    assert(0 == rc);

    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        zmq_recv(responder, buffer, sizeof(buffer), 0);
        printf("===> Received: [%s]\n", buffer);

        sleep(1);
        zmq_send(responder, "World", 5, 0);
    }
    
    zmq_close(responder);
    zmq_ctx_destroy(context);

    return 0;
}

