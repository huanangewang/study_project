#pragma once

#include <QtWidgets/QDialog>
#include "ui_QtQQ_Server.h"
#include "TcpServer.h"


#include <QSqlDatabase>
#include<QMessageBox>
#include <QModelIndex>
#include <QTableWidgetItem>
#include <QStringLiteral>
#include <QSqlRecord>
#include <QAbstractItemView>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTimer>
#include <QUdpSocket>

class QtQQ_Server : public QDialog
{
    Q_OBJECT

public:
    QtQQ_Server(QWidget *parent = nullptr);
    ~QtQQ_Server();

private:
	bool connectMysql(); //�������ݿ�
	void initTcpSocket();//��ʼ��tcp�׽���
	void initUdpSocket();//��ʼ��udp�׽���

	int getCompDepID();//��ȡ��˾Ⱥqq��
	void updateTableData(int depID = 0,int employeeID=0);//���±������,�����ʼ����Ϊ�˵�ʱ�����������������Ĭ��ֵ��������Ҫ���ò���id�飬���ߵ���Ա��id��

	void setStatusMap();//״̬ӳ��
	void setdepNameMap();//����ӳ��
	void setOnlineMap();//����״̬ӳ��
	void initComboBoxData();//��ʼ����Ͽ������


private slots:
	void onUDPbroadMsg(QByteArray& btData);//���յ��ͻ�����Ϣ��Ҫ�㲥�Ĳۺ���
	void onRefresh();//ˢ�±������


	//�������ź���ۣ��ڵ����ʱ�򣬻��Զ�����,����Ҫдconnect
	void on_queryDepartmentBtn_clicked();//��ѯ���Ű�ť���(����Ⱥqq��)
	void on_queryIdBtn_clicked();//��ѯԱ����ť���(����Ա��qq�Ų�)
	void on_logoutBtn_clicked(); //ע����ť
	void on_selectPictureBtn_clicked(); //ѡ��ͼƬ��ť(Ա��ͷ��)
	void on_addBtn_clicked(); //���Ա����ť


private:
    Ui::QtQQ_ServerClass ui;

	int m_compDepID;//��˾Ⱥqq��
	int m_depID;//���ź�
	int m_employeeID;//Ա����
	QString m_pixPath; //ͷ��·��

	QSqlQueryModel* m_queryInfoModel;//��ѯ����Ա������Ϣģ��

	//��ӳ�䣬���÷����״̬��ʾ�����Ի�
	QMap<QString, QString> m_statusMap;//״̬ӳ��
	QMap<QString, QString> m_depNameMap;//����ӳ��
	QMap<QString, QString >m_onlineMap; //����״̬ӳ��

	QTimer* m_timer;//��ʱˢ������


	TcpServer* m_tcpServer;//���տͻ��˵���Ϣ tcp�����
	QUdpSocket* m_udpSender; //udp�㲥

};
