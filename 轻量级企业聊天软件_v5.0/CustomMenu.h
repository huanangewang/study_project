#pragma once

#include <QMenu>
#include <QMap>

//封装好的自定义菜单类，可以在外部实现，菜单的任意动作添加，目前仅实现：
//显示，关闭

class CustomMenu  : public QMenu
{
	Q_OBJECT

public:
	CustomMenu(QWidget *parent = nullptr);
	~CustomMenu();

public:
	//添加自定义菜单---》1.动作按钮文本，2.菜单图标，3.菜单文本
	void addCustomMenu(const QString& text, const QString& icon,const QString& name );
	//通过文本 获取动作
	QAction* getAction(const QString& text); 

private:
	//映射动作,处理菜单动作,用容器存储动作名字，动作
	QMap <QString, QAction* > m_menuActionMap;
};
