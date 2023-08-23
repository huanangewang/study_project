#pragma once

#include "basicwindow.h"
#include "ui_SkinWindow.h"

//body里面放的都是Label
class SkinWindow : public BasicWindow
{
	Q_OBJECT

public:
	SkinWindow(QWidget *parent = Q_NULLPTR);
	~SkinWindow();

public:
	void initControl();//初始化控件

public slots:
	//重写关闭按钮
	void onShowClose();

private:
	Ui::SkinWindow ui;
};
