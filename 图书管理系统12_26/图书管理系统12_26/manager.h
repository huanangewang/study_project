#pragma once
#include <iostream>
using namespace std;
#include"Identity.h"
#include"globalFile.h"
#include"student.h"
#include"manager.h"
#include"bookdata.h"
#include<algorithm>

// ����Ա�����
class Manager :public Identity
{
public:

	//Ĭ�Ϲ���
	Manager();

	//�вι���
	Manager(string name, string pwd);

	//�Ӳ˵�����1  �û��˵�����
	virtual void operMenu();

	//�Ӳ˵�����2  ͼ�����˵�����
	virtual void operMenu1() ;

	////�Ӳ˵�����2 ͼ�����˵�����
	//void BookMenu();

	//��һ��С�˵����ܣ�
	//����û��˺�
	void addPerson();
	//ɾ���û��˺�
	void delectPerson();
	//�޸��û��˺�
	void modPerson();
	//�����û��˺�
	void findPerson();
	//��ʾ�û��˺�
	void showPerson();

	//�ڶ���С�˵����ܣ�
	//���ͼ��
	void addBook();
	//ɾ��ͼ��
	void delectBook();
	//�޸�ͼ��
	void modBook();
	//����ͼ��
	void findBook();
	//��ʾͼ��
	void showBook();



	//��ʼ��ѧ������
	void initVector();

	//��ʼ���鱾����
	void initVector1();

	//����ظ� ����1 ���ѧ��,�Է������ظ�ѧ�ŵ��˻�
	bool checkRepeat(int id);

	//����ظ� ����1 ���ѧ��,�Է������ظ��鼮��ŵ��鱾
	bool checkRepeat1(int B_id);

	//ѧ������
	vector<Student>vStu;

	//ͼ������
	vector<BookData>vBook;

};