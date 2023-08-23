#include "TcpServer.h"
#include <QTcpSocket>
#include "TcpSocket.h"

TcpServer::TcpServer(int port):m_port(port)
{

}

TcpServer::~TcpServer()
{}

//���ڷ���� ���� �˿ڣ���û�������ҷ�����
bool TcpServer::run()
{
	if (this->listen(QHostAddress::AnyIPv4, m_port))//Any��ָ�����κ�����������
	{
		qDebug() << QString::fromLocal8Bit("����˼����˿� %1 �ɹ�").arg(m_port);//�����ɹ�
		return true;
	}
	else
	{
		qDebug() << QString::fromLocal8Bit("����˼����˿� %1 ʧ��").arg(m_port);//����ʧ��
		return false;
	}
}


//�ͻ������µ�����ʱ�������
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << QString::fromLocal8Bit("�µ��������ˣ�") << socketDescriptor << endl;
	TcpSocket* tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);//����������
	tcpsocket->run();

	connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray&, int)),
		this, SLOT(SocketDataProcessing(QByteArray&, int)));//socket�Ǳ��������ˣ���������server��Ҫ�������� 

	connect(tcpsocket, SIGNAL(signalClientDisconnect(int)),
		this, SLOT(SocketDisconnected( int)));//socket�Ǳ߶Ͽ������ˣ�����Ҫ֪��˭����(ͨ��������)

	//socket��ӵ�������
	m_tcpSocketConnectList.append(tcpsocket);

}

//�Ͽ����Ӵ���
void TcpServer::SocketDisconnected(int descriptor)
{
	for (int i = 0;i < m_tcpSocketConnectList.length();++i)//������������˭Ҫ�Ͽ�����
	{
		QTcpSocket* item = m_tcpSocketConnectList.at(i);//��ȡÿһ��
		item->socketDescriptor();//��ȡ��������Ȼ���ٺʹ�����descriptor����ƥ��
		if (item->socketDescriptor() == descriptor || item->socketDescriptor() == -1)
		{
			//�Ͽ�����,�������Ƴ�,��������Դ����
			m_tcpSocketConnectList.removeAt(i);
			item->deleteLater();
			qDebug() << QString::fromLocal8Bit("tcpSocket�Ͽ�����") << descriptor;// fromLocal8Bit��ӱ��ر���ת��uico
			return;
		}
	}
}

//��������,����1������������  ����2����˭�����ݣ���������-->ȥ�����в�ѯ
void TcpServer::SocketDataProcessing(QByteArray& SendData, int descriptor)
{
	for (int i = 0;i < m_tcpSocketConnectList.length();++i)
	{
		QTcpSocket* item = m_tcpSocketConnectList.at(i);//��ȡÿһ��
		item->socketDescriptor();//��ȡ��������Ȼ���ٺʹ�����descriptor����ƥ��
		if (item->socketDescriptor() == descriptor)
		{
			qDebug() << QString::fromLocal8Bit("����IP:") << item->peerAddress().toString();//ͨ��QTcpSocket����ȡip��ַ
			qDebug() << QString::fromLocal8Bit("���������ݣ�") << QString(SendData);

			emit signalTcpMsgComes(SendData);//������Ϣ���˵��źţ�Ϊ�˵�ʱ��udp�㲥��ȥ
		}
	}
}