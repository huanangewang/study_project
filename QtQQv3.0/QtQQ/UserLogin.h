#pragma once

#include "BasicWindow.h"
#include "ui_UserLogin.h"

class UserLogin : public BasicWindow //基类修改BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();
	//void setLoginEmployeeID(const QString& UserID); //为了访问类内私有成员，并改变它的值
	//QString getLoginEmployeeID(); //获得QQ号


private slots:
	void onLoginBtnClicked();

private:
	void initControl();
	bool connectMysql(); //连接数据库
	bool veryfyAccountCode(bool &isAccountLogin,QString &strAccount);//验证账号密码,为了获得这两个值，我们要用引用方式传参

//private:
//	QString g_LoginEmployeeID;//登录者的QQ号(员工号)  将输入的账号进行赋值，方便后面使用

private:
	Ui::UserLogin ui;
};
