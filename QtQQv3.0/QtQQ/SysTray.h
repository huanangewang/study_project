#pragma once

#include <QSystemTrayIcon>
#include <QWidget>

//功能介绍：data:2023.08.06，华南歌王
//为了实现系统托盘--->最小化-->并且能点击，将父窗口设置活动窗口--->
//还能实现右键菜单，可以退出
//自定义菜单,封装成另一个类--->以便后续添加更多的自定义菜单~
class SysTray  : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget *parent);
	~SysTray();

public slots:
	//系统托盘图标被触发---->参数是被激活的原因
	void onIconActiveted(QSystemTrayIcon::ActivationReason reason);

private:
	void initSystemTray(); //初始化系统托盘
	void addSystrayMenu();//增加系统托盘菜单

private:
	//这里不能直接在 构造中就使用QWidget,因为我们最小化，再次点击的时候，要还原成之前的QWidget,也就是CCMainWindow
	QWidget* m_partent;//加个父类指针
};
