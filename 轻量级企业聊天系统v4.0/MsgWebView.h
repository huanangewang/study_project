#pragma once

#include <QWebEngineView>
#include <QDomNode>

class MsgHtmlObj :public QObject 
{
	Q_OBJECT

	//Q_PROPERTY��̬���ԣ��ַ���  1.�Է�����(msgLHtmlTmpl) 2.���Լ�������Ϣ  -->���ﶯ̬���Ե�ʹ�ò�̫��
	//������ ���� ���Ա ���Աȡ�� �ź� �ź�ȡ����
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)

public:
	//�Ӷ������(����Ϣ�����˵�ͷ��·��)
	MsgHtmlObj(QObject* parent,QString msgLPicPath = "");//����Ϣ�����˵�ͷ��·��

signals:
	void signalMsgHtml(const QString& html);

private:
	void initHtmlTmpl();//��ʼ��������ҳ
	QString getMsgTmplHtml(const QString& code);//����һ���ַ�������ҳ��������׼���õ�

private:
	QString m_msgLPicPath;//���ͷ��·��
	QString m_msgLHtmlTmpl;//���˷�������Ϣ
	QString m_msgRHtmlTmpl;//�ҷ�����Ϣ
};

//��ҳ-->���յ���ҳֻ���� qrc�е�
class MsgWebPage :public QWebEnginePage
{
	Q_OBJECT
public:
	MsgWebPage(QObject* parent = nullptr) :QWebEnginePage(parent) {}

protected://��д
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame);
};

//QWebEngineView��ͼ������ʾ��ҳ����
class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
	void appendMsg(const QString& html,QString strObj = "0");//׷����Ϣ

private:
	QList<QStringList> parseHtml(const QString& html);//����html
	//Qt������DOM�ڵ㣨���ԡ�˵�����ı��ȣ�������ʹ��QDomNode��ʾ
	QList<QStringList> parseDocNode(const QDomNode& node);//�����ڵ�

signals:
	void signalSendMsg(QString& strData, int &msgType, QString sFile = "");//������Ϣ�ź�
private:
	MsgHtmlObj* m_msgHtmlObj;
	QWebChannel* m_channel;//����ͨ��
};
