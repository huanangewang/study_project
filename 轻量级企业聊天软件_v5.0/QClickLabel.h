#pragma once

#include <QLabel>

//为了让标签label 能够实现类型按钮的功能，按下有cliecked信号
class QClickLabel : public QLabel
{
	Q_OBJECT

public:
	QClickLabel(QWidget *parent);
	~QClickLabel();

protected:
	//重写鼠标按下事件
	void mousePressEvent(QMouseEvent* event);

//信号
signals:
	void clicked();
};
