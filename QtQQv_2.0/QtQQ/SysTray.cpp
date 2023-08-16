#include "SysTray.h"
#include <QStringLiteral>
#include "CustomMenu.h"

SysTray::SysTray(QWidget *parent)
	: m_partent(parent)
	,QSystemTrayIcon(parent)
{
	initSystemTray();
	show();//显示
}

SysTray::~SysTray()
{
}

//初始化系统托盘
void SysTray::initSystemTray()
{
	setToolTip(QStringLiteral("华南歌王的QQ"));//设置文本提示
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));

	//点击，能触发父窗体
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(onIconActiveted(QSystemTrayIcon::ActivationReason)));
}

//增加系统托盘菜单
void SysTray::addSystrayMenu()
{
	CustomMenu *customMenu = new CustomMenu(m_partent);
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", QStringLiteral("显示"));

	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", QStringLiteral("退出"));

	//实现功能
	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)),
		m_partent, SLOT(onShowNormal(bool)));//显示

	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)),
		m_partent, SLOT(onShowQuit(bool)));//关闭
	//让自定义菜单进入事件循环，接收鼠标操作
	customMenu->exec(QCursor::pos());

	//因为进入了事件循环，所以这里自己释放
	delete customMenu;
	customMenu = nullptr;
}


//系统托盘图标被触发---->参数是被激活的原因
void SysTray::onIconActiveted(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger)
	{
		//如果是用户点击系统托盘图标-->显示父窗体
		m_partent->show();
		m_partent->activateWindow();//设置成活动窗口
	}
	else if (reason == QSystemTrayIcon::Context)
	{
		addSystrayMenu();
	}
}