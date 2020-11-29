#include "demo_comm.h"

int main()
{
    zmq_msg_t stMsg;
    void *pvContext = zmq_ctx_new();

    void *sink = zmq_socket(pvContext, ZMQ_ROUTER);
    zmq_bind(sink, "inproc://example.inp");

    void *annoymous = zmq_socket(pvContext, ZMQ_REQ);
    zmq_connect(annoymous, "inproc://example.inp");

    zmq_msg_init_data(&stMsg, "ROUTER uses a generated UUID", 
                        strlen("ROUTER uses a generated UUID"), NULL, NULL);
    zmq_msg_send(&stMsg, annoymous, 0);
    s_dump(sink);
    zmq_msg_close(&stMsg);

    void *indentfied = zmq_socket(pvContext, ZMQ_REQ);
    zmq_setsockopt(indentfied, ZMQ_IDENTITY, "HELLO", strlen("HELLO"));
    zmq_connect(indentfied, "inproc://example.inp");
    
    zmq_msg_init_data(&stMsg, "ROUTER socket uses REQ's socket identity", 
                        strlen("ROUTER socket uses REQ's socket identity"), NULL, NULL);
    zmq_msg_send(&stMsg, indentfied, 0);
    s_dump(sink);
    zmq_msg_close(&stMsg);

    zmq_close(sink);
    zmq_close(annoymous);
    zmq_close(indentfied);
    zmq_ctx_destroy(pvContext);

    return 0;
}
