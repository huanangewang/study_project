#pragma once

#include <QProxyStyle>
#include <QPixmap>
#include <QSize>

//�Զ����� Ŀ��: �ı�Ĭ�ϵĲ������
class CustomProxyStyle :public QProxyStyle
{
public:
	CustomProxyStyle(QObject* parent) //����ǰ������Ϊ����
	{
		setParent(parent);
	}
	//��д
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
		QPainter* painter, const QWidget *widget = 0)const
	{
		if (PE_FrameFocusRect == element)
		{
			//ȥ��windows�в���Ĭ�ϵı߿�����߿򣬲�����ȡ����ʱֱ�ӷŻأ������л���
			return;
		}
		else
		{
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};


class CommonUtils
{
public:
	CommonUtils();
public:
	static QPixmap getRoundImage(const QPixmap &src, QPixmap &mask, QSize masksize = QSize(0, 0));
	static void loadStyleSheet(QWidget* widget, const QString &sheetName);
	static void setDefaultSkinColor(const QColor &color);
	static QColor getDefaultSkinColor();
};

