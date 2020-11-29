#ifndef __DEMO_COMM_H__
#define __DEMO_COMM_H__

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define MSG_BUFF_LEN    256
#define RESULT_ASSERT(x) \
            do  \
            {   \
                if (!(x)) \
                {   \
                    printf("===> Error file[%s], line[%d]\n", __FILE__, __LINE__); \
                    return -1;  \
                }   \
            } while (0)


void s_dump(void *pvSocket)
{
    int i = 0;
    int iRet = 0;
    int iSize = 0;
    int isText = 1;
    char *pcData = NULL;
    zmq_msg_t stMsg;

    iRet = zmq_msg_init(&stMsg);
    assert(0 == iRet);

    printf("\n<------------------------------------------>\n");
    do
    {
        iSize = zmq_msg_recv(&stMsg, pvSocket, 0);
        assert(0 <= iSize);

        pcData = (char *)zmq_msg_data(&stMsg);
        assert(NULL != pcData);

        isText = 1;
        for (i = 0; i < iSize; i++)
        {
            if ((32 > (unsigned char)pcData[i]) || (126 < (unsigned char)pcData[i]))
            {
                isText = 0;
                break;
            }
        }

        printf("=== [%03d]: ", iSize);
        for(i = 0; i < iSize; i++)
        {
            if (isText)
            {
                printf("%c", pcData[i]);
            }
            else
            {
                printf("%02X", (unsigned char)pcData[i]);
            }
        }
        printf("\n");
        
    } while (zmq_msg_more(&stMsg));
    
    iRet = zmq_msg_close(&stMsg);
    assert(0 == iRet);

    return;
}

/* ×Ö·û´®¸ñÊ½»¯ */
void s_printf(zmq_msg_t *pstMsg, int iLen)
{
    int i = 0;
    int isText = 1;
    char *pcData = (char *)zmq_msg_data(pstMsg);

    for (i = 0; i < iLen; i++)
    {
        if ((32 > (unsigned char)pcData[i]) || (126 < (unsigned char)pcData[i]))
        {
            isText = 0;
            break;
        }
    }

    printf("=== [%03d]: ", iLen);
    for(i = 0; i < iLen; i++)
    {
        if (isText)
        {
            printf("%c", pcData[i]);
        }
        else
        {
            printf("%02X", (unsigned char)pcData[i]);
        }
    }
    printf("\n");
}

void s_forward(void *pcSocketA, void *pcSocketB)
{
    int isMore   = 0;
    int iRecvLen = 0;
    zmq_msg_t stMsg;

    do
    {
        zmq_msg_init(&stMsg);
        iRecvLen = zmq_msg_recv(&stMsg, pcSocketA, 0);
        s_printf(&stMsg, iRecvLen);
        isMore = zmq_msg_more(&stMsg);
        zmq_msg_send(&stMsg, pcSocketB, (isMore ? ZMQ_SNDMORE : 0));
        if (!isMore)
        {
            break;
        }
    } while (1);
    
    return;
}


char *s_recv(void *pSocket)
{
    int iRecvLen = 0;
    zmq_msg_t stMsg;
    char buffer[MSG_BUFF_LEN] = {0};

    zmq_msg_init(&stMsg);
    iRecvLen = zmq_msg_recv(&stMsg, pSocket, 0);
    if (-1 == iRecvLen)
    {
        return NULL;
    }

    memcpy(buffer, zmq_msg_data(&stMsg), MSG_BUFF_LEN - 1);
    buffer[(iRecvLen < MSG_BUFF_LEN)?iRecvLen:(MSG_BUFF_LEN - 1)] = '\0';

    return strdup(buffer);
}

int s_send(void *socket, char *string) 
{
    zmq_msg_t stMsg;
    zmq_msg_init_data(&stMsg, string, strlen (string), NULL, 0);
    int size = zmq_msg_send (&stMsg, socket, 0);
    zmq_msg_close(&stMsg);

    return size;
}

int s_sendmore(void *socket, char *string) 
{
    zmq_msg_t stMsg;
    zmq_msg_init_data(&stMsg, string, strlen (string), NULL, 0);
    int size = zmq_msg_send (&stMsg, socket, ZMQ_SNDMORE);
    zmq_msg_close(&stMsg);

    return size;
}



#endif  /* __DEMO_COMM_H__ */

