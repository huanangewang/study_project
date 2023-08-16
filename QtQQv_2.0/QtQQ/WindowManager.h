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
	QWidget* findWindowName(const QString& qsWindowName);//qsWindowName创建窗口时候的id，也就是item的data
	void deleteWindowName(const QString& qsWindowName);
	void addWindowName(const QString& qsWindowName, QWidget* qWidget);
	
	static WindowManager* getInstance();//单例 -->一定要static
	//添加聊天窗口,参数1：窗口的名称（唯一），参数2：添加聊天窗口类型，参数3：可能是单人聊天
	void addNewTalkWindow(const QString& uid/*, GroupType groupType = COMPANY, const QString& strPeople = ""*/);

	//数据
private:
	TalkWindowShell* m_talkwindowshell;
	QMap<QString, QWidget*> m_windowMap;//id和窗口要映射(UserRole+1的值)
};
