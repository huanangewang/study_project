#pragma once
#include <iostream>
using namespace std;
#include"Identity.h"
#include"globalFile.h"
#include"student.h"
#include"manager.h"
#include"bookdata.h"
#include<algorithm>

// 管理员类设计
class Manager :public Identity
{
public:

	//默认构造
	Manager();

	//有参构造
	Manager(string name, string pwd);

	//子菜单界面1  用户菜单界面
	virtual void operMenu();

	//子菜单界面2  图书管理菜单界面
	virtual void operMenu1() ;

	////子菜单界面2 图书管理菜单界面
	//void BookMenu();

	//第一个小菜单功能：
	//添加用户账号
	void addPerson();
	//删除用户账号
	void delectPerson();
	//修改用户账号
	void modPerson();
	//查找用户账号
	void findPerson();
	//显示用户账号
	void showPerson();

	//第二个小菜单功能：
	//添加图书
	void addBook();
	//删除图书
	void delectBook();
	//修改图书
	void modBook();
	//查找图书
	void findBook();
	//显示图书
	void showBook();



	//初始化学生容器
	void initVector();

	//初始化书本容器
	void initVector1();

	//检测重复 参数1 检测学号,以防加入重复学号的账户
	bool checkRepeat(int id);

	//检测重复 参数1 检测学号,以防加入重复书籍编号的书本
	bool checkRepeat1(int B_id);

	//学生容器
	vector<Student>vStu;

	//图书容器
	vector<BookData>vBook;

};