#ifndef  _THREAD_H
#define  _THREAD_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct ThreadPool ThreadPool;

//�����̳߳ز���ʼ��
ThreadPool *threadpool_creat(int min,int max,int queueCapcity);

//���̳߳��������,����2������ṹ���ڵĺ���ָ�룬����3���������� ����
void threadPoolAdd(ThreadPool* pool, void(*function) (void*), void* arg);
//��ȡ�̳߳��л��ŵ��̸߳���
int threadAliveNum(ThreadPool* pool);
//��ȡ�̳߳��й������̸߳���
int threadBusyNum(ThreadPool* pool);
//�̳߳�����
int threadDestroy(ThreadPool* pool);

////////////////////////�̳߳��ڲ�����////////////////////////
//��������
void* worker(void* arg);
//�����ߺ���
void* manager(void* arg);
//�߳��˳�����
void threadExit(ThreadPool* pool);



//typedef struct Task
//{
//	void (*function) (void* agr);
//	void* arg; //��Ϊ��֪��������ʲô����ʲô���ͣ�������ָ���һ��
//}Task;//����

#endif //  _THREAD_H
