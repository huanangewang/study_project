#pragma once

#include <QLabel>

//Ϊ���ñ�ǩlabel �ܹ�ʵ�����Ͱ�ť�Ĺ��ܣ�������cliecked�ź�
class QClickLabel : public QLabel
{
	Q_OBJECT

public:
	QClickLabel(QWidget *parent);
	~QClickLabel();

protected:
	//��д��갴���¼�
	void mousePressEvent(QMouseEvent* event);

//�ź�
signals:
	void clicked();
};
