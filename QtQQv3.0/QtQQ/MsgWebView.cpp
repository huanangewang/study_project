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
	m_msgLPicPath = msgLPicPath;//初始化左侧头像
	initHtmlTmpl();//初始化网页
}

//初始化网页--> 要将里面的%1 替换成对应的头像
void MsgHtmlObj::initHtmlTmpl()
{
	//左
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");//msgrightTmpl 是 code
	m_msgLHtmlTmpl.replace("%1", m_msgLPicPath);//替换成左侧头像的路径

	//右
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
	m_msgRHtmlTmpl.replace("%1", gstrLoginHeadPath);//在CCMain中的全局变量存储登录者的头像
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

//聊天窗口构造的时候，才会进行构造MsgWebView
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
	,m_channel(new QWebChannel(this))//初始化一个网络通道对象,通道创建
{
	MsgWebPage* page = new MsgWebPage(this);//网页创建
	setPage(page);

	m_msgHtmlObj = new MsgHtmlObj(this);
	m_channel->registerObject("external0", m_msgHtmlObj);//注册通道,右边的发信息对象

	TalkWindowShell* talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowShell::updateSendTcpMsg);//当我们发射信号的时候，TalkWindowShell，就进行向服务器写数据
	
	//当前正构建的聊天窗口的ID（QQ号）
	QString strTalkId = WindowManager::getInstance()->getCreatingTalkId();

	//分情况，群聊，单聊
	QSqlQueryModel queryEmpolyeeModel;
	QString strEmployeeID, strPicturePath;//用于存放员工id和员工头像
	QString strExternal;
	bool isGroupTalk = false;

	//获取公司群ID
	queryEmpolyeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
		.arg(QStringLiteral("公司群")));
	QModelIndex companyIndex = queryEmpolyeeModel.index(0, 0);
	QString strCompanyID = queryEmpolyeeModel.data(companyIndex).toString();//拿到公司群id号2000

	if (strTalkId == strCompanyID)//公司群聊,那么状态为1 的员工，都要拿出来(id,头像)
	{
		isGroupTalk = true;
		queryEmpolyeeModel.setQuery("SELECT employeeID,picture FROM tab_employees WHERE status = 1");
	}
	else 
	{
		//通过判断id的长度，来看是不是普通群聊，还是单聊
		if (strTalkId.length() == 4)//其他群聊
		{
			isGroupTalk = true;
			//获取某群，状态为1 的所有员工的头像和id
			queryEmpolyeeModel.setQuery(QString("SELECT employeeID,picture FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(strTalkId));
		}
		else//单独聊天
		{
			//获取某人，id，头像
			queryEmpolyeeModel.setQuery(QString("SELECT picture FROM tab_employees WHERE status = 1 AND employeeID = %1").arg(strTalkId));
			
			QModelIndex index = queryEmpolyeeModel.index(0, 0);
			strPicturePath = queryEmpolyeeModel.data(index).toString();

			strExternal = "external_" + strTalkId;
			MsgHtmlObj *msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}

	if (isGroupTalk)//群聊，我们要挨个获取,拿出来处理，比单聊麻烦一些
	{
		QModelIndex employeeModelIndex, pictureModelIndex;
		int rows = queryEmpolyeeModel.rowCount();//看有几条记录？
		for (int i = 0; i < rows; i++)
		{
			employeeModelIndex = queryEmpolyeeModel.index(i, 0);//拿到某人id索引
			pictureModelIndex = queryEmpolyeeModel.index(i, 1);//拿到某人头像路径索引

			strEmployeeID = queryEmpolyeeModel.data(employeeModelIndex).toString();//拿到某人id
			strPicturePath = queryEmpolyeeModel.data(pictureModelIndex).toString();//拿到某人头像路径

			strExternal = "external_" + strEmployeeID;

			MsgHtmlObj* msgHtmlObj = new MsgHtmlObj(this, strPicturePath);//创建对象(父亲，头像)
			m_channel->registerObject(strExternal, msgHtmlObj);//注册(id,对象)
		}
	}

	this->page()->setWebChannel(m_channel);
	//加载网页信息,初始化收信息网页页面
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{
}


//追加信息 // external0   , external_xxx(如果是这个，那再运行java脚本的时候，要加点东西)
//2023-08-18---->在追加信息的时候，根据是接收信息还是发送信息，进行了调整
void MsgWebView::appendMsg(const QString & html, QString strObj)
{
	QJsonObject msgObj;
	QString qsMsg;//要加的信息
	const QList<QStringList> msgLst = parseHtml(html);//解析html

	/*
		信息类型：0：表情     1：文本     2：文件
		如果表情是55 ,那就055 ,如果是8,那就008
		主要：获取表情名称（数字几）
	*/

	int imageNum = 0;
	int msgType = 1;//信息类型：0是表情 1文本 2文件
	bool isImageMsg = false;
	QString strData;//发送的数据
	

	for (int i = 0; i < msgLst.size(); i++)
	{
		if (msgLst.at(i).at(0) == "img")//当前循环的链表的第一个元素是图片
		{
			QString imagePath = msgLst.at(i).at(1);
			//有可能加了qrc，也有可能没加 1."qrc:/MainWindow/..../77.png" 2.":/MainWindow/.../77.png"
			QPixmap pixmap;

			//获取表情名称的位置(数字几)
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";//比如： qrc::/Resources/MainWindow/emotion/111.png
			int pos = strEmotionPath.size();
			isImageMsg = true;

			//获取表情名称
			QString strEmotionName = imagePath.mid(pos);//截取到qrc::/Resources/MainWindow/emotion/后面-->但也不要.png-->我们是为了要名称
			strEmotionName.replace(".png", "");//将.png用空替换-->现在就获得了表情名称

			int emotionNameL = strEmotionName.length();//看有几位,根据表情名称长度，进行设置表情数据，不足三位，则用0补足三位
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

			msgType = 0;//表情信息
			imageNum++;//表情个数增加一个

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
			strData = qsMsg;//直接将要添加的信息，直接给 我们打包发出的数据就行
		}
	}

	msgObj.insert("MSG", qsMsg);//插入键值在Qjson对象中

	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);//紧凑模式Compact
	if (strObj == "0")//发信息(自己右侧)
	{
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));//获取当前网页，跑起来
		
		if (isImageMsg)
		{
			//strData里现在是表情的名称和普通信息-->现在加上表情数量+ image+ 之前表情的数据
			strData = QString::number(imageNum) + "images" + strData;
		}

		emit signalSendMsg(strData, msgType); //发送自定义信号（携带我们需要的数据，1、打包好的数据（包括表情和文本）2、信息类型）
	}
	else//接收消息     --->只需要进行追加
	{
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}
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
	const QDomNodeList& list = node.childNodes();//返回左右子节点

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
