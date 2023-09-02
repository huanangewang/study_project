#pragma once

#include <QWidget>
#include "ui_ContactItem.h"

//ʵ���б��У����ڵ�㿪����ʾͷ���û���������ǩ��--->��UI
class ContactItem : public QWidget
{
	Q_OBJECT

public:
	ContactItem(QWidget *parent = Q_NULLPTR);
	~ContactItem();

	void setUserName(const QString& userName);//�����û���
	void setSignName(const QString& signName);//���ø���ǩ��
	void setHeadPixmap(const QPixmap& headPath);//����ͷ��
	QString getUserName()const; //�����û� ���ַ�������  С֪ʶ�㣺----����Ӹ�const����Ա���������Ͳ��ܶԳ�Ա���ݽ����޸�
	QSize getHeadLabelSize()const;//��Ҫ��������Բͷ��

private:
	void initControl();//��ʼ���ؼ�

private:
	Ui::ContactItem ui;
};
