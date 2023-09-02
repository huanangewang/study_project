#include "UserLogin.h"
#include "CCMainWindow.h"
#include <QSqlDatabase>
#include<QSqlQuery>
#include<QMessageBox>

//������Ʋ��ã�Ӧ�÷������˽�������ͨ����ķ���ȥ���ʵõ����ֵ
QString gLoginEmployeeID;//��¼�ߵ�QQ��(Ա����)  ��������˺Ž��и�ֵ���������ʹ��

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

////Ϊ�˷�������˽�г�Ա�����ı�����ֵ
//void UserLogin::setLoginEmployeeID(const QString & UserID)
//{
//	this->g_LoginEmployeeID = UserID;
//}
////���QQ��
//QString UserLogin::getLoginEmployeeID()
//{
//	return this->g_LoginEmployeeID;
//}

//��ʼ���ؼ�
void UserLogin::initControl()
{
	QLabel *headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);//�̶���С
	QPixmap pix(":/Resources/MainWindow/head_mask.png");//Բ��ͷ������,���Ǹ��յİ�Բ��
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/app/logo.ico"), pix, headlabel->size()));//ͨ������֮ǰ���õķ�������ȡԲ��ͷ��
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked); //���֮��Ҫ����QQ��������

	//�������ݿ�
	if (!connectMysql())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("�������ݿ�ʧ��"));
		close();
	}

}

//�������ݿ�
bool UserLogin::connectMysql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");//���ݿ���
	db.setHostName("localhost");//������
	db.setUserName("root");//�û���
	db.setPassword("root");//����
	db.setPort(3306);//�˿�

	if (db.open())
	{
		return true; //��ȷ��
	}
	else
	{
		return false;
	}
}

//��֤�˺�����
bool UserLogin::veryfyAccountCode(bool &isAccountLogin, QString &strAccount)
{
	QString strAccountInput = ui.editUserAccount->text(); 
	QString strCodeInput = ui.editPassword->text();

	//���ݿ���бȶ�
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID = %1").arg(strAccountInput);//1��QQ�Ž��е�¼
	QSqlQuery queryEmployeeID(strSqlCode); //��ѯqq��(Ա����)
	queryEmployeeID.exec();//ִ��
	
	if (queryEmployeeID.first()) //��һ������� ���,ָ��������һ��
	{
		QString strCode = queryEmployeeID.value(0).toString();//0�� ,������1 �У� �Դ����� ����code,�Ǹ��ֶ�,���������ݿ���qq�Ŷ�Ӧ������

		if (strCode == strCodeInput) //������ȷ
		{
			gLoginEmployeeID = strAccountInput;//��������˺Ž��и�ֵ���������ʹ��
			isAccountLogin = false;//QQ�ŵ�¼�����˺ŵ�¼��
			strAccount = strAccountInput;//���Ĵ���Ĳ���strAccount�������˺ţ�����yanyi��
			return true;
		}
		else
		{
			return false;
		}
	}
	//2���˺ŵ�¼ code������ ��employeeID��QQ�ţ�Ա���ţ�
	strSqlCode = QString("SELECT code,employeeID FROM tab_accounts WHERE account = '%1'").arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode); //�˺�
	queryAccount.exec();

	if (queryAccount.first()) //��һ������� ���,ָ��������һ��
	{
		QString strCode = queryAccount.value(0).toString();//0�� ,������1 �У� �Դ����� ����code,�Ǹ��ֶ�,���������ݿ���qq�Ŷ�Ӧ������

		if (strCode == strCodeInput)//������ȷ
		{
			gLoginEmployeeID = queryAccount.value(1).toString();
			isAccountLogin = true;//�˺ŵ�¼
			strAccount = strAccountInput;
			return true;
		}
		else
		{
			return false;
		}
	}

	//�˺�����������
	return false;
}

//��¼
void UserLogin::onLoginBtnClicked()
{
	bool isAccountLogin;
	QString strAccount;//�ȿ������˺ţ����ƣ���Ҳ������QQ�ţ�Ա���ţ�
	if (!veryfyAccountCode(isAccountLogin, strAccount))//������ֵ��ı䣬��Ϊ���������ô��ݲ���(���ô��ε��÷�)
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("��������˺Ż���������~~"));

		ui.editPassword->setText("");
		ui.editUserAccount->setText("");
		return;
	}

	//��½�ɹ���
	//���µ�¼״̬Ϊ������(2)
	QString strSqlStatus = QString("UPDATE tab_employees SET online = 2 WHERE employeeID = %1").arg(gLoginEmployeeID);//����sql���
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();//����


	this->close();
	CCMainWindow* mainwindow = new CCMainWindow(strAccount, isAccountLogin);//Ϊ���������õ�Ա��id��Ϣ-->Ҫ�������˺ŵ�¼����QQ�ŵ�¼
	mainwindow->show();
}