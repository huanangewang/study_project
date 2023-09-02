#pragma once

#include <QWidget>
#include "TalkWindowShell.h"
#include "ui_TalkWindow.h"

class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent,const QString& uid/*,GroupType groupType*/);//����1�����࣬����2����������֤��id������3�������������
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);//��ӱ���ͼƬ
	void setWindowName(const QString& name);//���ô�������
	QString getTalkId(); //Ϊ�˷���˽������ m_talkid

private:
	void initControl();//��ʼ���ؼ�
	void initGroupTalkStatus();//��ʼ��Ⱥ��״̬
	int getComDepID(); //��ȡ��˾��id��

	void addPeopInfo(QTreeWidgetItem* pRootGroupItem,int employeeID);//���Ա����Ϣ

	//�ۺ���
private slots:
	void onFileOpenBtnClicked(bool);//�ļ��򿪰�ť
	void onSendBtnClicked(bool);//������Ϣ
	void onItemDoubleClicked(QTreeWidgetItem* item,int column);//���촰���е�˫���Ҳദ��

private:
	//void initCompanyTalk();	//��ʼ����˾Ⱥ����
	//void initPersonTalk();	//��ʼ�����²�����
	//void initMarketTalk();	//��ʼ���г�������
	//void initDevelopTalk();	//��ʼ���з�������


	void initPtoPTalk();	//��ʼ����������
	void initTalkWindow();	//��ʼ��Ⱥ��

private:
	bool m_isGroupTalk;//�Ƿ�ΪȺ��
	Ui::TalkWindow ui;
	QString m_talkId;
	//GroupType m_groupType;
	QMap <QTreeWidgetItem*, QString> m_groupPerpleMap;//���з�����ϵ������������ӳ��

	//�Ӹ����ѣ�Ϊ��TalkWindowShell�����Է���TalkWindow��˽������
	friend class TalkWindowShell;
};
