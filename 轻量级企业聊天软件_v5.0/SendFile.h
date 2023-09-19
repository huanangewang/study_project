#pragma once

#include "BasicWindow.h"
#include "ui_SendFile.h"

//�̳иĳ�BasicWindow
class SendFile : public BasicWindow
{
	Q_OBJECT

public:
	SendFile(QWidget *parent = Q_NULLPTR);
	~SendFile();

	//TalkWindow -- > TalkWindowShell --> SendFile
signals://�����ļ�������ź�(������TalkWindowShell���������Ҫ���͵�tcp����)
	void sendFileClicked(QString &strData, int &msgType, QString fileName);

private slots:
	void on_openBtn_clicked();	//���ļ���ť���
	void on_sendBtn_clicked();	//���Ͱ�ť��ť���
private:
	Ui::SendFile ui;
	QString m_filePath;			//ѡ����ļ�·��
};


//Ŀǰ��֧�� �� *.txt *.doc