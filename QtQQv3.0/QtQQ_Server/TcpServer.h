#pragma once

#include <QTcpServer>

class TcpServer  : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(int port);
	~TcpServer();

signals:
	void signalTcpMsgComes(QByteArray&);//Я����Ϣ���з��� �ź�

public:
	bool run();//���ڷ���˼����˿ڣ���û�������ҷ�����

protected:
	void incomingConnection(qintptr socketDescriptor);//�ͻ������µ�����ʱ�������(�����µ�����)

private slots:
	void SocketDataProcessing(QByteArray& SendData,int descriptor);//��������,����1������������  ����2����˭�����ݣ���������-->ȥ�����в�ѯ
	void SocketDisconnected(int descriptor); //�Ͽ����Ӵ���

private:
	int m_port;//�˿ں�
	QList <QTcpSocket*> m_tcpSocketConnectList;//���е����Ӷ���������ά��

};
