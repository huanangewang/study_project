#include "TcpSocket.h"

TcpSocket::TcpSocket()
{}

TcpSocket::~TcpSocket()
{}

void TcpSocket::run()
{
	m_descriptor = this->socketDescriptor();//����������
	connect(this, SIGNAL(readyRead()), this, SLOT(onReceiveData()));//���пͻ����������ˣ���ִ�вۺ���onReceiveData
	connect(this, SIGNAL(disconnected()), this, SLOT(onClientDisconnect()));//���пͻ��˶Ͽ��ˣ���ִ�вۺ���onClientDisconnect
}


//����ͻ��˷��������� ��ʵreadyRead�źŵĶ�ȡ
void TcpSocket::onReceiveData()
{
	//�������ó�����
	QByteArray buffer = this->readAll();
	if (!buffer.isEmpty())
	{
		QString strData = QString::fromLocal8Bit(buffer);
	}
	//������յ��˿ͻ������ݵ��ź�
	emit signalGetDataFromClient(buffer, m_descriptor);
}

//�ͻ��˶Ͽ�����
void TcpSocket::onClientDisconnect()
{
	emit signalClientDisconnect(m_descriptor); //��ʱ��server �������н��бȶԣ���֪����˭�Ͽ�������
}


