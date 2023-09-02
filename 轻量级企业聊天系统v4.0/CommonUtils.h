#pragma once

#include <QProxyStyle>
#include <QPixmap>
#include <QSize>

//自定义类 目的: 改变默认的部件风格
class CustomProxyStyle :public QProxyStyle
{
public:
	CustomProxyStyle(QObject* parent) //将当前窗体作为父类
	{
		setParent(parent);
	}
	//重写
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
		QPainter* painter, const QWidget *widget = 0)const
	{
		if (PE_FrameFocusRect == element)
		{
			//去掉windows中部件默认的边框或虚线框，部件获取焦点时直接放回，不进行绘制
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

