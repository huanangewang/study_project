#pragma once

#include "QClickLabel.h"
#include <QMovie>

class QMovie;

class EmotionLabelItem  : public QClickLabel
{
	Q_OBJECT

public:
	EmotionLabelItem(QWidget *parent);
	~EmotionLabelItem();

	void setEmotionName(int emotionName);//���ñ�������

	//�ź�
signals:
	void emotionClicked(int emotionNum);

private:
	void initControl();//��ʼ���ؼ�

private:
	int m_emotionName;
	QMovie* m_apngMovie;//������ʾ��ͼ
};
