#include "SendFile.h"
#include "TalkWindowShell.h"
#include "WindowManager.h"
#include <QFileDialog>
#include <QMessageBox>

SendFile::SendFile(QWidget *parent)
	: BasicWindow(parent)
	, m_filePath("")
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//设置窗口资源保持一致
	initTitleBar();//设置标题栏，初始化
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");//标题栏设置成QQ的图片
	loadStyleSheet("SendFile");//加载样式表
	this->move(100, 400);//移动窗口

	//下面进行对象之间的通信 SendFile--> TalkWindowShell
	TalkWindowShell* talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &SendFile::sendFileClicked,
		talkWindowShell, &TalkWindowShell::updateSendTcpMsg);
}

SendFile::~SendFile()
{
}

//打开文件按钮点击
void SendFile::on_openBtn_clicked()
{
	//打开文件对话框
	m_filePath = QFileDialog::getOpenFileName(
		this,
		QString::fromLocal8Bit("选择文件"),//标题
		"/",//当前
		QString::fromLocal8Bit("发送的文件(*.txt *.doc);;所有文件(*.*);;")//文件类型
	);
	ui.lineEdit->setText(m_filePath);
}

//发送按钮按钮点击
void SendFile::on_sendBtn_clicked()
{
	if (!m_filePath.isEmpty())
	{
		QFile file(m_filePath);
		if (file.open(QIODevice::ReadOnly))//只读打开
		{
			int msgType = 2; //文件类型
			QString str = file.readAll(); //文件内容全读出来

			//文件名称
			QFileInfo fileInfo(m_filePath);
			QString fileName = fileInfo.fileName();

			emit sendFileClicked(str, msgType, fileName);//信号携带参数发送出去
			file.close();
		}
		else
		{
			QMessageBox::information(this,
				QStringLiteral("提示"),
				QString::fromLocal8Bit("发送文件:%1失败！").arg(m_filePath));
			this->close();
			return;
		}

		m_filePath = "";
		this->close();
	}
}
