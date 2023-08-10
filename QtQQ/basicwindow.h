#pragma once

#include <QDialog>
#include "titlebar.h"

//窗体的外观，风格，事件的处理，初始化标题栏，点击事件的操作，鼠标位置变量的保存
class BasicWindow : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();

public:
	//加载样式表
	void loadStyleSheet(const QString &sheetName);

	//获取圆头像
	QPixmap getRoundImage(const QPixmap &src, QPixmap &mask, QSize masksize = QSize(0, 0));

private:
	void initBackGroundColor();	//初始化背景

protected:
	//事件
	void paintEvent(QPaintEvent*);				//绘制事件
	void mousePressEvent(QMouseEvent* event);	//鼠标事件
	void mouseMoveEvent(QMouseEvent* event);	//鼠标移动事件
	void mouseReleaseEvent(QMouseEvent*);		//鼠标松开事件

protected:
	void initTitleBar(ButtonType buttontype = MIN_BUTTON);
	void setTitleBarTitle(const QString& title, const QString& icon = ""); //设置标题栏内容 默认路径空

	//槽函数
public slots:
	void onShowClose(bool);
	void onShowMin(bool);
	void onShowHide(bool);
	void onShowNormal(bool);
	void onShowQuit(bool);
	void onSignalSkinChanged(const QColor &color);

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
	QPoint m_mousePoint;	//鼠标位置
	bool m_mousePressed;	//鼠标是否按下
	QColor m_colorBackGround;	//背景色
	QString m_styleName;	//样式文件名称
	TitleBar* _titleBar;	//标题栏--->在另一个类中实现了，这里用指针指向
};
