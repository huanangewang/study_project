#include "ContactItem.h"

ContactItem::ContactItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initControl();
}

ContactItem::~ContactItem()
{
}

//设置用户名
void ContactItem::setUserName(const QString & userName)
{
	ui.userName->setText(userName);
}

//设置个性签名
void ContactItem::setSignName(const QString & signName)
{
	ui.signName->setText(signName);
}

//设置头像
void ContactItem::setHeadPixmap(const QPixmap & headPath)
{
	ui.label->setPixmap(headPath);
}

//返回用户 以字符串返回  小知识点：----后面加个const，成员函数这样就不能对成员数据进行修改
QString ContactItem::getUserName() const
{
	return ui.userName->text();
}

//主要用于设置圆头像
QSize ContactItem::getHeadLabelSize() const
{
	return ui.label->size();
}

void ContactItem::initControl()
{
}
