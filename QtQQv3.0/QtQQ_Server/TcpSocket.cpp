#include "TcpSocket.h"

TcpSocket::TcpSocket()
{}

TcpSocket::~TcpSocket()
{}

void TcpSocket::run()
{
	m_descriptor = this->socketDescriptor();//设置描述符
	connect(this, SIGNAL(readyRead()), this, SLOT(onReceiveData()));//当有客户端数据来了，就执行槽函数onReceiveData
	connect(this, SIGNAL(disconnected()), this, SLOT(onClientDisconnect()));//当有客户端断开了，就执行槽函数onClientDisconnect
}


//处理客户端发来的数据 其实readyRead信号的读取
void TcpSocket::onReceiveData()
{
	//将数据拿出来，
	QByteArray buffer = this->readAll();
	if (!buffer.isEmpty())
	{
		QString strData = QString::fromLocal8Bit(buffer);
	}
	//发射接收到了客户端数据的信号
	emit signalGetDataFromClient(buffer, m_descriptor);
}

//客户端断开连接
void TcpSocket::onClientDisconnect()
{
	emit signalClientDisconnect(m_descriptor); //到时候server 在链表中进行比对，就知道是谁断开连接了
}


