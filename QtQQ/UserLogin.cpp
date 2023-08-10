#include "UserLogin.h"
#include "CCMainWindow.h"

UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose); //���ô�������
	initTitleBar();//��ʼ������
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");//����ͼƬ
	loadStyleSheet("UserLogin");//������ʽ��
	initControl();
}

UserLogin::~UserLogin()
{
}

//��ʼ���ؼ�
void UserLogin::initControl()
{
	QLabel *headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);//�̶���С
	QPixmap pix(":/Resources/MainWindow/head_mask.png");//Բ��ͷ������,���Ǹ��յİ�Բ��
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/xmb.png"), pix, headlabel->size()));//ͨ������֮ǰ���õķ�������ȡԲ��ͷ��
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked); //���֮��Ҫ����QQ��������
}

//��¼
void UserLogin::onLoginBtnClicked()
{
	this->close();
	CCMainWindow* mainwindow = new CCMainWindow;
	mainwindow->show();
}