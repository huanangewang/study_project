#pragma once

#include <QObject>
#include "TalkWindowShell.h"


class WindowManager : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();

public:
	QWidget* findWindowName(const QString& qsWindowName);//qsWindowName��������ʱ���id��Ҳ����item��data
	void deleteWindowName(const QString& qsWindowName);
	void addWindowName(const QString& qsWindowName, QWidget* qWidget);
	
	static WindowManager* getInstance();//���� -->һ��Ҫstatic
	TalkWindowShell* getTalkWindowShell();//Ϊ�˷���˽������
	QString getCreatingTalkId(); //��ȡ���ڴ��������촰��id

	//������촰��,����1�����ڵ����ƣ�Ψһ��������2��������촰�����ͣ�����3�������ǵ�������
	void addNewTalkWindow(const QString&uid);

private:
	TalkWindowShell* m_talkwindowshell;
	QMap<QString, QWidget*> m_windowMap;//id�ʹ���Ҫӳ��(UserRole+1��ֵ)
	QString m_strCreatingTalkId = "";//���ڴ��������촰�ڣ�QQ�ţ�
};
