#include "TcpServer.h"
#include <QTcpSocket>
#include "TcpSocket.h"

TcpServer::TcpServer(int port):m_port(port)
{

}

TcpServer::~TcpServer()
{}

//用于服务端 监听 端口，有没有连接我服务器
bool TcpServer::run()
{
	if (this->listen(QHostAddress::AnyIPv4, m_port))//Any是指监听任何主机的连接
	{
		qDebug() << QString::fromLocal8Bit("服务端监听端口 %1 成功").arg(m_port);//监听成功
		return true;
	}
	else
	{
		qDebug() << QString::fromLocal8Bit("服务端监听端口 %1 失败").arg(m_port);//监听失败
		return false;
	}
}


//客户端有新的连接时，则调用
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << QString::fromLocal8Bit("新的连接来了：") << socketDescriptor << endl;
	TcpSocket* tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);//设置描述符
	tcpsocket->run();

	connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray&, int)),
		this, SLOT(SocketDataProcessing(QByteArray&, int)));//socket那边来数据了，我们这里server就要处理数据 

	connect(tcpsocket, SIGNAL(signalClientDisconnect(int)),
		this, SLOT(SocketDisconnected( int)));//socket那边断开连接了，我们要知道谁断了(通过描述符)

	//socket添加到链表中
	m_tcpSocketConnectList.append(tcpsocket);

}

//断开连接处理
void TcpServer::SocketDisconnected(int descriptor)
{
	for (int i = 0;i < m_tcpSocketConnectList.length();++i)//遍历看看，是谁要断开连接
	{
		QTcpSocket* item = m_tcpSocketConnectList.at(i);//获取每一项
		item->socketDescriptor();//获取描述符，然后再和传来的descriptor进行匹配
		if (item->socketDescriptor() == descriptor || item->socketDescriptor() == -1)
		{
			//断开连接,链表中移除,并进行资源回收
			m_tcpSocketConnectList.removeAt(i);
			item->deleteLater();
			qDebug() << QString::fromLocal8Bit("tcpSocket断开连接") << descriptor;// fromLocal8Bit会从本地编码转成uico
			return;
		}
	}
}

//处理数据,参数1：发来的数据  参数2：是谁的数据（描述符）-->去链表中查询
void TcpServer::SocketDataProcessing(QByteArray& SendData, int descriptor)
{
	for (int i = 0;i < m_tcpSocketConnectList.length();++i)
	{
		QTcpSocket* item = m_tcpSocketConnectList.at(i);//获取每一项
		item->socketDescriptor();//获取描述符，然后再和传来的descriptor进行匹配
		if (item->socketDescriptor() == descriptor)
		{
			qDebug() << QString::fromLocal8Bit("来自IP:") << item->peerAddress().toString();//通过QTcpSocket，获取ip地址
			qDebug() << QString::fromLocal8Bit("发来的数据：") << QString(SendData);

			emit signalTcpMsgComes(SendData);//发出消息来了的信号，为了到时候udp广播出去
		}
	}
}