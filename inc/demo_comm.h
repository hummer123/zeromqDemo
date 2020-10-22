#ifndef __DEMO_COMM_H__
#define __DEMO_COMM_H__

#define RESULT_ASSERT(x) \
            do  \
            {   \
                if (!(x)) \
                {   \
                    printf("===> Error file[%s], line[%d]\n", __FILE__, __LINE__); \
                    return -1;  \
                }   \
            } while (0)


#endif  /* __DEMO_COMM_H__ */

