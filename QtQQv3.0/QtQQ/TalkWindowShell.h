#pragma once

#include "basicwindow.h"
#include "ui_TalkWindowShell.h"
#include <qmap.h>
#include <QTcpSocket>
#include <QUdpSocket>

class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

const int gtcpPort = 8888;

class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

public:
	//�������-->���촰��  ,����2 �����ں���һ������
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString& uid);
	
	//���õ�ǰ����Ĵ���(���ĸ����ڽ�������)
	void setCurrentWidget(QWidget* widget);

	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap()const;//Ϊ�����ⲿ�������˽�г�Ա

private:
	void initControl();		//��ʼ���ؼ�
	void initTcpSocket();	//��ʼ��TCP
	void initUdpSocket();	//��ʼ��UDP
	void getEmployeesID(QStringList& employeesList);	//��ȡ����Ա��QQ��
	bool createJSFile(QStringList &employeesList);		//������дJS�ļ���
	void handleReceivedMsg(int senderEmployeeID, int msgType, QString strMsg);//�����յ�����Ϣ��������ҳ��

	//�������Ĳۺ������
public slots:
	void onEmotionBtnClicked(bool);//���鰴ť�����ִ�еĲۺ���

	//�ͻ��˷���Tcp���ݣ����ݣ��������ͣ��ļ���
	void updateSendTcpMsg(QString& strData, int &msgType, QString fileName = "");
private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);//��ʵ������ߵ��б���
	void onEmotionItemClicked(int emotionNum);	//ѡ���ĸ�����
	void onProcessPendingData();//���������udp�㲥�յ�������

private:
	Ui::TalkWindowClass ui;
	//�� ֵ��  �򿪵����촰�ڵ�ӳ��(��ߵ��б��ʹ򿪵����촰����һ��ӳ�䣬��Ӧ��ϵ)
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;
	EmotionWindow* m_emotionWindow;	//���鴰��

private:
	QTcpSocket *m_tcpClientSocket;	//Tcp�ͻ���
	QUdpSocket *m_udpReceiver;		//udp���ն�
};
