#pragma once

#include <QTcpSocket>

class TcpSocket  : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket();
	~TcpSocket();
	
	void run();

signals:
	//目的是告诉server，有数据要处理
	void signalGetDataFromClient(QByteArray&, int);//从客户端收到数据,我需要知道他的数据是什么，也要知道他的标识
	void signalClientDisconnect(int); //告诉server 有客户端断开连接了


private slots:
	void onReceiveData();//处理客户端发来的数据 readyRead信号的读取
	void onClientDisconnect(); //客户端断开连接

private:
	int m_descriptor;//用来作为 唯一标识 描述符

};
