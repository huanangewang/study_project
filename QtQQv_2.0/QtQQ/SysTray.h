#pragma once

#include <QSystemTrayIcon>
#include <QWidget>

//���ܽ��ܣ�data:2023.08.06�����ϸ���
//Ϊ��ʵ��ϵͳ����--->��С��-->�����ܵ���������������û����--->
//����ʵ���Ҽ��˵��������˳�
//�Զ���˵�,��װ����һ����--->�Ա������Ӹ�����Զ���˵�~
class SysTray  : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget *parent);
	~SysTray();

public slots:
	//ϵͳ����ͼ�걻����---->�����Ǳ������ԭ��
	void onIconActiveted(QSystemTrayIcon::ActivationReason reason);

private:
	void initSystemTray(); //��ʼ��ϵͳ����
	void addSystrayMenu();//����ϵͳ���̲˵�

private:
	//���ﲻ��ֱ���� �����о�ʹ��QWidget,��Ϊ������С�����ٴε����ʱ��Ҫ��ԭ��֮ǰ��QWidget,Ҳ����CCMainWindow
	QWidget* m_partent;//�Ӹ�����ָ��
};
