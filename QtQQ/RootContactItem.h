#pragma once

#include <QLabel>
#include <QPropertyAnimation>

class RootContactItem  : public QLabel
{
	Q_OBJECT

	//动态属性，箭头角度
	Q_PROPERTY(int rotation READ rotation WRITE setRotation)

public:
	//hasArrow 是否有箭头
	RootContactItem(bool hasArrow = true,QWidget *parent = nullptr);
	~RootContactItem();

public:
	void setText(const QString& title);//设置文本
	void setExpanded(bool expand); //设置展开

private:
	int rotation();//角度
	void setRotation(int rotation);//设置角度

protected:
	//重写绘图事件
	void paintEvent(QPaintEvent* event);

private:
	QString m_titleText;//显示的文本
	int m_rotation;		//箭头的角度
	bool m_hasArrow;	//是否有箭头
	//动画
	QPropertyAnimation* m_animation;

};
