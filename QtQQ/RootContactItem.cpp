#include "RootContactItem.h"
#include<QPainter>

RootContactItem::RootContactItem(bool hasArrow ,QWidget *parent)
	: QLabel(parent)
	,m_rotation(0)
	,m_hasArrow(hasArrow)
{
	setFixedHeight(32);//固定高度
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);//设置策略（UI中也有）

	m_animation = new QPropertyAnimation(this, "rotation");//属性动画,针对角度做动画
	m_animation->setDuration(50);//设置单次动画时长50ms
	m_animation->setEasingCurve(QEasingCurve::InQuad);//设置变化的曲线-->InQuad-->t的平方 帮助文档!


}

RootContactItem::~RootContactItem()
{}


//设置文本
void RootContactItem::setText(const QString & title)
{
	m_titleText = title;
	update();//重新绘制
}

//设置展开
void RootContactItem::setExpanded(bool expand)
{
	if (expand)
	{
		//其实是设置属性动画的结束值
		m_animation->setEndValue(90);//属性值是90，就是旋转90度的意思(动画)
	}
	else
	{
		m_animation->setEndValue(0);
	}
	m_animation->start();//启动动画

}

//角度
int RootContactItem::rotation()
{
	return m_rotation;
}

//设置角度
void RootContactItem::setRotation(int rotation)
{
	//更新自己的角度
	m_rotation = rotation;
	update();//重新绘制
}

//重写绘图事件
void RootContactItem::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::TextAntialiasing,true);//文本反锯齿-->为了好看

	//字体设置
	QFont font;
	font.setPointSize(10);
	painter.setFont(font);
	//绘制文本-->其实就是这个自定义控件的标题
	painter.drawText(24, 0, width() - 24, height(), Qt::AlignLeft | Qt::AlignVCenter, m_titleText);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);//平滑，提高图片质量
	painter.save();	//保存画家设置

	if (m_hasArrow)
	{
		QPixmap pixmap;
		pixmap.load(":/Resources/MainWindow/arrow.png");

		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);

		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform, true);

		//translate坐标系偏移（x方向偏移，y反向偏移）
		p.translate(pixmap.width() / 2, pixmap.height() / 2);
		p.rotate(m_rotation);	//旋转坐标系（顺时针）
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);//绘制
		//以上都是画在了tmpPixmap上

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);
		painter.restore();	//恢复画家设置--。恢复到save之前的设置
	}

	//默认处理
	QLabel::paintEvent(event);

}
