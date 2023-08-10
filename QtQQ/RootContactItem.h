#pragma once

#include <QLabel>
#include <QPropertyAnimation>

class RootContactItem  : public QLabel
{
	Q_OBJECT

	//��̬���ԣ���ͷ�Ƕ�
	Q_PROPERTY(int rotation READ rotation WRITE setRotation)

public:
	//hasArrow �Ƿ��м�ͷ
	RootContactItem(bool hasArrow = true,QWidget *parent = nullptr);
	~RootContactItem();

public:
	void setText(const QString& title);//�����ı�
	void setExpanded(bool expand); //����չ��

private:
	int rotation();//�Ƕ�
	void setRotation(int rotation);//���ýǶ�

protected:
	//��д��ͼ�¼�
	void paintEvent(QPaintEvent* event);

private:
	QString m_titleText;//��ʾ���ı�
	int m_rotation;		//��ͷ�ĽǶ�
	bool m_hasArrow;	//�Ƿ��м�ͷ
	//����
	QPropertyAnimation* m_animation;

};
