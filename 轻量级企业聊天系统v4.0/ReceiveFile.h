#pragma once

#include "BasicWindow.h"
#include "ui_ReceiveFile.h"

//�޸Ļ���Ϊ��BasicWindow
class ReceiveFile : public BasicWindow
{
	Q_OBJECT

public:
	ReceiveFile(QWidget *parent = Q_NULLPTR);
	~ReceiveFile();
	void setMsg(QString &msgLabel);//������Ϣ��ʲô��Ϣ�����ݴ�����ַ����������ã�

signals:
	void refuseFile();//�ܾ������ļ�

	//��
private slots:
	void on_cancelBtn_clicked();//ȡ��
	void on_okBtn_clicked();//ȷ��

private:
	Ui::ReceiveFile ui;
};
