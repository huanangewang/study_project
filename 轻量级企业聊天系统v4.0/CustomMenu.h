#pragma once

#include <QMenu>
#include <QMap>

//��װ�õ��Զ���˵��࣬�������ⲿʵ�֣��˵������⶯����ӣ�Ŀǰ��ʵ�֣�
//��ʾ���ر�

class CustomMenu  : public QMenu
{
	Q_OBJECT

public:
	CustomMenu(QWidget *parent = nullptr);
	~CustomMenu();

public:
	//����Զ���˵�---��1.������ť�ı���2.�˵�ͼ�꣬3.�˵��ı�
	void addCustomMenu(const QString& text, const QString& icon,const QString& name );
	//ͨ���ı� ��ȡ����
	QAction* getAction(const QString& text); 

private:
	//ӳ�䶯��,����˵�����,�������洢�������֣�����
	QMap <QString, QAction* > m_menuActionMap;
};
