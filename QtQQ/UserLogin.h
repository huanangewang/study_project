#pragma once

#include "BasicWindow.h"
#include "ui_UserLogin.h"


class UserLogin : public BasicWindow //»ùÀàÐÞ¸ÄBasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();

private slots:
	void onLoginBtnClicked();

private:
	void initControl();

private:
	Ui::UserLogin ui;
};
