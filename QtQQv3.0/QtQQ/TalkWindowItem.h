#pragma once

#include <QWidget>
#include "ui_TalkWindowItem.h"

//设置头像，人名,关闭按钮

class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = nullptr);
	~TalkWindowItem();

public:
	//void setHeadPixmap(const QString& pixmap);//设置头像 const QPixmap& pixmap
	void setHeadPixmap(const QPixmap& pixmap);//设置头像
	void setMsgLabelContent(const QString& msg);//设置标签内容，是和人还是公司聊天(个性签名)
	QString getMsgLabelText();//获取在和谁聊天

private:
	void initControl();//初始化控件

	//信号
signals:
	void signalCloseClicked();

	//事件重写
private:
	void enterEvent(QEvent* event);//进入事件
	void leaveEvent(QEvent* event);//离开事件
	void resizeEvent(QResizeEvent* event);//尺寸重置

private:
	Ui::TalkWindowItemClass ui;
};
