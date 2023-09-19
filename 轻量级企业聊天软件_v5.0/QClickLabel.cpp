#include "QClickLabel.h"
#include <QMouseEvent>

QClickLabel::QClickLabel(QWidget *parent)
	: QLabel(parent)
{
}

QClickLabel::~QClickLabel()
{
}

//��д�����label������clicked�ź�
void QClickLabel::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit clicked();
	}
}
