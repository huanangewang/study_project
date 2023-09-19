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

	void setEmotionName(int emotionName);//设置表情名称

	//信号
signals:
	void emotionClicked(int emotionNum);

private:
	void initControl();//初始化控件

private:
	int m_emotionName;
	QMovie* m_apngMovie;//用来显示动图
};
