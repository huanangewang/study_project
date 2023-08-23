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
	//添加聊天-->聊天窗口  ,参数2 ：你在和哪一项聊天
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString& uid);
	
	//设置当前聊天的窗口(和哪个窗口进行聊天)
	void setCurrentWidget(QWidget* widget);

	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap()const;//为了在外部访问这个私有成员

private:
	void initControl();		//初始化控件
	void initTcpSocket();	//初始化TCP
	void initUdpSocket();	//初始化UDP
	void getEmployeesID(QStringList& employeesList);	//获取所有员工QQ号
	bool createJSFile(QStringList &employeesList);		//创建（写JS文件）
	void handleReceivedMsg(int senderEmployeeID, int msgType, QString strMsg);//处理收到的信息，整到网页上

	//聊天界面的槽函数设计
public slots:
	void onEmotionBtnClicked(bool);//表情按钮点击后执行的槽函数

	//客户端发送Tcp数据（数据，数据类型，文件）
	void updateSendTcpMsg(QString& strData, int &msgType, QString fileName = "");
private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);//其实就是左边的列表部件
	void onEmotionItemClicked(int emotionNum);	//选了哪个表情
	void onProcessPendingData();//处理服务器udp广播收到的数据

private:
	Ui::TalkWindowClass ui;
	//键 值对  打开的聊天窗口的映射(左边的列表，和打开的聊天窗口做一个映射，对应关系)
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;
	EmotionWindow* m_emotionWindow;	//表情窗口

private:
	QTcpSocket *m_tcpClientSocket;	//Tcp客户端
	QUdpSocket *m_udpReceiver;		//udp接收端
};
