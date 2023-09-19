#pragma once

#include <QWidget>
#include "TalkWindowShell.h"
#include "ui_TalkWindow.h"

class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent,const QString& uid/*,GroupType groupType*/);//参数1：父类，参数2：聊天的身份证号id，参数3：聊天分组类型
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);//添加表情图片
	void setWindowName(const QString& name);//设置窗口名字
	QString getTalkId(); //为了访问私有数据 m_talkid

private:
	void initControl();//初始化控件
	void initGroupTalkStatus();//初始化群聊状态
	int getComDepID(); //获取公司的id号

	void addPeopInfo(QTreeWidgetItem* pRootGroupItem,int employeeID);//添加员工信息

	//槽函数
private slots:
	void onFileOpenBtnClicked(bool);//文件打开按钮
	void onSendBtnClicked(bool);//发送信息
	void onItemDoubleClicked(QTreeWidgetItem* item,int column);//聊天窗口中的双击右侧处理

private:
	//void initCompanyTalk();	//初始化公司群聊天
	//void initPersonTalk();	//初始化人事部聊天
	//void initMarketTalk();	//初始化市场部聊天
	//void initDevelopTalk();	//初始化研发部聊天


	void initPtoPTalk();	//初始化单独聊天
	void initTalkWindow();	//初始化群聊

private:
	bool m_isGroupTalk;//是否为群聊
	Ui::TalkWindow ui;
	QString m_talkId;
	//GroupType m_groupType;
	QMap <QTreeWidgetItem*, QString> m_groupPerpleMap;//所有分组联系人姓名，做个映射

	//加个好友，为了TalkWindowShell，可以访问TalkWindow的私有数据
	friend class TalkWindowShell;
};
