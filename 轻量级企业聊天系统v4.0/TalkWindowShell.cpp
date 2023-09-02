#include "TalkWindowShell.h"
#include "EmotionWindow.h"
#include "TalkWindow.h"
#include "CommonUtils.h"
#include <qlistwidget.h>
#include "TalkWindowItem.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFile>
#include <QSqlQuery>
#include "WindowManager.h"
#include "ReceiveFile.h"
#include"PoolTask.h"
#include<QThreadPool>

extern QString gLoginEmployeeID;
const int gUdpPort = 6666;

QString gfileName; //�ļ�����
QString gfileData;//�ļ�����

//�޸Ļ��� BasicWindow
TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//�������ԣ������ڹرգ�Ҫ������Դ����
	initControl();//��ʼ���ؼ�
	initTcpSocket();//��ʼ��tcp
	initUdpSocket();//��ʼ��udp


	QFile file("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size())//���js��û���ݣ���д
	{
		QStringList employeesIdList;
		getEmployeesID(employeesIdList);//ֱ�Ӵ�ֵ�����������ô��Σ���ı�employeesIdList��
		if (!createJSFile(employeesIdList))//дJS�ļ�
		{
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����js�ļ�ʧ��"));
		}
	}
}

TalkWindowShell::~TalkWindowShell()
{
	delete m_emotionWindow;
	m_emotionWindow = nullptr;
}

//�������-->���촰��  ,����2 �����ں���һ������
//���������һ��һ��һ�Զ�
void TalkWindowShell::addTalkWindow(TalkWindow* talkWindow, TalkWindowItem * talkWindowItem, const QString& uid/* GroupType grouptype*/)
{
	ui.rightStackedWidget->addWidget(talkWindow);//˫�������һ�����촰��
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()),
		talkWindow, SLOT(onSetEmotionBtnStatus()));// onSetEmotionBtnStatus���ñ��鰴ť״̬

	QListWidgetItem* aItem = new QListWidgetItem(ui.listWidget);//����б�
	m_talkwindowItemMap.insert(aItem, talkWindow); //key:aItem ��value:talkWindow

	aItem->setSelected(true);//��ѡ��

	//ͨ�����ݿ�����ͷ��
	//�ж���Ⱥ�Ļ��ǵ���
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(uid);
	sqlDepModel.setQuery(strQuery);//���ò�ѯ��������û�н��
	int rows = sqlDepModel.rowCount(); //0�о��ǵ��ģ�����Ⱥ��

	if (rows == 0)//����,Ҫ����sql���
	{
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid);
		sqlDepModel.setQuery(strQuery);
	}

	QModelIndex index;//ģ������
	index = sqlDepModel.index(0, 0);//�У��У����ͷ�������·��

	QImage img;
	img.load(sqlDepModel.data(index).toString());
	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));//����ͷ�� TODO:�ĳ�Pixmap

	ui.listWidget->addItem(aItem);//����б���(���)
	ui.listWidget->setItemWidget(aItem, talkWindowItem);//���ò���

	onTalkWindowItemClicked(aItem);//��ǰ������

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked,
		[talkWindowItem, talkWindow, aItem, this]()
	{
		//Ҫ��ӳ���Ƴ����б����Ƴ������촰�ڹرգ�talkWindowItem��Դ�ͷ�
		//�ȴ������
		m_talkwindowItemMap.remove(aItem);
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));
		delete talkWindowItem;
		//�����ұ�
		ui.rightStackedWidget->removeWidget(talkWindow);
		if (ui.rightStackedWidget->count() < 1)
		{
			this->close();
		}
	});
}

//���õ�ǰ����Ĵ���(���ĸ����ڽ�������)
void TalkWindowShell::setCurrentWidget(QWidget * widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

//Ϊ�����ⲿ�������˽�г�Ա
const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::getTalkWindowItemMap() const
{
	return m_talkwindowItemMap;
}

//��ʼ���ؼ�
void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");//������ʽ��
	setWindowTitle(QString::fromLocal8Bit("˧��-���촰��"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();		//���ر��鴰��

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;//���154�����أ��ұ�...
	ui.splitter->setSizes(leftWidgetSize);	//���������óߴ�

	ui.listWidget->setStyle(new CustomProxyStyle(this));//���÷��

	//��౻����Ļ������߱��鱻ѡ��
	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}

//��ʼ��tcp�׽��� --->1��1��
void TalkWindowShell::initTcpSocket()
{
	m_tcpClientSocket = new QTcpSocket(this);
	m_tcpClientSocket->connectToHost("127.0.0.1", gtcpPort);//������������������� gtcpPort:6666

}

//��ʼ��udp�׽��� --->ֻ�󶨶˿ڣ��������ݽ���,˭���������ԣ�ֻҪ�˿�������
void TalkWindowShell::initUdpSocket()
{
	m_udpReceiver = new QUdpSocket(this);

	//��˿ڽ��н���
	for (quint16 port = gUdpPort; port < gUdpPort + 200; ++port)
	{
		if (m_udpReceiver->bind(port, QUdpSocket::ShareAddress))//�˿ڡ�ģʽ
		{
			break;//�󶨳ɹ��󷵻�
		}
	}
	//һ���˿������������ˣ��ͽ������ݴ��� 
	//TODO:�����̳߳������񼴿ɣ��̳߳����Զ������߳�ִ�� 2023-08-30
	connect(m_udpReceiver, &QUdpSocket::readyRead, this, &TalkWindowShell::onProcessPendingData);
}


//��ȡ����Ա��QQ��,Ϊ�˸ı�employeesIdList�������޸ĳ����ô���
void TalkWindowShell::getEmployeesID(QStringList& employeesIdList)
{
	//֮ǰд�˸������ؾֲ����������ã���Ȼû��
	QSqlQueryModel queryModel;
	queryModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status = 1"));//����״̬Ϊ1 ��Ա��
	int employeesNum = queryModel.rowCount(); //�����м���Ա��(id)
	QModelIndex index; //ģ��Ҫ�����ݣ�����Ҫ����
	//��ӵ�List�б���
	for (int i = 0;i < employeesNum;i++)
	{
		index = queryModel.index(i, 0); // �У���
		employeesIdList << queryModel.data(index).toString(); //������,��ӵ�������
	}

}

//������дJS�ļ���
bool TalkWindowShell::createJSFile(QStringList& employeesList)
{
	//��ȡ.txt�ļ�����
	QString strFileTxt = "Resources/MainWindow/MsgHtml/msgtmpl.txt"; //�ȶ�ԭʼ����
	QFile fileRead(strFileTxt);//��������txt���ݶ����ļ���
	QString strFile;

	if (fileRead.open(QIODevice::ReadOnly))
	{
		strFile = fileRead.readAll();
		fileRead.close();
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�����δ����js�ļ�����ȡmsgtmpl.txtʧ��"));
		return false;
	}

	//�滻����ǰ������ԭʼ����
	//�滻���ݣ�external0,appendHtml0 ����׷����Ϣ�Ľű�����Ҳ�����Լ�����Ϣ�õģ��������滻��
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");//д��js��Ŀ���ַ(js��Ϣ ���������λ��)

	if (fileWrite.open(QIODevice::WriteOnly) | QIODevice::Truncate) //�������˼�ǣ����и���
	{
		//������ԭʼ���ݣ�strSourceInitNull ;strSourceInit; strSourceNew ;strSourceRecvHtml
		QString strSourceInitNull = "var external = null;";//���¿�ֵ
		QString strSourceInit = "external = channel.objects.external;";//ͨ���Ķ���,//��ʼֵ
		//����newWebChannel
		QString strSourceNew =
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
		}\
		); \
		";
		//����׷�ӵĽű� --> ����ֱ�Ӷ�recvHtml.txt����(���ļ���ʽ) ,��Ϊ����Ľű�����˫����(����������⴦��)
		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly))
		{
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��ȡrecvHtml.txtʧ��"));
			return false;
		}

		//�����滻��Ľű�
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;

		//����ÿ��Ա����
		for (int i = 0;i < employeesList.length();i++)
		{
			//�༭�滻��Ŀ�ֵ
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i))); //%1��Ա����id�滻
			strReplaceInitNull += strInitNull;//���滻��Ľ��������strReplaceInitNull
			strReplaceInitNull += "\n";//����

			//�༭�滻��ĳ�ʼֵ
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i))); //%1��Ա����id�滻
			strReplaceInit += strInit;//���滻��Ľ��������strReplaceInit
			strReplaceInit += "\n";//����

			//�༭�滻���newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i))); //%1��Ա����id�滻
			strReplaceNew += strNew;//���滻��Ľ��������strReplaceNew
			strReplaceNew += "\n";//����

			//�༭�滻��� recvHtml
			QString strRecvHtml = strSourceRecvHtml;
			strRecvHtml.replace("external", QString("external_%1").arg(employeesList.at(i))); //%1��Ա����id�滻
			strRecvHtml.replace("external", QString("external_%1").arg(i));//�ýű�����Ҳ��һ������Ȼ���ж���ͬһ���ű� 
			strReplaceRecvHtml += strRecvHtml; //���滻��Ľ��������strReplaceRecvHtml
			strReplaceRecvHtml += "\n";//����
		}

		//��ʼ�������滻 ���滻���strFile�о���������Ҫ������������
		strFile.replace(strSourceInitNull, strReplaceInitNull);
		strFile.replace(strSourceInit, strReplaceInit);
		strFile.replace(strSourceNew, strReplaceNew);
		strFile.replace(strSourceRecvHtml, strReplaceRecvHtml);

		QTextStream stream(&fileWrite); //����д���ļ� fileWrite��QFile�ļ�,֮ǰ�Ѿ���ֻд�ķ�ʽ�򿪣�������
		stream << strFile;//д��
		fileWrite.close();
		return true;

	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("д msgtmpl.js ʧ��"));
		return false;
	}

}

//�����յ�����Ϣ��������ҳ��
void TalkWindowShell::handleReceivedMsg(int senderEmployeeID, int msgType, QString strMsg)
{
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setText(strMsg);

	if (msgType == 1)//�ı���Ϣ
	{
		msgTextEdit.document()->toHtml();
	}
	else if (msgType == 0)//������Ϣ
	{
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;

		for (int i = 0; i < emotionNum; i++)
		{
			msgTextEdit.addEmotionUrl(strMsg.mid(i * emotionWidth, emotionWidth).toInt());
		}
	}

	QString html = msgTextEdit.document()->toHtml();

	//�ı�html���û���������������
	if (!html.contains(".png") && !html.contains("</span>"))
	{
		QString fontHtml;
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			fontHtml.replace("%1", strMsg);
			file.close();
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ")
				, QString::fromLocal8Bit("�ļ� msgFont.txt �����ڣ�"));
			return;
		}

		if (!html.contains(fontHtml))
		{
			html.replace(strMsg, fontHtml);
		}
	}

	TalkWindow* talkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	talkWindow->ui.msgWidget->appendMsg(html, QString::number(senderEmployeeID));

}

//���鰴ť�����
void TalkWindowShell::onEmotionBtnClicked(bool)
{
	//��Ҫ�Ǳ��鴰�ڿɼ������ǲ��ɼ�
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//����ǰ�ؼ������λ�ã�ת��Ϊ��Ļ�ľ���λ��

	emotionPoint.setX(emotionPoint.x() + 170);//ƫ�Ʊ��鴰�ڣ�Ϊ�˺ÿ�
	emotionPoint.setY(emotionPoint.y() + 220);

	m_emotionWindow->move(emotionPoint);
}

/*
1���ı����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ����id��(qq��) + ����Ϣ��������Ϣ�˵�id�ţ�������Ⱥqq�ţ�
 +��Ϣ����(�ı������顢�ļ�) + ���ݳ��� + ����
	 msgType:0-->������Ϣ  1-->�ı���Ϣ 2-->�ļ���Ϣ
2���������ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ����id��(qq��)+ ����Ϣ��������Ϣ�˵�id�ţ�������Ⱥqq�ţ�
 +��Ϣ����(�ı������顢�ļ�) + ������� + images + ����
*/

//�յ�TalkWindow �����ķ�����Ϣ �źź� Ҫʵ�ֵĲۺ���
void TalkWindowShell::updateSendTcpMsg(QString& strData, int& msgType, QString fileName)
{
	//�Ȼ�ȡ�����������촰��  �õ����ݰ��������ʽ����ʱ�򷽱������ͻ��˽������ݵ�ʱ����н���
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget()); //����ұߵĻ����
	QString talkId = curTalkWindow->getTalkId();

	QString strGroupFlag;//Ⱥ�ı�־
	QString strSend; //���͵�����

	if (talkId.length() == 4) //����Ⱥqq�ŵĳ���
	{
		strGroupFlag = "1";//��,talkId��Ⱥqq��
	}
	else
	{
		strGroupFlag = "0";//����,talkId����ĳ��Ա����qq��
	}
	//2023-08-18 Bug���
	int nstrDataLength = strData.length();//��hellowrold���������10,ԭʼ���ݳ���
	int dataLength = QString::number(nstrDataLength).length();//���ݳ��� "10" ��ʵӦ����ռ�ַ���Ҳ������������Ӧ����10
	//const int sourceDataLength = dataLength;//ԭʼ���ݳ���
	QString strdataLength; //�������ݳ���

	if (msgType == 1)//���͵����ı���Ϣ
	{
		//�ı���Ϣ����Լ��Ϊ5-->��������0����5λ
		if (dataLength == 1)
		{
			strdataLength = "0000" + QString::number(nstrDataLength); //����
		}
		else if (dataLength == 2)
		{
			strdataLength = "000" + QString::number(nstrDataLength); //����
		}
		else if (dataLength == 3)
		{
			strdataLength = "00" + QString::number(nstrDataLength); //����
		}
		else if (dataLength == 4)
		{
			strdataLength = "0" + QString::number(nstrDataLength); //����
		}
		else if (dataLength == 5)
		{
			strdataLength = QString::number(nstrDataLength);
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����������ݳ���"));
		}

		//��֯���ݰ�
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strdataLength + strData;
	}
	else if (msgType == 0)//���͵��Ǳ�����Ϣ
	{
		//��֯���ݰ�
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData; //strData���Ѿ������� ������� + images + ����
	}
	else if (msgType == 2)//�ļ���Ϣ
	{   /*
			�ļ����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��qq�� + ����ϢԱ��qq�ţ�Ⱥqq�ţ�+
			��Ϣ����(2) + �ļ����� + "bytes "+�ļ����� + "data_begin" + �ļ�����
		*/

		QString strLength = QString::number(strData.toUtf8().length()); //��ȡ�ֽ�����,��������ĳ���,toUtf8����QStringת���ֽ�����
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2" + strLength + fileName + "data_begin" + strData;//��֯���ݰ�
	}

	//ǰ����ð��󣬾�Ҫ������ͻ���д�����ݣ������ֽ�������ʽ
	QByteArray dataBt;
	dataBt.resize(strSend.length());//�����ֽ����鳤��
	dataBt = strSend.toUtf8();//toUtf8����QStringת���ֽ�����
	m_tcpClientSocket->write(dataBt);//��ʼд���ݣ����Ĵ��룩


}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem * item)
{
	QWidget* talkwindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkwindowWidget);
}

//ѡ���ĸ�����
void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	//��������TalkWindow��,Ĭ�Ϸ����ǲ������ͣ�����ǿת
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());

	if (curTalkWindow)
	{
		//��ӱ���ͼƬ
		curTalkWindow->addEmotionImage(emotionNum);

	}
}

/*  ���ݰ��ĸ�ʽԼ����

	�ı����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ����id + ����ϢԱ��id�ţ�Ҳ������Ⱥid�ţ�+ ��Ϣ����(1) + ���ݳ���+����;
	�������ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ����id + ����ϢԱ��id�ţ�Ҳ������Ⱥid�ţ�+ ��Ϣ����(0) + ������� + images +��������(3λ��);
	�ļ����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ����id + ����ϢԱ��id�ţ�Ҳ������Ⱥid�ţ�+ ��Ϣ����(2)
	+ �ļ��ֽ��� + bytes +�ļ��� + data_begin +�ļ�����;

	Ⱥ�ı�־ռ1λ��0��ʾ���ģ�1��ʾȺ��
	��Ϣ����ռ1λ��0��ʾ������Ϣ��1��ʾ�ı���Ϣ��2��ʾ�ļ���Ϣ
	qq��ռ5λ��qqȺ��ռ4λ�����ݳ���ռ5λ����������ռ3λ
	����Ⱥ�ı�־Ϊ1�������ݰ���������Ϣ�˵�qq�ţ���������Ϣ Ⱥqq��,
	��Ⱥ�ı�־Ϊ0�������ݰ���û������ϢȺqq�ţ���������Ϣ���˵�qq��

	���磺Ⱥ���ı���Ϣ��1 10001 2001 1 00005 Hello ��ʾ10001�������Ⱥ2001 �����ı���Ϣ Hello
		  ����ͼƬ��Ϣ��0 10001 10002 0 1 images 060        ��ʾQQ10001��QQ10002���ͱ���60.png
		  Ⱥ���ļ���Ϣ��1 10005 2000 2 10 bytes test.txt data_begin   ��ʾqq10005��Ⱥ2000�����ļ�(test.txt)��Ϣ
*/


//���������udp�㲥�յ�������(����)
void TalkWindowShell::onProcessPendingData()
{
	while (m_udpReceiver->hasPendingDatagrams())//udp�˿����Ƿ���δ��������� bool
	{
		const static int groupFlagWidth = 1;	//Ⱥ�ı�־ռλ�����ԶΪ1
		const static int groupWidth = 4;		//Ⱥqq�ſ��
		const static int employeeWidth = 5;		//Ա��qq�ſ��
		const static int msgTypeWidth = 1;		//��Ϣ���Ϳ��
		const static int msgLengthWidth = 5;	//�ı���Ϣ���ȵĿ��
		const static int pictureWidth = 3;		//����ͼƬ���
	
		//�ó�����
		QByteArray btData;
		btData.resize(m_udpReceiver->pendingDatagramSize());//δ�������ݵĴ�С(����Ҫ��������ݴ�С)
		m_udpReceiver->readDatagram(btData.data(), btData.size());//��ȡudp����(���ݡ���С)
	
		QString strData = btData.data();//�õ�ȫ������
		QString strWindowID;//���촰��ID��Ⱥ�ľ���Ⱥ�ţ����ľ���Ա��id��
		QString strSendEmployeeID, strRecevieEmployeeID;//���ͼ����ն˵�qq��
		QString strMsg;//����
	
		int msgLen;//���ݳ���
		int msgType;//��������
	
		strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);//��ȡ,��Ⱥ�ı�־��ʼ����ȡid�ų���
	
		//�������˷�����ģ������Լ���������Ϣ��Ҳ���Ա��������㲥���Լ����������������
		if (strSendEmployeeID == gLoginEmployeeID)
		{
			return;
		}
	
		if (btData[0] == '1') //Ⱥ��
		{
			//�õ�Ⱥ�Ĵ�������id
			strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth); //ͨ������Ķ���Ŀ�Ⱥͽ�����ݰ���ʽ���н�ȡ
	
			//�ж���Ϣ����,��Ϊ��QChar���ͣ�������'1'������
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];
			if (cMsgType == '1')//�ı���Ϣ
			{
				msgType = 1;//��Ϣ����
				msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();//��ȡ��ռλΪ5����Ϣ�����Ƕ���
	
				//��ȡ��Ϣ--��ͨ����ȡ���õ�����Ϣ����
				strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (cMsgType == '0')//������Ϣ
			{
				msgType = 0;
				int posImages = strData.indexOf("images");//����������ַ�����һ�����ֵ�λ��
				//int imagesWidth = QString("images").length();
	
				//��ȡ��Ϣ,��ȡ���ұ�(������ ��:077)
				strMsg = strData.right(strData.length() - posImages - QString("images").length());
	
			}
			else if (cMsgType == '2')//�ļ���Ϣ
			{
				msgType = 2;
				int byteWidth = QString("bytes").length();//bytes����
				int posBytes = strData.indexOf("bytes");//bytes��λ�ã������Ӵ���һ�����ֵ�λ��
				int posData_begin = strData.indexOf("data_begin"); //data_begin��λ�ã������Ӵ���һ�����ֵ�λ��
				int data_beginWidth = QString("data_begin").length();
	
				//��ȡ�ļ����� ,ͨ��data_begin �� bytes ֮����ŵ� �������ļ�����
				QString fileName = strData.mid(posBytes + byteWidth, posData_begin - posBytes - byteWidth);
				gfileName = fileName;//����ȫ�ֱ���
	
				//��ȡ�ļ�����
				int dataLengthWidth;
				int posData = posData_begin + data_beginWidth;//�����ļ����ݵ�λ��
				strMsg = strData.mid(posData);//��ȡ����
				gfileData = strMsg;//����ȫ�ֱ���
	
				//����employeeID��ȡ����������
				QString senderName;
				int employeeID = strSendEmployeeID.toInt();
				//�鷢��������ͨ�������ݣ�id
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID =%1").arg(employeeID));
				querySenderName.exec();
				if (querySenderName.first()) //�鵽��
				{
					senderName = querySenderName.value(0).toString();//������
				}
	
				//Ⱥ���� �����ļ��ĺ�������
				ReceiveFile* recvFile = new ReceiveFile(this);
				//������˾ܾ�����ֱ�ӷ��ؼ���
				connect(recvFile, &ReceiveFile::refuseFile, [this]()
				{
					return;
				});
				QString msgLabel = QString::fromLocal8Bit("�յ�����") + senderName + QString::fromLocal8Bit("�������ļ����Ƿ���ܣ�");
				recvFile->setMsg(msgLabel);
				recvFile->show();//��ʾ
			}
	
		}
		else//����
		{
			//�õ������ߵ�qq��
			strRecevieEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);
			strWindowID = strSendEmployeeID;
	
			//���Ƿ����ҵ���Ϣ��������
			if (strRecevieEmployeeID != gLoginEmployeeID)
			{
				return;
			}
	
			//��ȡ��Ϣ������
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
			if (cMsgType == '1')//�ı���Ϣ
			{
				msgType = 1;
	
				//�ı���Ϣ����
				msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth, msgLengthWidth).toInt();
	
				//�ı���Ϣ
				strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (cMsgType == '0')//������Ϣ
			{
				msgType = 0;
				int posImages = strData.indexOf("images");//����images������ַ�����һ�����ֵ�λ��
				int imagesWidth = QString("images").length();
				//��ȡ��Ϣ,��ȡ���ұ�(������ ��:077)
				strMsg = strData.mid(posImages + imagesWidth);
			}
			else if (cMsgType == '2')//�ļ���Ϣ-->���ơ�����
			{
				msgType = 2;
				int bytesWidth = QString("bytes").length();//bytes����
				int posBytes = strData.indexOf("bytes");//bytes��λ�ã������Ӵ���һ�����ֵ�λ��
				int data_beginWidth = QString("data_begin").length();//data_begin����
				int posData_begin = strData.indexOf("data_begin");//data_begin��λ�ã������Ӵ���һ�����ֵ�λ��
	
	
				//��ȡ�ļ����� ,ͨ��data_begin �� bytes ֮����ŵ� �������ļ�����
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gfileName = fileName;//����ȫ�ֱ���
	
				//��ȡ�ļ�����
				int dataLengthWidth;
				int posData = posData_begin + data_beginWidth;//�����ļ����ݵ�λ��
				strMsg = strData.mid(posData_begin + data_beginWidth);//��ȡ����
				gfileData = strMsg;//����ȫ�ֱ���
	
				//����employeeID��ȡ����������
				QString senderName;
				int employeeID = strSendEmployeeID.toInt();
				//�鷢��������ͨ�������ݣ�id
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID =%1").arg(employeeID));
				querySenderName.exec();
				if (querySenderName.first()) //�鵽��
				{
					senderName = querySenderName.value(0).toString();//������
				}
	
				//�����д��� �����ļ��ĺ������� 
				ReceiveFile* recvFile = new ReceiveFile(this);
				//������˾ܾ�����ֱ�ӷ��ؼ���
				connect(recvFile, &ReceiveFile::refuseFile, [this]()
				{
					return;
				});
				QString msgLabel = QString::fromLocal8Bit("�յ�����") + senderName + QString::fromLocal8Bit("�������ļ����Ƿ���ܣ�");
				recvFile->setMsg(msgLabel);
				recvFile->show();//��ʾ
	
	
			}
	
		}
		//���ܵ�����Ϣ����ʾ����ҳ��,�����촰����Ϊ�����
		//�Ȼ�ȡ���ڵ�ַ
		QWidget* widget = WindowManager::getInstance()->findWindowName(strWindowID);
		if (widget)//���촰�ڴ���
		{
			this->setCurrentWidget(widget);
	
			//ͬ������������촰��
			QListWidgetItem* item = m_talkwindowItemMap.key(widget);
			item->setSelected(true);
		}
		else//���촰��δ��
		{
			return;
		}
	
		//�ļ���Ϣ��������
		if (msgType != 2)
		{
			int sendEmployeeID = strSendEmployeeID.toInt();
			handleReceivedMsg(sendEmployeeID, msgType, strMsg);//����Ϣ׷�ӵ���ҳ��
		}
	}
	

	//// ����һ���̳߳�����
	//PoolTask* pendingDataTask = new PoolTask;
	//// ����������̳߳���
	//QThreadPool::globalInstance()->start(pendingDataTask);
}