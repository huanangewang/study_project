#pragma once

#include <QWebEngineView>
#include <QDomNode>

//�������
class MsgHtmlObj :public QObject 
{
	Q_OBJECT
	
		//Q_PROPERTY��̬���ԣ��ַ���  1.�Է�����(msgLHtmlTmpl) 2.���Լ�������Ϣ  -->���ﶯ̬���Ե�ʹ�ò�̫��
		//������ ���� ���Ա ���Աȡ�� �ź� �ź�ȡ����
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl  NOTIFY signalMsgHtml) 
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl  NOTIFY signalMsgHtml)

public:
	MsgHtmlObj(QObject* parent);

signals:
	void signalMsgHtml(const QString& html);

private:
	void initHtmlTmpl();//��ʼ��������ҳ
	QString getMsgTmplHtml(const QString& code);//����һ���ַ�������ҳ��������׼���õ�

private:
	QString m_msgLHtmlTmpl;//���˷�������Ϣ
	QString m_msgRHtmlTmpl;//�ҷ�����Ϣ
};

//��ҳ-->���յ���ҳֻ���� qrc�е�
class MsgWebPage :public QWebEnginePage
{
	Q_OBJECT
public:
	MsgWebPage(QObject* parent = nullptr) :QWebEnginePage(parent) {}

	//��д
protected:
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame);//���յ�������
};

//QWebEngineView��ͼ��������ʾ��ҳ����
class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
	void appendMsg(const QString& html);//׷����Ϣ

	//����
private:
	QList <QStringList> parseHtml(const QString& html);//����html,������html��ʽ���ַ���
	//Qt������DOM�ڵ㣨���ԡ�˵�����ı��ȣ�������ʹ��QDomNode��ʾ
	QList<QStringList> parseDocNode(const QDomNode& node);//�����ڵ�

private:
	MsgHtmlObj* m_msgHtmlObj;
};
