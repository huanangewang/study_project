#pragma once

#include <QWidget>
#include "ui_TalkWindowItem.h"

//����ͷ������,�رհ�ť

class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = nullptr);
	~TalkWindowItem();

public:
	//void setHeadPixmap(const QString& pixmap);//����ͷ�� const QPixmap& pixmap
	void setHeadPixmap(const QPixmap& pixmap);//����ͷ��
	void setMsgLabelContent(const QString& msg);//���ñ�ǩ���ݣ��Ǻ��˻��ǹ�˾����(����ǩ��)
	QString getMsgLabelText();//��ȡ�ں�˭����

private:
	void initControl();//��ʼ���ؼ�

	//�ź�
signals:
	void signalCloseClicked();

	//�¼���д
private:
	void enterEvent(QEvent* event);//�����¼�
	void leaveEvent(QEvent* event);//�뿪�¼�
	void resizeEvent(QResizeEvent* event);//�ߴ�����

private:
	Ui::TalkWindowItemClass ui;
};
