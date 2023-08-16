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

//�����û���
void ContactItem::setUserName(const QString & userName)
{
	ui.userName->setText(userName);
}

//���ø���ǩ��
void ContactItem::setSignName(const QString & signName)
{
	ui.signName->setText(signName);
}

//����ͷ��
void ContactItem::setHeadPixmap(const QPixmap & headPath)
{
	ui.label->setPixmap(headPath);
}

//�����û� ���ַ�������  С֪ʶ�㣺----����Ӹ�const����Ա���������Ͳ��ܶԳ�Ա���ݽ����޸�
QString ContactItem::getUserName() const
{
	return ui.userName->text();
}

//��Ҫ��������Բͷ��
QSize ContactItem::getHeadLabelSize() const
{
	return ui.label->size();
}

void ContactItem::initControl()
{
}
