#pragma once

#include <QTcpServer>

class TcpServer  : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(int port);
	~TcpServer();

signals:
	void signalTcpMsgComes(QByteArray&);//携带信息进行发送 信号

public:
	bool run();//用于服务端监听端口，有没有连接我服务器

protected:
	void incomingConnection(qintptr socketDescriptor);//客户端有新的连接时，则调用(处理新的连接)

private slots:
	void SocketDataProcessing(QByteArray& SendData,int descriptor);//处理数据,参数1：发来的数据  参数2：是谁的数据（描述符）-->去链表中查询
	void SocketDisconnected(int descriptor); //断开连接处理

private:
	int m_port;//端口号
	QList <QTcpSocket*> m_tcpSocketConnectList;//所有的连接都在链表中维护

};
