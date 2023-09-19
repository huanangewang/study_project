#include "ReceiveFile.h"
#include <QFileDialog>
#include <QMessageBox>

extern QString gfileName;
extern QString gfileData;//�ļ�����

ReceiveFile::ReceiveFile(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//���ô�����Դ����һ��
	initTitleBar();//���ñ���������ʼ��
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");//���������ó�QQ��ͼƬ
	loadStyleSheet("ReceiveFile");//������ʽ��
	this->move(100, 400);//�ƶ�����

	//�û�Ҳ������ϽǵĹر�-->Ҳ���;ܾ������ļ����ź�
	connect(_titleBar, &TitleBar::signalButtonCloseClicked, this, &ReceiveFile::refuseFile);
}

ReceiveFile::~ReceiveFile()
{
}

//���ý����ļ������еı�ǩ����
void ReceiveFile::setMsg(QString & msgLabel)
{
	ui.label->setText(msgLabel);
}

//ȡ��
void ReceiveFile::on_cancelBtn_clicked()
{
	emit refuseFile();//���;ܾ������ļ����ź�
	this->close();
}

//ȷ��
void ReceiveFile::on_okBtn_clicked()
{
	this->close();

	//�浽����
	//��ȡ��Ҫ������ļ�·��(��ȡ���ڵ�·��)
	QString fileDirPath = QFileDialog::getExistingDirectory(
		nullptr,
		QStringLiteral("�ļ�����·��"),
		"/"
		);

	QString filePath = fileDirPath + "/" + gfileName;//ȫ�ֱ���:gfileName

	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly))//ֻд
	{
		QMessageBox::information(nullptr,
			QStringLiteral("��ʾ"),
			QStringLiteral("�ļ�����ʧ�ܣ�"));
	}
	else
	{
		file.write(gfileData.toUtf8());
		file.close();
		QMessageBox::information(nullptr,
			QStringLiteral("��ʾ"),
			QStringLiteral("�ļ����ճɹ���"));
	}
}
