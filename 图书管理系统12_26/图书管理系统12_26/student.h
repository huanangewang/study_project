#pragma once
#include <iostream>
using namespace std;
#include"Identity.h"
#include"globalFile.h"
#include"bookdata.h"
#include<vector>

//ѧ����
class Student : public Identity
{
public:
	
	//Ĭ�Ϲ���
	Student();
	//�вι���  ������ѧ�š�����������
	Student(int id, string name, string pwd);

	//�˵�����
	virtual void operMenu();

	//�Ӳ˵�����2  ͼ�����˵�����
	virtual void operMenu1() ;

	//����
	void returnBook();

	//����ͼ��  //Ҳ���ǽ���
	void findBook();
	//��ʾͼ��
	void showBook();

    //��ʼ���鱾���� ��ȡ�������ļ��� �鱾
	void initVector1();

	//ѧ��ѧ��
	int m_Id;

	//ͼ�������
	vector<BookData> vBook;

};