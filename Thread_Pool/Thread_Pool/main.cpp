#include <cstdio>
#include "Thread.h"
#include <pthread.h> 

//任务函数
void taskFunc(void* arg)
{
    int num = *(int*)arg;
    printf("线程工作的 id： %ld  数据为：= %d\n",
        pthread_self(), num);
    sleep(1);
}

int main()
{
    // 创建线程池
    ThreadPool* pool = threadpool_creat(3, 10, 100);
    for (int i = 0; i < 100; ++i)
    {
        int* num = (int*)malloc(sizeof(int));
        *num = i + 100;
        threadPoolAdd(pool, taskFunc, num);
    }

    sleep(30);

    threadDestroy(pool);
    return 0;
}