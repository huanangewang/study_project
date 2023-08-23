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
	//Ŀ���Ǹ���server��������Ҫ����
	void signalGetDataFromClient(QByteArray&, int);//�ӿͻ����յ�����,����Ҫ֪������������ʲô��ҲҪ֪�����ı�ʶ
	void signalClientDisconnect(int); //����server �пͻ��˶Ͽ�������


private slots:
	void onReceiveData();//����ͻ��˷��������� readyRead�źŵĶ�ȡ
	void onClientDisconnect(); //�ͻ��˶Ͽ�����

private:
	int m_descriptor;//������Ϊ Ψһ��ʶ ������

};
