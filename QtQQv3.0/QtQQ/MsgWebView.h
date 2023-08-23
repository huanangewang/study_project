#pragma once

#include <QWebEngineView>
#include <QDomNode>

class MsgHtmlObj :public QObject 
{
	Q_OBJECT

	//Q_PROPERTY动态属性，字符串  1.对方发的(msgLHtmlTmpl) 2.我自己发的信息  -->这里动态属性的使用不太懂
	//（类型 名称 类成员 类成员取名 信号 信号取名）
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)

public:
	//加多个参数(发信息来的人的头像路径)
	MsgHtmlObj(QObject* parent,QString msgLPicPath = "");//发信息来的人的头像路径

signals:
	void signalMsgHtml(const QString& html);

private:
	void initHtmlTmpl();//初始化聊天网页
	QString getMsgTmplHtml(const QString& code);//传入一个字符串（网页），事先准备好的

private:
	QString m_msgLPicPath;//左侧头像路径
	QString m_msgLHtmlTmpl;//别人发来的信息
	QString m_msgRHtmlTmpl;//我发的信息
};

//网页-->接收的网页只能是 qrc中的
class MsgWebPage :public QWebEnginePage
{
	Q_OBJECT
public:
	MsgWebPage(QObject* parent = nullptr) :QWebEnginePage(parent) {}

protected://重写
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame);
};

//QWebEngineView视图用于显示网页内容
class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
	void appendMsg(const QString& html,QString strObj = "0");//追加信息

private:
	QList<QStringList> parseHtml(const QString& html);//解析html
	//Qt中所有DOM节点（属性、说明、文本等）都可以使用QDomNode表示
	QList<QStringList> parseDocNode(const QDomNode& node);//解析节点

signals:
	void signalSendMsg(QString& strData, int &msgType, QString sFile = "");//发送信息信号
private:
	MsgHtmlObj* m_msgHtmlObj;
	QWebChannel* m_channel;//网络通道
};
