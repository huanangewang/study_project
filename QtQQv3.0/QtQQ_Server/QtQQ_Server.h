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
	bool connectMysql(); //连接数据库
	void initTcpSocket();//初始化tcp套接字
	void initUdpSocket();//初始化udp套接字

	int getCompDepID();//获取公司群qq号
	void updateTableData(int depID = 0,int employeeID=0);//更新表格数据,这里初始化是为了到时候如果不传，可以有默认值，比如我要但用部门id查，或者单用员工id查

	void setStatusMap();//状态映射
	void setdepNameMap();//部门映射
	void setOnlineMap();//在线状态映射
	void initComboBoxData();//初始化组合框的数据


private slots:
	void onUDPbroadMsg(QByteArray& btData);//接收到客户端信息后，要广播的槽函数
	void onRefresh();//刷新表格数据


	//这两个信号与槽，在点击的时候，会自动连接,不需要写connect
	void on_queryDepartmentBtn_clicked();//查询部门按钮点击(根据群qq号)
	void on_queryIdBtn_clicked();//查询员工按钮点击(根据员工qq号查)
	void on_logoutBtn_clicked(); //注销按钮
	void on_selectPictureBtn_clicked(); //选择图片按钮(员工头像)
	void on_addBtn_clicked(); //添加员工按钮


private:
    Ui::QtQQ_ServerClass ui;

	int m_compDepID;//公司群qq号
	int m_depID;//部门号
	int m_employeeID;//员工号
	QString m_pixPath; //头像路径

	QSqlQueryModel* m_queryInfoModel;//查询所有员工的信息模型

	//加映射，来让服务端状态显示更人性化
	QMap<QString, QString> m_statusMap;//状态映射
	QMap<QString, QString> m_depNameMap;//部门映射
	QMap<QString, QString >m_onlineMap; //在线状态映射

	QTimer* m_timer;//定时刷新数据


	TcpServer* m_tcpServer;//接收客户端的信息 tcp服务端
	QUdpSocket* m_udpSender; //udp广播

};
