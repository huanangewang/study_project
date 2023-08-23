#include "QClickLabel.h"
#include <QMouseEvent>

QClickLabel::QClickLabel(QWidget *parent)
	: QLabel(parent)
{
}

QClickLabel::~QClickLabel()
{
}

//重写，点击label，发射clicked信号
void QClickLabel::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit clicked();
	}
}
