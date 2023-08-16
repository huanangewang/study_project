#pragma once

#include "basicwindow.h"
#include "ui_TalkWindowShell.h"
#include<qmap.h>

class TalkWindowClass;
class TalkWindow;
class TalkWindowItem;//左侧那些item,比如啥群，啥人
class EmotionWindow;

//枚举
enum GroupType {
	COMPANY = 0,	//公司群
	PERSONELGROUP,	//人事部
	DEVELOPMENTGROUP,//研发部
	MARKETGROUP,	//市场部
	PTOP			//同事之间单独聊天
};

//修改基类 BasicWindow
class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = nullptr);
	~TalkWindowShell();

	//设计聊天接口
public:
	void addTalkWindow(TalkWindow* talkWindow,TalkWindowItem* talkWindowItem,const QString& uid/*, GroupType grouptype*/);//添加聊天-->聊天窗口  ,参数2 ：你在和哪一项聊天
	void setCurrentWidget(QWidget* widget); //设置当前聊天的窗口(和哪个窗口进行聊天)
	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap() const; //为了在外部访问这个私有成员

private:
	void initControl();//初始化控件

	//聊天界面的槽函数设计
public slots:
	void onEmotionBtnClicked(bool);//表情按钮被点击

private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);//其实就是左边的列表部件
	void onEmotionItemClicked(int emotionNum);//选了哪个表情

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;//键 值对  打开的聊天窗口的映射(左边的列表，和打开的聊天窗口做一个映射，对应关系)
	EmotionWindow* m_emotionWindow;//表情窗口

};
