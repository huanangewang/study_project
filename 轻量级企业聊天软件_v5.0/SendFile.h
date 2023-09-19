#pragma once

#include "BasicWindow.h"
#include "ui_SendFile.h"

//继承改成BasicWindow
class SendFile : public BasicWindow
{
	Q_OBJECT

public:
	SendFile(QWidget *parent = Q_NULLPTR);
	~SendFile();

	//TalkWindow -- > TalkWindowShell --> SendFile
signals://发送文件发射的信号(这里在TalkWindowShell里，更新我们要发送的tcp数据)
	void sendFileClicked(QString &strData, int &msgType, QString fileName);

private slots:
	void on_openBtn_clicked();	//打开文件按钮点击
	void on_sendBtn_clicked();	//发送按钮按钮点击
private:
	Ui::SendFile ui;
	QString m_filePath;			//选择的文件路径
};


//目前仅支持 ： *.txt *.doc