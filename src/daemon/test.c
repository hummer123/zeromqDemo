#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main
{
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    zmq_bind(responder, "tcp://*:555");

    while(1)
    {
        zmq_msg_t request;
        zmq_msg_init(&request);
        zmq_recv(responder, &request, 0);
        printf("recv: %s\n", request.data);
        zmq_msg_close(&request);

        sleep(1);

        zmq_msg_t reply;
        zmq_msg_init_size(&reply, 5);
        memcpy(zmq_msg_data(&reply), "world", 5);
        zmq_send(responder, &reply, 0);
        zmq_msg_close(&reply);
    }

    zmq_close(&responder);
    zmq_ctx_destroy(context);
}
