#pragma once
#include <iostream>
using namespace std;
#include"bookdata.h"
#include<vector>

//ֻ��Ҫ����.hͷ�ļ����ɣ��޾���ʵ��
//��ݳ������
class Identity
{
public:
	//�Ӳ˵����麯��
//���������д�����еĴ��麯������������Ҳ�ǳ����࣬�޷�ʵ��������
	virtual void operMenu() = 0;


	//�Ӳ˵�����2  ͼ�����˵�����
	virtual void operMenu1()=0;

	string m_Name;
	//�û���
	string m_Pwd;
	//����
};