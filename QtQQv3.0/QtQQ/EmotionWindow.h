#pragma once

#include <QWidget>
#include "ui_EmotionWindow.h"
#include "EmotionLabelItem.h"


/*
2023-8-11
想实现一个，点击Label但又要实现表情动图的效果，这里要用到movei 的效果，
所以我们创建一个类(EmotionLabelItem)派生QClickLabel即可~~
*/

class EmotionWindow : public QWidget
{
	Q_OBJECT

public:
	EmotionWindow(QWidget *parent = nullptr);
	~EmotionWindow();

private:
	void initControl();//初始化控件

	//槽函数
private slots:
	void onAddEmotion(int emotionNum);//添加表情1,2,3....
	//void addEmotion(int emotionNum);//添加表情1,2,3....

	//信号
signals:
	void signalEmotionWindowHide(); //表情窗口隐藏
	void signalEmotionItemClicked(int emotionNum);//点击了哪个表情？

	//重绘
private:
	void paintEvent(QPaintEvent* event) override;

private:
	Ui::EmotionWindowClass ui;
};
