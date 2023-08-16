#include "CustomMenu.h"
#include "CommonUtils.h"

CustomMenu::CustomMenu(QWidget *parent)
	: QMenu(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);//设置透明
	CommonUtils::loadStyleSheet(this, "Menu");//加载样式

}

CustomMenu::~CustomMenu()
{}


//添加自定义菜单---》1.动作按钮文本，2.菜单图标(传入路径即可)，3.菜单文本
void CustomMenu::addCustomMenu(const QString & text, const QString & icon, const QString & name)
{
	QAction* pAction = addAction(QIcon(icon), name);//添加动作
	m_menuActionMap.insert(text, pAction);			//将自定义菜单 加入到容器
}

//通过文本 获取动作
QAction* CustomMenu::getAction(const QString & text)
{
	return m_menuActionMap[text]; //QMap< ,>
}
