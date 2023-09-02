#include "UserLogin.h"
#include "CCMainWindow.h"
#include <QSqlDatabase>
#include<QSqlQuery>
#include<QMessageBox>

//这样设计不好，应该放在类的私有属性里，通过类的方法去访问得到这个值
QString gLoginEmployeeID;//登录者的QQ号(员工号)  将输入的账号进行赋值，方便后面使用

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

////为了访问类内私有成员，并改变它的值
//void UserLogin::setLoginEmployeeID(const QString & UserID)
//{
//	this->g_LoginEmployeeID = UserID;
//}
////获得QQ号
//QString UserLogin::getLoginEmployeeID()
//{
//	return this->g_LoginEmployeeID;
//}

//初始化控件
void UserLogin::initControl()
{
	QLabel *headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);//固定大小
	QPixmap pix(":/Resources/MainWindow/head_mask.png");//圆形头像，设置,这是个空的白圆形
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/app/logo.ico"), pix, headlabel->size()));//通过我们之前设置的方法，获取圆形头像
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked); //点击之后要出现QQ的主窗口

	//连接数据库
	if (!connectMysql())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("连接数据库失败"));
		close();
	}

}

//连接数据库
bool UserLogin::connectMysql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");//数据库名
	db.setHostName("localhost");//主机名
	db.setUserName("root");//用户名
	db.setPassword("root");//密码
	db.setPort(3306);//端口

	if (db.open())
	{
		return true; //正确打开
	}
	else
	{
		return false;
	}
}

//验证账号密码
bool UserLogin::veryfyAccountCode(bool &isAccountLogin, QString &strAccount)
{
	QString strAccountInput = ui.editUserAccount->text(); 
	QString strCodeInput = ui.editPassword->text();

	//数据库进行比对
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID = %1").arg(strAccountInput);//1、QQ号进行登录
	QSqlQuery queryEmployeeID(strSqlCode); //查询qq号(员工号)
	queryEmployeeID.exec();//执行
	
	if (queryEmployeeID.first()) //第一条结果有 结果,指向结果集第一条
	{
		QString strCode = queryEmployeeID.value(0).toString();//0列 ,后面是1 列， 以此类推 就是code,那个字段,这里是数据库中qq号对应的密码

		if (strCode == strCodeInput) //密码正确
		{
			gLoginEmployeeID = strAccountInput;//将输入的账号进行赋值，方便后面使用
			isAccountLogin = false;//QQ号登录（非账号登录）
			strAccount = strAccountInput;//更改传入的参数strAccount：这是账号（比如yanyi）
			return true;
		}
		else
		{
			return false;
		}
	}
	//2、账号登录 code：密码 ；employeeID：QQ号（员工号）
	strSqlCode = QString("SELECT code,employeeID FROM tab_accounts WHERE account = '%1'").arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode); //账号
	queryAccount.exec();

	if (queryAccount.first()) //第一条结果有 结果,指向结果集第一条
	{
		QString strCode = queryAccount.value(0).toString();//0列 ,后面是1 列， 以此类推 就是code,那个字段,这里是数据库中qq号对应的密码

		if (strCode == strCodeInput)//密码正确
		{
			gLoginEmployeeID = queryAccount.value(1).toString();
			isAccountLogin = true;//账号登录
			strAccount = strAccountInput;
			return true;
		}
		else
		{
			return false;
		}
	}

	//账号输入有问题
	return false;
}

//登录
void UserLogin::onLoginBtnClicked()
{
	bool isAccountLogin;
	QString strAccount;//既可能是账号（名称），也可能是QQ号（员工号）
	if (!veryfyAccountCode(isAccountLogin, strAccount))//这两个值会改变，因为我们是引用传递参数(引用传参的用法)
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("您输入的账号或密码有误~~"));

		ui.editPassword->setText("");
		ui.editUserAccount->setText("");
		return;
	}

	//登陆成功后
	//更新登录状态为：上线(2)
	QString strSqlStatus = QString("UPDATE tab_employees SET online = 2 WHERE employeeID = %1").arg(gLoginEmployeeID);//设置sql语句
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();//运行


	this->close();
	CCMainWindow* mainwindow = new CCMainWindow(strAccount, isAccountLogin);//为了主窗口拿到员工id信息-->要区分是账号登录还是QQ号登录
	mainwindow->show();
}