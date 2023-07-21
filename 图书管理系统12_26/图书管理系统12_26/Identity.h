#pragma once
#include <iostream>
using namespace std;
#include"bookdata.h"
#include<vector>

//只需要创建.h头文件即可，无具体实现
//身份抽象基类
class Identity
{
public:
	//子菜单纯虚函数
//子类必须重写父类中的纯虚函数，否则子类也是抽象类，无法实例化对象
	virtual void operMenu() = 0;


	//子菜单界面2  图书管理菜单界面
	virtual void operMenu1()=0;

	string m_Name;
	//用户名
	string m_Pwd;
	//密码
};