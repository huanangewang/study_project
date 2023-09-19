#pragma once

//#include <QtWidgets/QWidget>
//将基类修改成我们自定义的类,而不再是QWidget
#include "ui_CCMainWindow.h"
#include "basicwindow.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>

class QTreeWidgetItem;


////点击了哪个app图标：
//enum AppType
//{
//	SkinChange //更换皮肤
//	////未完待续///
//};

class CCMainWindow : public BasicWindow
{
	Q_OBJECT

public:
	CCMainWindow(QString account , bool isAccountLogin, QWidget *parent = Q_NULLPTR);
	~CCMainWindow();

public:
	void setUserName(const QString& username);	//设置用户名
	void setLevelPixmap(int level);				//设置等级
	void setHeadPixmap(const QString& headPath);//设置头像
	void setStatusMenuIcon(const QString& statusPath);//设置状态
	//添加应用部件(app图片路径，app部件对象名)--->拓展功能可以慢慢实现：2023.08.06
	QWidget* addOtherAppExtension(const QString& appPath, const QString& appName);

	void initContactTree();//初始化联系人

private:
	void initTimer();	//初始化计时器，用于等级提升
	void initControl();//初始化控件
	QString getHeadPicturePath(); //获取头像路径-->通过数据库
	void updataSeachSheet(); //更新搜索的样式
	void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, int DepID);//添加公司部门  ,1.根项 2.部门名字(现在改成了部门编号ID)

private:
	//重写
	void resizeEvent(QResizeEvent* event);//设置用户名大小事件
	bool eventFilter(QObject* obj, QEvent* event);//事件过滤器返回值是bool,1.监视的对象 2.事件
	void mousePressEvent(QMouseEvent* event);

	//联系人根项
private slots:
	void onItemClicked(QTreeWidgetItem* item, int column);//点击，column列
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);//双击	
	void onItemExpanded(QTreeWidgetItem* item);//拓展
	void onItemCollapsed(QTreeWidgetItem* item);//伸缩

	//app功能槽函数
private slots:
	void onAppIconClicked();//换肤实现

//	//映射保存 项--对应组的id --> 有数据库后，可以不需要
//private:
//	QMap<QTreeWidgetItem*, QString> m_groupMap;//所有分组的分组项

private:
	Ui::CCMainWindowClass ui;

private:
	bool m_isAccountLogin; //是不是账号登录（既可能是名字登录，也可能是员工号登录）
	QString m_account;    //登录的名称账号(名称)，或者是员工号（QQ号）
};
