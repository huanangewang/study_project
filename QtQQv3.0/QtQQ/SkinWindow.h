#pragma once

#include "basicwindow.h"
#include "ui_SkinWindow.h"

//body����ŵĶ���Label
class SkinWindow : public BasicWindow
{
	Q_OBJECT

public:
	SkinWindow(QWidget *parent = Q_NULLPTR);
	~SkinWindow();

public:
	void initControl();//��ʼ���ؼ�

public slots:
	//��д�رհ�ť
	void onShowClose();

private:
	Ui::SkinWindow ui;
};
