#pragma once

#include <QObject>

class NotifyManager : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();

signals:
	//Ƥ���ı���ź�
	void signalSkinChanged(const QColor& color);

public:
	static NotifyManager* getInstance();//��ò����ľ�̬ ʵ��
	void notifyOtherWindowChangeSkin(const QColor& color);//֪ͨ�������ڸı���ɫ

private:
	static NotifyManager* instance;//�����ľ�̬ ʵ��
};
