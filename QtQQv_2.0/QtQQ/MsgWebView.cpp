#include "MsgWebView.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>

MsgHtmlObj::MsgHtmlObj(QObject* parent) :QObject(parent)
{
	initHtmlTmpl();//��ʼ����ҳ
}

//��ʼ����ҳ
void MsgHtmlObj::initHtmlTmpl()
{
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");//msgrightTmpl �� code
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
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

MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
{
	//��ҳ����
	MsgWebPage* page = new MsgWebPage(this);
	setPage(page);

	//ͨ������
	QWebChannel* channel = new QWebChannel(this);
	m_msgHtmlObj = new MsgHtmlObj(this);
	channel->registerObject("external", m_msgHtmlObj);//ע��ͨ��
	this->page()->setWebChannel(channel);

	//������ҳ��Ϣ,��ʼ������Ϣ��ҳҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{
}

//׷����Ϣ
void MsgWebView::appendMsg(const QString & html)
{
	QJsonObject msgObj;
	QString qsMsg;//Ҫ�ӵ���Ϣ
	const QList<QStringList> msgLst = parseHtml(html);//����html

	for (int i = 0; i < msgLst.size(); i++)
	{
		if (msgLst.at(i).at(0) == "img")//��ǰѭ��������ĵ�һ��Ԫ����ͼƬ
		{
			QString imagePath = msgLst.at(i).at(1);
			//�п��ܼ���qrc��Ҳ�п���û�� 1."qrc:/MainWindow/..../77.png" 2.":/MainWindow/.../77.png"
			QPixmap pixmap;
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
		}
	}

	msgObj.insert("MSG", qsMsg);//�����ֵ��Qjson������


	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);//����ģʽCompact
	this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));//��ȡ��ǰ��ҳ��������
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
