#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUFF_LEN    10

int main()
{
    int  request_nbr = 0;
    char buffer[BUFF_LEN] = {0};

    printf("Connecting to hello world server...\n");
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5555");

    for (request_nbr = 0; request_nbr != 10; request_nbr++)
    {
        printf("Sending Hello %d...\n", request_nbr);
        zmq_send(requester, "Hello", 5, 0);
        memset(buffer, 0, sizeof(buffer));
		zmq_recv(requester, buffer, sizeof(buffer), 0);
        printf("Received %s %d...\n", buffer, request_nbr);
    }

    zmq_close(requester);
    zmq_ctx_destroy(context);

    return 0;
}

