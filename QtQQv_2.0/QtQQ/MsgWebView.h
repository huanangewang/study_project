#pragma once

#include <QWebEngineView>
#include <QDomNode>

//网络对象
class MsgHtmlObj :public QObject 
{
	Q_OBJECT
	
		//Q_PROPERTY动态属性，字符串  1.对方发的(msgLHtmlTmpl) 2.我自己发的信息  -->这里动态属性的使用不太懂
		//（类型 名称 类成员 类成员取名 信号 信号取名）
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl  NOTIFY signalMsgHtml) 
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl  NOTIFY signalMsgHtml)

public:
	MsgHtmlObj(QObject* parent);

signals:
	void signalMsgHtml(const QString& html);

private:
	void initHtmlTmpl();//初始化聊天网页
	QString getMsgTmplHtml(const QString& code);//传入一个字符串（网页），事先准备好的

private:
	QString m_msgLHtmlTmpl;//别人发来的信息
	QString m_msgRHtmlTmpl;//我发的信息
};

//网页-->接收的网页只能是 qrc中的
class MsgWebPage :public QWebEnginePage
{
	Q_OBJECT
public:
	MsgWebPage(QObject* parent = nullptr) :QWebEnginePage(parent) {}

	//重写
protected:
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame);//接收导航请求
};

//QWebEngineView视图，用于显示网页内容
class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
	void appendMsg(const QString& html);//追加信息

	//解析
private:
	QList <QStringList> parseHtml(const QString& html);//解析html,参数是html格式的字符串
	//Qt中所有DOM节点（属性、说明、文本等）都可以使用QDomNode表示
	QList<QStringList> parseDocNode(const QDomNode& node);//解析节点

private:
	MsgHtmlObj* m_msgHtmlObj;
};
