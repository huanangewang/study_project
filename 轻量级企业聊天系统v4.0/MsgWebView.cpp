#include "MsgWebView.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>
#include "TalkWindowShell.h"
#include "WindowManager.h"
#include <QSqlQueryModel>

extern QString gstrLoginHeadPath;

MsgHtmlObj::MsgHtmlObj(QObject* parent, QString msgLPicPath) :QObject(parent)
{
	m_msgLPicPath = msgLPicPath;//��ʼ�����ͷ��
	initHtmlTmpl();//��ʼ����ҳ
}

//��ʼ����ҳ--> Ҫ�������%1 �滻�ɶ�Ӧ��ͷ��
void MsgHtmlObj::initHtmlTmpl()
{
	//��
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");//msgrightTmpl �� code
	m_msgLHtmlTmpl.replace("%1", m_msgLPicPath);//�滻�����ͷ���·��

	//��
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
	m_msgRHtmlTmpl.replace("%1", gstrLoginHeadPath);//��CCMain�е�ȫ�ֱ����洢��¼�ߵ�ͷ��
}


//����һ���ַ�������ҳ��������׼���õ�
//��ȡ��Ϣ��ֱ��ȫ������
QString MsgHtmlObj::getMsgTmplHtml(const QString& code)
{
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;
	if (file.isOpen())
	{
		strData = QLatin1String(file.readAll());//ȫ������,Ҫת��QLatin1String  
	}
	else 
	{
		QMessageBox::information(nullptr, "Tips", "Failed to init html!");
		return QString("");
	}
	file.close();
	return strData;
}

//���յ������� url,��ʵ���������Ƿ���html��·������������·�������ܽ���qrc:/*.html
bool MsgWebPage::acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//������qrc:/*.html
	if (url.scheme() == QString("qrc"))//�ж�url����
		return true;
	return false;
}

//���촰�ڹ����ʱ�򣬲Ż���й���MsgWebView
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
	,m_channel(new QWebChannel(this))//��ʼ��һ������ͨ������,ͨ������
{
	MsgWebPage* page = new MsgWebPage(this);//��ҳ����
	setPage(page);

	m_msgHtmlObj = new MsgHtmlObj(this);
	m_channel->registerObject("external0", m_msgHtmlObj);//ע��ͨ��,�ұߵķ���Ϣ����

	TalkWindowShell* talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowShell::updateSendTcpMsg);//�����Ƿ����źŵ�ʱ��TalkWindowShell���ͽ����������д����
	
	//��ǰ�����������촰�ڵ�ID��QQ�ţ�
	QString strTalkId = WindowManager::getInstance()->getCreatingTalkId();

	//�������Ⱥ�ģ�����
	QSqlQueryModel queryEmpolyeeModel;
	QString strEmployeeID, strPicturePath;//���ڴ��Ա��id��Ա��ͷ��
	QString strExternal;
	bool isGroupTalk = false;

	//��ȡ��˾ȺID
	queryEmpolyeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
		.arg(QStringLiteral("��˾Ⱥ")));
	QModelIndex companyIndex = queryEmpolyeeModel.index(0, 0);
	QString strCompanyID = queryEmpolyeeModel.data(companyIndex).toString();//�õ���˾Ⱥid��2000

	if (strTalkId == strCompanyID)//��˾Ⱥ��,��ô״̬Ϊ1 ��Ա������Ҫ�ó���(id,ͷ��)
	{
		isGroupTalk = true;
		queryEmpolyeeModel.setQuery("SELECT employeeID,picture FROM tab_employees WHERE status = 1");
	}
	else 
	{
		//ͨ���ж�id�ĳ��ȣ������ǲ�����ͨȺ�ģ����ǵ���
		if (strTalkId.length() == 4)//����Ⱥ��
		{
			isGroupTalk = true;
			//��ȡĳȺ��״̬Ϊ1 ������Ա����ͷ���id
			queryEmpolyeeModel.setQuery(QString("SELECT employeeID,picture FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(strTalkId));
		}
		else//��������
		{
			//��ȡĳ�ˣ�id��ͷ��
			queryEmpolyeeModel.setQuery(QString("SELECT picture FROM tab_employees WHERE status = 1 AND employeeID = %1").arg(strTalkId));
			
			QModelIndex index = queryEmpolyeeModel.index(0, 0);
			strPicturePath = queryEmpolyeeModel.data(index).toString();

			strExternal = "external_" + strTalkId;
			MsgHtmlObj *msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}

	if (isGroupTalk)//Ⱥ�ģ�����Ҫ������ȡ,�ó��������ȵ����鷳һЩ
	{
		QModelIndex employeeModelIndex, pictureModelIndex;
		int rows = queryEmpolyeeModel.rowCount();//���м�����¼��
		for (int i = 0; i < rows; i++)
		{
			employeeModelIndex = queryEmpolyeeModel.index(i, 0);//�õ�ĳ��id����
			pictureModelIndex = queryEmpolyeeModel.index(i, 1);//�õ�ĳ��ͷ��·������

			strEmployeeID = queryEmpolyeeModel.data(employeeModelIndex).toString();//�õ�ĳ��id
			strPicturePath = queryEmpolyeeModel.data(pictureModelIndex).toString();//�õ�ĳ��ͷ��·��

			strExternal = "external_" + strEmployeeID;

			MsgHtmlObj* msgHtmlObj = new MsgHtmlObj(this, strPicturePath);//��������(���ף�ͷ��)
			m_channel->registerObject(strExternal, msgHtmlObj);//ע��(id,����)
		}
	}

	this->page()->setWebChannel(m_channel);
	//������ҳ��Ϣ,��ʼ������Ϣ��ҳҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{
}


//׷����Ϣ // external0   , external_xxx(������������������java�ű���ʱ��Ҫ�ӵ㶫��)
//2023-08-18---->��׷����Ϣ��ʱ�򣬸����ǽ�����Ϣ���Ƿ�����Ϣ�������˵���
void MsgWebView::appendMsg(const QString & html, QString strObj)
{
	QJsonObject msgObj;
	QString qsMsg;//Ҫ�ӵ���Ϣ
	const QList<QStringList> msgLst = parseHtml(html);//����html

	/*
		��Ϣ���ͣ�0������     1���ı�     2���ļ�
		���������55 ,�Ǿ�055 ,�����8,�Ǿ�008
		��Ҫ����ȡ�������ƣ����ּ���
	*/

	int imageNum = 0;
	int msgType = 1;//��Ϣ���ͣ�0�Ǳ��� 1�ı� 2�ļ�
	bool isImageMsg = false;
	QString strData;//���͵�����
	

	for (int i = 0; i < msgLst.size(); i++)
	{
		if (msgLst.at(i).at(0) == "img")//��ǰѭ��������ĵ�һ��Ԫ����ͼƬ
		{
			QString imagePath = msgLst.at(i).at(1);
			//�п��ܼ���qrc��Ҳ�п���û�� 1."qrc:/MainWindow/..../77.png" 2.":/MainWindow/.../77.png"
			QPixmap pixmap;

			//��ȡ�������Ƶ�λ��(���ּ�)
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";//���磺 qrc::/Resources/MainWindow/emotion/111.png
			int pos = strEmotionPath.size();
			isImageMsg = true;

			//��ȡ��������
			QString strEmotionName = imagePath.mid(pos);//��ȡ��qrc::/Resources/MainWindow/emotion/����-->��Ҳ��Ҫ.png-->������Ϊ��Ҫ����
			strEmotionName.replace(".png", "");//��.png�ÿ��滻-->���ھͻ���˱�������

			int emotionNameL = strEmotionName.length();//���м�λ,���ݱ������Ƴ��ȣ��������ñ������ݣ�������λ������0������λ
			if (emotionNameL == 1)
			{
				strData = strData + "00" + strEmotionName;
			}
			else if (emotionNameL == 2)
			{
				strData = strData + "0" + strEmotionName;
			}
			else if (emotionNameL == 3)
			{
				strData = strData + strEmotionName;
			}

			msgType = 0;//������Ϣ
			imageNum++;//�����������һ��

			if (imagePath.left(3) == "qrc")//����Ҫ�ж���û��qrc���ж�����ߵ������ַ��ǲ���qrc
			{
				pixmap.load(imagePath.mid(3));//ȥ������·����qrc
			}
			else
			{
				pixmap.load(imagePath);
			}

			//����ͼƬhtml��ʽ�ı���ϣ�����Ҫ��·������ȣ��߶�
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());
			qsMsg += imgPath;
		}
		else if (msgLst.at(i).at(0) == "text")//��ǰѭ��������ĵ�һ��Ԫ�����ı�
		{
			qsMsg += msgLst.at(i).at(1);
			strData = qsMsg;//ֱ�ӽ�Ҫ��ӵ���Ϣ��ֱ�Ӹ� ���Ǵ�����������ݾ���
		}
	}

	msgObj.insert("MSG", qsMsg);//�����ֵ��Qjson������

	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);//����ģʽCompact
	if (strObj == "0")//����Ϣ(�Լ��Ҳ�)
	{
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));//��ȡ��ǰ��ҳ��������
		
		if (isImageMsg)
		{
			//strData�������Ǳ�������ƺ���ͨ��Ϣ-->���ڼ��ϱ�������+ image+ ֮ǰ���������
			strData = QString::number(imageNum) + "images" + strData;
		}

		emit signalSendMsg(strData, msgType); //�����Զ����źţ�Я��������Ҫ�����ݣ�1������õ����ݣ�����������ı���2����Ϣ���ͣ�
	}
	else//������Ϣ     --->ֻ��Ҫ����׷��
	{
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}
}

//����html,������html��ʽ���ַ���
QList<QStringList> MsgWebView::parseHtml(const QString & html)
{
	QDomDocument doc;
	doc.setContent(html);//ת��Qt�ڵ��ĵ�-->Ϊ�˽���html���body
	const QDomElement& root = doc.documentElement();//�ڵ�Ԫ��
	const QDomNode& node = root.firstChildElement("body");
	return parseDocNode(node);//�Խڵ���н���
}

//�Խڵ���н���
QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node)
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();//���������ӽڵ�

	for (int i = 0; i < list.count(); i++)
	{
		const QDomNode& node = list.at(i);//��������ѭ�����Ľڵ�
		
		
		if (node.isElement())//����ڵ���Ԫ��
		{
			//�ڵ�ת��Ԫ��
			const QDomElement& element = node.toElement();
			if (element.tagName() == "img")
			{
				QStringList attributeList;//��������
				attributeList << "img" << element.attribute("src");//Ϊ���õ�ͼƬ·��
				attribute << attributeList;
			}

			if (element.tagName() == "span")
			{
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;
			}

			if (node.hasChildNodes())//��������ӽڵ�
			{
				attribute << parseDocNode(node);//�ݹ�
			}
		}
	}

	return attribute;
}
