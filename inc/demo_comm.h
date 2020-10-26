#ifndef __DEMO_COMM_H__
#define __DEMO_COMM_H__

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>

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

        for (i = 0; i < iSize; i++)
        {
            if ((32 > (unsigned char)pcData[i]) || (126 < (unsigned char)pcData[i]))
            {
                isText = 0;
            }
        }

        printf("===== recv size[%03d]\n", iSize);
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




#endif  /* __DEMO_COMM_H__ */

