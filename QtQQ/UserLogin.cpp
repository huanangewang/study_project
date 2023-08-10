#include "UserLogin.h"
#include "CCMainWindow.h"

UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose); //设置窗体属性
	initTitleBar();//初始化标题
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");//设置图片
	loadStyleSheet("UserLogin");//加载样式表
	initControl();
}

UserLogin::~UserLogin()
{
}

//初始化控件
void UserLogin::initControl()
{
	QLabel *headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);//固定大小
	QPixmap pix(":/Resources/MainWindow/head_mask.png");//圆形头像，设置,这是个空的白圆形
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/xmb.png"), pix, headlabel->size()));//通过我们之前设置的方法，获取圆形头像
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked); //点击之后要出现QQ的主窗口
}

//登录
void UserLogin::onLoginBtnClicked()
{
	this->close();
	CCMainWindow* mainwindow = new CCMainWindow;
	mainwindow->show();
}