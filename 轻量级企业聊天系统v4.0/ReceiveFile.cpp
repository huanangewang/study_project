#include "ReceiveFile.h"
#include <QFileDialog>
#include <QMessageBox>

extern QString gfileName;
extern QString gfileData;//文件内容

ReceiveFile::ReceiveFile(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//设置窗口资源保持一致
	initTitleBar();//设置标题栏，初始化
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");//标题栏设置成QQ的图片
	loadStyleSheet("ReceiveFile");//加载样式表
	this->move(100, 400);//移动窗口

	//用户也会点右上角的关闭-->也发送拒绝接收文件的信号
	connect(_titleBar, &TitleBar::signalButtonCloseClicked, this, &ReceiveFile::refuseFile);
}

ReceiveFile::~ReceiveFile()
{
}

//设置接受文件窗口中的标签文字
void ReceiveFile::setMsg(QString & msgLabel)
{
	ui.label->setText(msgLabel);
}

//取消
void ReceiveFile::on_cancelBtn_clicked()
{
	emit refuseFile();//发送拒绝接受文件的信号
	this->close();
}

//确定
void ReceiveFile::on_okBtn_clicked()
{
	this->close();

	//存到本地
	//获取想要保存的文件路径(获取存在的路径)
	QString fileDirPath = QFileDialog::getExistingDirectory(
		nullptr,
		QStringLiteral("文件保存路径"),
		"/"
		);

	QString filePath = fileDirPath + "/" + gfileName;//全局变量:gfileName

	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly))//只写
	{
		QMessageBox::information(nullptr,
			QStringLiteral("提示"),
			QStringLiteral("文件接收失败！"));
	}
	else
	{
		file.write(gfileData.toUtf8());
		file.close();
		QMessageBox::information(nullptr,
			QStringLiteral("提示"),
			QStringLiteral("文件接收成功！"));
	}
}
