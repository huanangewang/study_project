#pragma once

#include <QWidget>
#include "ui_ContactItem.h"

//实现列表中，根节点点开后，显示头像、用户名、个性签名--->有UI
class ContactItem : public QWidget
{
	Q_OBJECT

public:
	ContactItem(QWidget *parent = Q_NULLPTR);
	~ContactItem();

	void setUserName(const QString& userName);//设置用户名
	void setSignName(const QString& signName);//设置个性签名
	void setHeadPixmap(const QPixmap& headPath);//设置头像
	QString getUserName()const; //返回用户 以字符串返回  小知识点：----后面加个const，成员函数这样就不能对成员数据进行修改
	QSize getHeadLabelSize()const;//主要用于设置圆头像

private:
	void initControl();//初始化控件

private:
	Ui::ContactItem ui;
};
