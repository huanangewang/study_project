#pragma once

#include <QDialog>
#include "titlebar.h"

//�������ۣ�����¼��Ĵ�����ʼ��������������¼��Ĳ��������λ�ñ����ı���
class BasicWindow : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();

public:
	//������ʽ��
	void loadStyleSheet(const QString &sheetName);

	//��ȡԲͷ��
	QPixmap getRoundImage(const QPixmap &src, QPixmap &mask, QSize masksize = QSize(0, 0));

private:
	void initBackGroundColor();	//��ʼ������

protected:
	//�¼�
	void paintEvent(QPaintEvent*);				//�����¼�
	void mousePressEvent(QMouseEvent* event);	//����¼�
	void mouseMoveEvent(QMouseEvent* event);	//����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent*);		//����ɿ��¼�

protected:
	void initTitleBar(ButtonType buttontype = MIN_BUTTON);
	void setTitleBarTitle(const QString& title, const QString& icon = ""); //���ñ��������� Ĭ��·����

	//�ۺ���
public slots:
	void onShowClose(bool);
	void onShowMin(bool);
	void onShowHide(bool);
	void onShowNormal(bool);
	void onShowQuit(bool);
	void onSignalSkinChanged(const QColor &color);

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
	QPoint m_mousePoint;	//���λ��
	bool m_mousePressed;	//����Ƿ���
	QColor m_colorBackGround;	//����ɫ
	QString m_styleName;	//��ʽ�ļ�����
	TitleBar* _titleBar;	//������--->����һ������ʵ���ˣ�������ָ��ָ��
};
