#pragma once

#include <QWidget>
#include "ui_EmotionWindow.h"
#include "EmotionLabelItem.h"


/*
2023-8-11
��ʵ��һ�������Label����Ҫʵ�ֱ��鶯ͼ��Ч��������Ҫ�õ�movei ��Ч����
�������Ǵ���һ����(EmotionLabelItem)����QClickLabel����~~
*/

class EmotionWindow : public QWidget
{
	Q_OBJECT

public:
	EmotionWindow(QWidget *parent = nullptr);
	~EmotionWindow();

private:
	void initControl();//��ʼ���ؼ�

	//�ۺ���
private slots:
	void onAddEmotion(int emotionNum);//��ӱ���1,2,3....
	//void addEmotion(int emotionNum);//��ӱ���1,2,3....

	//�ź�
signals:
	void signalEmotionWindowHide(); //���鴰������
	void signalEmotionItemClicked(int emotionNum);//������ĸ����飿

	//�ػ�
private:
	void paintEvent(QPaintEvent* event) override;

private:
	Ui::EmotionWindowClass ui;
};
