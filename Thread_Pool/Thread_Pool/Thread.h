#ifndef  _THREAD_H
#define  _THREAD_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct ThreadPool ThreadPool;

//创建线程池并初始化
ThreadPool *threadpool_creat(int min,int max,int queueCapcity);

//给线程池添加任务,参数2：任务结构体内的函数指针，参数3：任务函数的 参数
void threadPoolAdd(ThreadPool* pool, void(*function) (void*), void* arg);
//获取线程池中活着的线程个数
int threadAliveNum(ThreadPool* pool);
//获取线程池中工作的线程个数
int threadBusyNum(ThreadPool* pool);
//线程池销毁
int threadDestroy(ThreadPool* pool);

////////////////////////线程池内部调用////////////////////////
//工作函数
void* worker(void* arg);
//管理者函数
void* manager(void* arg);
//线程退出函数
void threadExit(ThreadPool* pool);



//typedef struct Task
//{
//	void (*function) (void* agr);
//	void* arg; //因为不知道传入是什么任务，什么类型，用智能指针好一点
//}Task;//别名

#endif //  _THREAD_H
