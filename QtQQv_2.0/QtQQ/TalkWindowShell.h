#pragma once

#include "basicwindow.h"
#include "ui_TalkWindowShell.h"
#include<qmap.h>

class TalkWindowClass;
class TalkWindow;
class TalkWindowItem;//�����Щitem,����ɶȺ��ɶ��
class EmotionWindow;

//ö��
enum GroupType {
	COMPANY = 0,	//��˾Ⱥ
	PERSONELGROUP,	//���²�
	DEVELOPMENTGROUP,//�з���
	MARKETGROUP,	//�г���
	PTOP			//ͬ��֮�䵥������
};

//�޸Ļ��� BasicWindow
class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = nullptr);
	~TalkWindowShell();

	//�������ӿ�
public:
	void addTalkWindow(TalkWindow* talkWindow,TalkWindowItem* talkWindowItem,const QString& uid/*, GroupType grouptype*/);//�������-->���촰��  ,����2 �����ں���һ������
	void setCurrentWidget(QWidget* widget); //���õ�ǰ����Ĵ���(���ĸ����ڽ�������)
	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap() const; //Ϊ�����ⲿ�������˽�г�Ա

private:
	void initControl();//��ʼ���ؼ�

	//�������Ĳۺ������
public slots:
	void onEmotionBtnClicked(bool);//���鰴ť�����

private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);//��ʵ������ߵ��б���
	void onEmotionItemClicked(int emotionNum);//ѡ���ĸ�����

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;//�� ֵ��  �򿪵����촰�ڵ�ӳ��(��ߵ��б��ʹ򿪵����촰����һ��ӳ�䣬��Ӧ��ϵ)
	EmotionWindow* m_emotionWindow;//���鴰��

};
