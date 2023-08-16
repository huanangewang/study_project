#include "MsgWebView.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>

MsgHtmlObj::MsgHtmlObj(QObject* parent) :QObject(parent)
{
	initHtmlTmpl();//初始化网页
}

//初始化网页
void MsgHtmlObj::initHtmlTmpl()
{
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");//msgrightTmpl 是 code
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
}

//传入一个字符串（网页），事先准备好的
//获取信息，直接全读出来
QString MsgHtmlObj::getMsgTmplHtml(const QString& code)
{
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;
	if (file.isOpen())
	{
		strData = QLatin1String(file.readAll());//全读出来,要转成QLatin1String  
	}
	else 
	{
		QMessageBox::information(nullptr, "Tips", "Failed to init html!");
		return QString("");
	}
	file.close();
	return strData;
}

//接收导航请求 url,其实我们这里是放了html的路径，进行限制路径，仅能接收qrc:/*.html
bool MsgWebPage::acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//仅接受qrc:/*.html
	if (url.scheme() == QString("qrc"))//判断url类型
		return true;
	return false;
}

MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
{
	//网页创建
	MsgWebPage* page = new MsgWebPage(this);
	setPage(page);

	//通道创建
	QWebChannel* channel = new QWebChannel(this);
	m_msgHtmlObj = new MsgHtmlObj(this);
	channel->registerObject("external", m_msgHtmlObj);//注册通道
	this->page()->setWebChannel(channel);

	//加载网页信息,初始化收信息网页页面
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{
}

//追加信息
void MsgWebView::appendMsg(const QString & html)
{
	QJsonObject msgObj;
	QString qsMsg;//要加的信息
	const QList<QStringList> msgLst = parseHtml(html);//解析html

	for (int i = 0; i < msgLst.size(); i++)
	{
		if (msgLst.at(i).at(0) == "img")//当前循环的链表的第一个元素是图片
		{
			QString imagePath = msgLst.at(i).at(1);
			//有可能加了qrc，也有可能没加 1."qrc:/MainWindow/..../77.png" 2.":/MainWindow/.../77.png"
			QPixmap pixmap;
			if (imagePath.left(3) == "qrc")//所以要判断有没有qrc，判断最左边的三个字符是不是qrc
			{
				pixmap.load(imagePath.mid(3));//去掉表情路径中qrc
			}
			else
			{
				pixmap.load(imagePath);
			}

			//表情图片html格式文本组合，表情要，路径，宽度，高度
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());
			qsMsg += imgPath;
		}
		else if (msgLst.at(i).at(0) == "text")//当前循环的链表的第一个元素是文本
		{
			qsMsg += msgLst.at(i).at(1);
		}
	}

	msgObj.insert("MSG", qsMsg);//插入键值在Qjson对象中


	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);//紧凑模式Compact
	this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));//获取当前网页，跑起来
}

//解析html,参数是html格式的字符串
QList<QStringList> MsgWebView::parseHtml(const QString & html)
{
	QDomDocument doc;
	doc.setContent(html);//转成Qt节点文档-->为了解析html里的body
	const QDomElement& root = doc.documentElement();//节点元素
	const QDomNode& node = root.firstChildElement("body");
	return parseDocNode(node);//对节点进行解析
}

//对节点进行解析
QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node)
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();//返回所有子节点

	for (int i = 0; i < list.count(); i++)
	{
		const QDomNode& node = list.at(i);//保存链表循环到的节点
		
		if (node.isElement())//如果节点是元素
		{
			//节点转换元素
			const QDomElement& element = node.toElement();
			if (element.tagName() == "img")
			{
				QStringList attributeList;//属性链表
				attributeList << "img" << element.attribute("src");//为了拿到图片路径
				attribute << attributeList;
			}

			if (element.tagName() == "span")
			{
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;
			}

			if (node.hasChildNodes())//如果还有子节点
			{
				attribute << parseDocNode(node);//递归
			}
		}
	}

	return attribute;
}
