#pragma once

#include "BasicWindow.h"
#include "ui_ReceiveFile.h"

//修改基类为：BasicWindow
class ReceiveFile : public BasicWindow
{
	Q_OBJECT

public:
	ReceiveFile(QWidget *parent = Q_NULLPTR);
	~ReceiveFile();
	void setMsg(QString &msgLabel);//设置信息（什么信息，根据传入的字符串进行设置）

signals:
	void refuseFile();//拒绝接受文件

	//槽
private slots:
	void on_cancelBtn_clicked();//取消
	void on_okBtn_clicked();//确定

private:
	Ui::ReceiveFile ui;
};
