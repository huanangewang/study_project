#pragma once

#include "BasicWindow.h"
#include "ui_UserLogin.h"

class UserLogin : public BasicWindow //�����޸�BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();
	//void setLoginEmployeeID(const QString& UserID); //Ϊ�˷�������˽�г�Ա�����ı�����ֵ
	//QString getLoginEmployeeID(); //���QQ��


private slots:
	void onLoginBtnClicked();

private:
	void initControl();
	bool connectMysql(); //�������ݿ�
	bool veryfyAccountCode(bool &isAccountLogin,QString &strAccount);//��֤�˺�����,Ϊ�˻��������ֵ������Ҫ�����÷�ʽ����

//private:
//	QString g_LoginEmployeeID;//��¼�ߵ�QQ��(Ա����)  ��������˺Ž��и�ֵ���������ʹ��

private:
	Ui::UserLogin ui;
};
