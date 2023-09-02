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
	setAttribute(Qt::WA_DeleteOnClose);//���ô�����Դ����һ��
	initTitleBar();//���ñ���������ʼ��
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");//���������ó�QQ��ͼƬ
	loadStyleSheet("SendFile");//������ʽ��
	this->move(100, 400);//�ƶ�����

	//������ж���֮���ͨ�� SendFile--> TalkWindowShell
	TalkWindowShell* talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &SendFile::sendFileClicked,
		talkWindowShell, &TalkWindowShell::updateSendTcpMsg);
}

SendFile::~SendFile()
{
}

//���ļ���ť���
void SendFile::on_openBtn_clicked()
{
	//���ļ��Ի���
	m_filePath = QFileDialog::getOpenFileName(
		this,
		QString::fromLocal8Bit("ѡ���ļ�"),//����
		"/",//��ǰ
		QString::fromLocal8Bit("���͵��ļ�(*.txt *.doc);;�����ļ�(*.*);;")//�ļ�����
	);
	ui.lineEdit->setText(m_filePath);
}

//���Ͱ�ť��ť���
void SendFile::on_sendBtn_clicked()
{
	if (!m_filePath.isEmpty())
	{
		QFile file(m_filePath);
		if (file.open(QIODevice::ReadOnly))//ֻ����
		{
			int msgType = 2; //�ļ�����
			QString str = file.readAll(); //�ļ�����ȫ������

			//�ļ�����
			QFileInfo fileInfo(m_filePath);
			QString fileName = fileInfo.fileName();

			emit sendFileClicked(str, msgType, fileName);//�ź�Я���������ͳ�ȥ
			file.close();
		}
		else
		{
			QMessageBox::information(this,
				QStringLiteral("��ʾ"),
				QString::fromLocal8Bit("�����ļ�:%1ʧ�ܣ�").arg(m_filePath));
			this->close();
			return;
		}

		m_filePath = "";
		this->close();
	}
}
