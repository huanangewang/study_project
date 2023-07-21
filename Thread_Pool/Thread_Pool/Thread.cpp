#include "Thread.h"
#include <pthread.h> 
#define THREAD_NUM 2

//任务结构体
typedef struct Task
{
	void (*function) (void* agr);
	void *arg; //因为不知道传入是什么任务，什么类型，用智能指针好一点
}Task;//别名

//线程池结构体
struct ThreadPool
{
	//任务队列
	Task* taskQ;
	//容量
	int queueCapcity;
	//当前任务个数
	int queueSize;
	//队头--->取数据
	int queueFront;
	//队尾--->放数据
	int queueRear;

	//管理者线程ID
	pthread_t managerID;
	//工作的线程ID,有多个，用指针指向一个数组的地址
	pthread_t * workerID;
	//最小的线程数
	int minNum;
	//最大线程数
	int maxNum;
	//当前忙的线程数,可单独加锁
	int busyNum;
	//存活的线程个数
	int liveNum;
	//杀死线程的个数
	int killNum;
	//为了线程同步，加互斥锁，1.锁整个线程池 2.锁busynum 
	//3.判断任务队列是不是满了，4.任务队列是不是空了
	pthread_mutex_t mutePool;
	pthread_mutex_t muteBusy;
	//条件变量 
	pthread_cond_t notFull; 
	pthread_cond_t notEmpty;
	//是否要销毁线程池 ,销毁为1
	int shutdown;
};

//初始化
ThreadPool* threadpool_creat(int min, int max, int queueCapcity)
{
	ThreadPool* pool = (ThreadPool*) malloc(sizeof(ThreadPool));
	do {
		if (pool == NULL)
		{
			printf("线程池创建失败...\n");
			break;
		}
		//创建多个连续堆空间（是数组，用指针指向）
		pool->workerID = (pthread_t*)malloc(sizeof(pthread_t) * max);
		if (pool->workerID == NULL)
		{
			printf("线程的工作ID创建失败...\n");
			break;
		}
		memset(pool->workerID, 0, sizeof(pthread_t) * max);//清0
		pool->minNum = min;
		pool->maxNum = max;
		pool->busyNum = 0;
		pool->liveNum = min;    // 和最小个数相等
		pool->killNum = 0;

		if (pthread_mutex_init(&pool->mutePool, NULL) != 0 ||
			pthread_mutex_init(&pool->muteBusy, NULL) != 0 ||
			pthread_cond_init(&pool->notEmpty, NULL) != 0 ||
			pthread_cond_init(&pool->notFull, NULL) != 0)
		{
			printf("初始化失败...\n");
			break;
		}

		// 任务队列
		pool->taskQ = (Task*)malloc(sizeof(Task) * queueCapcity);
		pool->queueCapcity = queueCapcity;
		pool->queueSize = 0;
		pool->queueFront = 0;
		pool->queueRear = 0;

		pool->shutdown = 0;

		// 创建线程,参数3：管理者线程的任务函数
		pthread_create(&pool->managerID, NULL , manager , pool);
		//创建工作线程。按最小数创建
		for (int i = 0;i < min;++i)
		{
			pthread_create(&pool->workerID[i], NULL, worker, pool);
		}
		//没问题，返回pool指针
		return pool;
	} while (0);


	//出现异常，资源释放
	if (pool)
	{
		free(pool);
	}
	if (pool->workerID)
	{
		free(pool->workerID);
	}
	if (pool->taskQ)
	{
		free(pool->taskQ);
	}

	return NULL;
}


//工作线程的函数
void* worker(void* arg)
{
	//把传来的线程池的对象，类型转换
	ThreadPool* pool = (ThreadPool*)arg;

	//不停读任务队列
	while (1)
	{
		//访问线程池要加锁
		pthread_mutex_lock(&pool->mutePool);
		//没关闭，又没事干的线程
		while (pool->queueSize == 0 &&!pool->shutdown)
		{
			//阻塞工作线程,不为空，那么就将阻塞的线程唤醒,参数1：条件变量
			pthread_cond_wait(&pool->notEmpty,&pool->mutePool);
			//被管理者唤醒
			if (pool->killNum > 0)
			{
				pool->killNum--; 
				if (pool->liveNum > pool->minNum)
				{
					pool->liveNum--;
					//因为wait获得了pool->mutePool的所有权,这里要解锁
					pthread_mutex_unlock(&pool->mutePool);
					threadExit(pool);
				}
			}

		}
		//唤醒，首先判断线程池是否被关闭
		if (pool->shutdown)
		{
			pthread_mutex_unlock(&pool->mutePool);
			threadExit(pool);
		}
		//取任务---取参数和函数  的 地址----相当于消费
		Task task;
		task.function = pool->taskQ[pool->queueFront].function;
		task.arg = pool->taskQ[pool->queueFront].arg;
		//移动头,循环队列的概念
		//如：容量为6，此时头是5，(5+1)%6==0
		pool->queueFront = (pool->queueFront + 1) % pool->queueCapcity;
		pool->queueSize--;
		//将生产者唤醒
		pthread_cond_signal(&pool->notFull);

		//解锁,参数2，互斥锁的地址
		pthread_mutex_unlock(&pool->mutePool);

		//执行任务
		printf("线程 %ld 开始工作...\n",pthread_self());
		pthread_mutex_lock(&pool->muteBusy);
		pool->busyNum++;
		pthread_mutex_unlock(&pool->muteBusy);

		task.function(task.arg);
		free(task.arg);
		task.arg = NULL;

		printf("线程 %ld 结束工作...\n", pthread_self());
		pthread_mutex_lock(&pool->muteBusy);
		pool->busyNum--;
		pthread_mutex_unlock(&pool->muteBusy);

	}

	return NULL;
}

//管理者函数
void* manager(void* arg)
{
	//将传进来的参数转换成线程池结构体类型
	ThreadPool* pool = (ThreadPool*) arg;
	while (!pool->shutdown)
	{
		//每隔一段时间检测，是否要销毁，或者是否要添加线程
		sleep(3);
		//取线程池中任务数量，当前线程数量
		pthread_mutex_lock(&pool->mutePool);
		int queueSize = pool->queueSize;
		int liveNum = pool->liveNum;
		pthread_mutex_unlock(&pool->mutePool);

		//取出忙的线程数量
		pthread_mutex_lock(&pool->muteBusy);
		int busyNum = pool->busyNum;
		pthread_mutex_unlock(&pool->muteBusy);

		//添加：如果 任务个数>存活的线程数 && 存活线程个数<最大线程数
		if (queueSize > liveNum && liveNum < pool->maxNum)
		{
			int counter = 0;//加到2之后就不添加了
			pthread_mutex_lock(&pool->mutePool);
			for (int i = 0;i < pool->maxNum && counter<THREAD_NUM && liveNum<pool->maxNum ;++i)
			{
				if (pool->workerID[i] == 0)
				{
					//线程创建
					pthread_create(&pool->workerID[i], NULL, worker, pool);
					counter++;

					pool->liveNum++;

				}
			}
			pthread_mutex_unlock(&pool->mutePool);
		}

		//销毁：如果忙碌的线程数*2<存活线程数 && 存活线程>最小线程数
		if (pool->busyNum * 2 < liveNum && liveNum > pool->minNum)
		{
			pthread_mutex_lock(&pool->mutePool);
			pool->killNum = THREAD_NUM;
			pthread_mutex_unlock(&pool->mutePool);
			//让工作线程自杀，通知阻塞的线程
			for (int i = 0;i < THREAD_NUM;++i)
			{
				//唤醒没事干的工作线程
				pthread_cond_signal(&pool->notEmpty);
			}
		}
	}

	return nullptr;
}

//线程退出
void threadExit(ThreadPool* pool)
{
	//获取当前线程的线程号
	pthread_t id = pthread_self();
	for (int i = 0;i < pool->maxNum;++i)
	{
		if (pool->workerID[i] == id)
		{
			pool->workerID[i] = 0;
			printf("线程：%d 已经退出了 .....\n", id);
			break;
		}
	}
	pthread_exit(NULL); 
}

//任务添加到线程池中的任务队列中
void threadPoolAdd(ThreadPool* pool, void(*function)(void*), void* arg)
{
	pthread_mutex_lock(&pool->mutePool);
	//任务队列满了
	while (pool->queueSize == pool->queueCapcity && !pool->shutdown)
	{
		//阻塞
		pthread_cond_wait(&pool->notFull, &pool->mutePool);
	}
	if (pool->shutdown)
	{
		pthread_mutex_unlock(&pool->mutePool);
		return;
	}
	//添加任务,队尾添加-------相当于生产
	pool->taskQ[pool->queueRear].function = function;
	pool->taskQ[pool->queueRear].arg = arg;
	//移动队尾
	pool->queueRear = (pool->queueRear + 1) % pool->queueCapcity;
	pool->queueSize++;
	//唤醒阻塞在条件变量上的工作线程
	pthread_cond_signal(&pool->notEmpty);
	pthread_mutex_unlock(&pool->mutePool);

}

//获取活着的线程数
int threadAliveNum(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->mutePool);
	int AilveNum = pool->liveNum;
	pthread_mutex_unlock(&pool->mutePool);
	return AilveNum;
}


//获取工作的线程数
int threadBusyNum(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->muteBusy);
	int busyNum = pool->busyNum;
	pthread_mutex_unlock(&pool->muteBusy);
	return busyNum;
}

//销毁线程池
int threadDestroy(ThreadPool* pool)
{
	if (pool == NULL) //无效内存
	{
		return -1;
	}
	//关闭线程池
	pool->shutdown = 1;
	//阻塞回收管理者线程
	pthread_join(pool->managerID,NULL);
	//唤醒阻塞的消费者线程（工作线程）
	for (int i = 0;i < pool->liveNum;++i)
	{
		pthread_cond_signal(&pool->notEmpty);
	}


	//释放堆内存
	if (pool->taskQ)
	{
		free(pool->taskQ);
	}
	if (pool->workerID)
	{
		free(pool->workerID);
	}

	//销毁各种互斥锁
	pthread_mutex_destroy(&pool->muteBusy);
	pthread_mutex_destroy(&pool->muteBusy);
	pthread_cond_destroy(&pool->notEmpty);
	pthread_cond_destroy(&pool->notFull);

	free(pool);
	pool = NULL;

	return 0;   
}

