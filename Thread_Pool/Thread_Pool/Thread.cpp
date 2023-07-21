#include "Thread.h"
#include <pthread.h> 
#define THREAD_NUM 2

//����ṹ��
typedef struct Task
{
	void (*function) (void* agr);
	void *arg; //��Ϊ��֪��������ʲô����ʲô���ͣ�������ָ���һ��
}Task;//����

//�̳߳ؽṹ��
struct ThreadPool
{
	//�������
	Task* taskQ;
	//����
	int queueCapcity;
	//��ǰ�������
	int queueSize;
	//��ͷ--->ȡ����
	int queueFront;
	//��β--->������
	int queueRear;

	//�������߳�ID
	pthread_t managerID;
	//�������߳�ID,�ж������ָ��ָ��һ������ĵ�ַ
	pthread_t * workerID;
	//��С���߳���
	int minNum;
	//����߳���
	int maxNum;
	//��ǰæ���߳���,�ɵ�������
	int busyNum;
	//�����̸߳���
	int liveNum;
	//ɱ���̵߳ĸ���
	int killNum;
	//Ϊ���߳�ͬ�����ӻ�������1.�������̳߳� 2.��busynum 
	//3.�ж���������ǲ������ˣ�4.��������ǲ��ǿ���
	pthread_mutex_t mutePool;
	pthread_mutex_t muteBusy;
	//�������� 
	pthread_cond_t notFull; 
	pthread_cond_t notEmpty;
	//�Ƿ�Ҫ�����̳߳� ,����Ϊ1
	int shutdown;
};

//��ʼ��
ThreadPool* threadpool_creat(int min, int max, int queueCapcity)
{
	ThreadPool* pool = (ThreadPool*) malloc(sizeof(ThreadPool));
	do {
		if (pool == NULL)
		{
			printf("�̳߳ش���ʧ��...\n");
			break;
		}
		//������������ѿռ䣨�����飬��ָ��ָ��
		pool->workerID = (pthread_t*)malloc(sizeof(pthread_t) * max);
		if (pool->workerID == NULL)
		{
			printf("�̵߳Ĺ���ID����ʧ��...\n");
			break;
		}
		memset(pool->workerID, 0, sizeof(pthread_t) * max);//��0
		pool->minNum = min;
		pool->maxNum = max;
		pool->busyNum = 0;
		pool->liveNum = min;    // ����С�������
		pool->killNum = 0;

		if (pthread_mutex_init(&pool->mutePool, NULL) != 0 ||
			pthread_mutex_init(&pool->muteBusy, NULL) != 0 ||
			pthread_cond_init(&pool->notEmpty, NULL) != 0 ||
			pthread_cond_init(&pool->notFull, NULL) != 0)
		{
			printf("��ʼ��ʧ��...\n");
			break;
		}

		// �������
		pool->taskQ = (Task*)malloc(sizeof(Task) * queueCapcity);
		pool->queueCapcity = queueCapcity;
		pool->queueSize = 0;
		pool->queueFront = 0;
		pool->queueRear = 0;

		pool->shutdown = 0;

		// �����߳�,����3���������̵߳�������
		pthread_create(&pool->managerID, NULL , manager , pool);
		//���������̡߳�����С������
		for (int i = 0;i < min;++i)
		{
			pthread_create(&pool->workerID[i], NULL, worker, pool);
		}
		//û���⣬����poolָ��
		return pool;
	} while (0);


	//�����쳣����Դ�ͷ�
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


//�����̵߳ĺ���
void* worker(void* arg)
{
	//�Ѵ������̳߳صĶ�������ת��
	ThreadPool* pool = (ThreadPool*)arg;

	//��ͣ���������
	while (1)
	{
		//�����̳߳�Ҫ����
		pthread_mutex_lock(&pool->mutePool);
		//û�رգ���û�¸ɵ��߳�
		while (pool->queueSize == 0 &&!pool->shutdown)
		{
			//���������߳�,��Ϊ�գ���ô�ͽ��������̻߳���,����1����������
			pthread_cond_wait(&pool->notEmpty,&pool->mutePool);
			//�������߻���
			if (pool->killNum > 0)
			{
				pool->killNum--; 
				if (pool->liveNum > pool->minNum)
				{
					pool->liveNum--;
					//��Ϊwait�����pool->mutePool������Ȩ,����Ҫ����
					pthread_mutex_unlock(&pool->mutePool);
					threadExit(pool);
				}
			}

		}
		//���ѣ������ж��̳߳��Ƿ񱻹ر�
		if (pool->shutdown)
		{
			pthread_mutex_unlock(&pool->mutePool);
			threadExit(pool);
		}
		//ȡ����---ȡ�����ͺ���  �� ��ַ----�൱������
		Task task;
		task.function = pool->taskQ[pool->queueFront].function;
		task.arg = pool->taskQ[pool->queueFront].arg;
		//�ƶ�ͷ,ѭ�����еĸ���
		//�磺����Ϊ6����ʱͷ��5��(5+1)%6==0
		pool->queueFront = (pool->queueFront + 1) % pool->queueCapcity;
		pool->queueSize--;
		//�������߻���
		pthread_cond_signal(&pool->notFull);

		//����,����2���������ĵ�ַ
		pthread_mutex_unlock(&pool->mutePool);

		//ִ������
		printf("�߳� %ld ��ʼ����...\n",pthread_self());
		pthread_mutex_lock(&pool->muteBusy);
		pool->busyNum++;
		pthread_mutex_unlock(&pool->muteBusy);

		task.function(task.arg);
		free(task.arg);
		task.arg = NULL;

		printf("�߳� %ld ��������...\n", pthread_self());
		pthread_mutex_lock(&pool->muteBusy);
		pool->busyNum--;
		pthread_mutex_unlock(&pool->muteBusy);

	}

	return NULL;
}

//�����ߺ���
void* manager(void* arg)
{
	//���������Ĳ���ת�����̳߳ؽṹ������
	ThreadPool* pool = (ThreadPool*) arg;
	while (!pool->shutdown)
	{
		//ÿ��һ��ʱ���⣬�Ƿ�Ҫ���٣������Ƿ�Ҫ����߳�
		sleep(3);
		//ȡ�̳߳���������������ǰ�߳�����
		pthread_mutex_lock(&pool->mutePool);
		int queueSize = pool->queueSize;
		int liveNum = pool->liveNum;
		pthread_mutex_unlock(&pool->mutePool);

		//ȡ��æ���߳�����
		pthread_mutex_lock(&pool->muteBusy);
		int busyNum = pool->busyNum;
		pthread_mutex_unlock(&pool->muteBusy);

		//��ӣ���� �������>�����߳��� && ����̸߳���<����߳���
		if (queueSize > liveNum && liveNum < pool->maxNum)
		{
			int counter = 0;//�ӵ�2֮��Ͳ������
			pthread_mutex_lock(&pool->mutePool);
			for (int i = 0;i < pool->maxNum && counter<THREAD_NUM && liveNum<pool->maxNum ;++i)
			{
				if (pool->workerID[i] == 0)
				{
					//�̴߳���
					pthread_create(&pool->workerID[i], NULL, worker, pool);
					counter++;

					pool->liveNum++;

				}
			}
			pthread_mutex_unlock(&pool->mutePool);
		}

		//���٣����æµ���߳���*2<����߳��� && ����߳�>��С�߳���
		if (pool->busyNum * 2 < liveNum && liveNum > pool->minNum)
		{
			pthread_mutex_lock(&pool->mutePool);
			pool->killNum = THREAD_NUM;
			pthread_mutex_unlock(&pool->mutePool);
			//�ù����߳���ɱ��֪ͨ�������߳�
			for (int i = 0;i < THREAD_NUM;++i)
			{
				//����û�¸ɵĹ����߳�
				pthread_cond_signal(&pool->notEmpty);
			}
		}
	}

	return nullptr;
}

//�߳��˳�
void threadExit(ThreadPool* pool)
{
	//��ȡ��ǰ�̵߳��̺߳�
	pthread_t id = pthread_self();
	for (int i = 0;i < pool->maxNum;++i)
	{
		if (pool->workerID[i] == id)
		{
			pool->workerID[i] = 0;
			printf("�̣߳�%d �Ѿ��˳��� .....\n", id);
			break;
		}
	}
	pthread_exit(NULL); 
}

//������ӵ��̳߳��е����������
void threadPoolAdd(ThreadPool* pool, void(*function)(void*), void* arg)
{
	pthread_mutex_lock(&pool->mutePool);
	//�����������
	while (pool->queueSize == pool->queueCapcity && !pool->shutdown)
	{
		//����
		pthread_cond_wait(&pool->notFull, &pool->mutePool);
	}
	if (pool->shutdown)
	{
		pthread_mutex_unlock(&pool->mutePool);
		return;
	}
	//�������,��β���-------�൱������
	pool->taskQ[pool->queueRear].function = function;
	pool->taskQ[pool->queueRear].arg = arg;
	//�ƶ���β
	pool->queueRear = (pool->queueRear + 1) % pool->queueCapcity;
	pool->queueSize++;
	//�������������������ϵĹ����߳�
	pthread_cond_signal(&pool->notEmpty);
	pthread_mutex_unlock(&pool->mutePool);

}

//��ȡ���ŵ��߳���
int threadAliveNum(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->mutePool);
	int AilveNum = pool->liveNum;
	pthread_mutex_unlock(&pool->mutePool);
	return AilveNum;
}


//��ȡ�������߳���
int threadBusyNum(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->muteBusy);
	int busyNum = pool->busyNum;
	pthread_mutex_unlock(&pool->muteBusy);
	return busyNum;
}

//�����̳߳�
int threadDestroy(ThreadPool* pool)
{
	if (pool == NULL) //��Ч�ڴ�
	{
		return -1;
	}
	//�ر��̳߳�
	pool->shutdown = 1;
	//�������չ������߳�
	pthread_join(pool->managerID,NULL);
	//�����������������̣߳������̣߳�
	for (int i = 0;i < pool->liveNum;++i)
	{
		pthread_cond_signal(&pool->notEmpty);
	}


	//�ͷŶ��ڴ�
	if (pool->taskQ)
	{
		free(pool->taskQ);
	}
	if (pool->workerID)
	{
		free(pool->workerID);
	}

	//���ٸ��ֻ�����
	pthread_mutex_destroy(&pool->muteBusy);
	pthread_mutex_destroy(&pool->muteBusy);
	pthread_cond_destroy(&pool->notEmpty);
	pthread_cond_destroy(&pool->notFull);

	free(pool);
	pool = NULL;

	return 0;   
}

