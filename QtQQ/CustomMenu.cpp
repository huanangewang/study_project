#include "CustomMenu.h"
#include "CommonUtils.h"

CustomMenu::CustomMenu(QWidget *parent)
	: QMenu(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);//����͸��
	CommonUtils::loadStyleSheet(this, "Menu");//������ʽ

}

CustomMenu::~CustomMenu()
{}


//����Զ���˵�---��1.������ť�ı���2.�˵�ͼ��(����·������)��3.�˵��ı�
void CustomMenu::addCustomMenu(const QString & text, const QString & icon, const QString & name)
{
	QAction* pAction = addAction(QIcon(icon), name);//��Ӷ���
	m_menuActionMap.insert(text, pAction);			//���Զ���˵� ���뵽����
}

//ͨ���ı� ��ȡ����
QAction* CustomMenu::getAction(const QString & text)
{
	return m_menuActionMap[text]; //QMap< ,>
}
