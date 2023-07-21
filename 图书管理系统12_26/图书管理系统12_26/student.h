#pragma once
#include <iostream>
using namespace std;
#include"Identity.h"
#include"globalFile.h"
#include"bookdata.h"
#include<vector>

//学生类
class Student : public Identity
{
public:
	
	//默认构造
	Student();
	//有参构造  参数：学号、姓名、密码
	Student(int id, string name, string pwd);

	//菜单界面
	virtual void operMenu();

	//子菜单界面2  图书管理菜单界面
	virtual void operMenu1() ;

	//还书
	void returnBook();

	//查找图书  //也就是借书
	void findBook();
	//显示图书
	void showBook();

    //初始化书本容器 获取到所有文件中 书本
	void initVector1();

	//学生学号
	int m_Id;

	//图书库容器
	vector<BookData> vBook;

};