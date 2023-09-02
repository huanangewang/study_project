#include "SysTray.h"
#include <QStringLiteral>
#include "CustomMenu.h"

SysTray::SysTray(QWidget *parent)
	: m_partent(parent)
	,QSystemTrayIcon(parent)
{
	initSystemTray();
	show();//��ʾ
}

SysTray::~SysTray()
{
}

//��ʼ��ϵͳ����
void SysTray::initSystemTray()
{
	setToolTip(QStringLiteral("���ϸ�����QQ"));//�����ı���ʾ
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));

	//������ܴ���������
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(onIconActiveted(QSystemTrayIcon::ActivationReason)));
}

//����ϵͳ���̲˵�
void SysTray::addSystrayMenu()
{
	CustomMenu *customMenu = new CustomMenu(m_partent);
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", QStringLiteral("��ʾ"));

	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", QStringLiteral("�˳�"));

	//ʵ�ֹ���
	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)),
		m_partent, SLOT(onShowNormal(bool)));//��ʾ

	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)),
		m_partent, SLOT(onShowQuit(bool)));//�ر�
	//���Զ���˵������¼�ѭ��������������
	customMenu->exec(QCursor::pos());

	//��Ϊ�������¼�ѭ�������������Լ��ͷ�
	delete customMenu;
	customMenu = nullptr;
}


//ϵͳ����ͼ�걻����---->�����Ǳ������ԭ��
void SysTray::onIconActiveted(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger)
	{
		//������û����ϵͳ����ͼ��-->��ʾ������
		m_partent->show();
		m_partent->activateWindow();//���óɻ����
	}
	else if (reason == QSystemTrayIcon::Context)
	{
		addSystrayMenu();
	}
}