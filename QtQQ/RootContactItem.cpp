#include "RootContactItem.h"
#include<QPainter>

RootContactItem::RootContactItem(bool hasArrow ,QWidget *parent)
	: QLabel(parent)
	,m_rotation(0)
	,m_hasArrow(hasArrow)
{
	setFixedHeight(32);//�̶��߶�
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);//���ò��ԣ�UI��Ҳ�У�

	m_animation = new QPropertyAnimation(this, "rotation");//���Զ���,��ԽǶ�������
	m_animation->setDuration(50);//���õ��ζ���ʱ��50ms
	m_animation->setEasingCurve(QEasingCurve::InQuad);//���ñ仯������-->InQuad-->t��ƽ�� �����ĵ�!


}

RootContactItem::~RootContactItem()
{}


//�����ı�
void RootContactItem::setText(const QString & title)
{
	m_titleText = title;
	update();//���»���
}

//����չ��
void RootContactItem::setExpanded(bool expand)
{
	if (expand)
	{
		//��ʵ���������Զ����Ľ���ֵ
		m_animation->setEndValue(90);//����ֵ��90��������ת90�ȵ���˼(����)
	}
	else
	{
		m_animation->setEndValue(0);
	}
	m_animation->start();//��������

}

//�Ƕ�
int RootContactItem::rotation()
{
	return m_rotation;
}

//���ýǶ�
void RootContactItem::setRotation(int rotation)
{
	//�����Լ��ĽǶ�
	m_rotation = rotation;
	update();//���»���
}

//��д��ͼ�¼�
void RootContactItem::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::TextAntialiasing,true);//�ı������-->Ϊ�˺ÿ�

	//��������
	QFont font;
	font.setPointSize(10);
	painter.setFont(font);
	//�����ı�-->��ʵ��������Զ���ؼ��ı���
	painter.drawText(24, 0, width() - 24, height(), Qt::AlignLeft | Qt::AlignVCenter, m_titleText);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);//ƽ�������ͼƬ����
	painter.save();	//���滭������

	if (m_hasArrow)
	{
		QPixmap pixmap;
		pixmap.load(":/Resources/MainWindow/arrow.png");

		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);

		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform, true);

		//translate����ϵƫ�ƣ�x����ƫ�ƣ�y����ƫ�ƣ�
		p.translate(pixmap.width() / 2, pixmap.height() / 2);
		p.rotate(m_rotation);	//��ת����ϵ��˳ʱ�룩
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);//����
		//���϶��ǻ�����tmpPixmap��

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);
		painter.restore();	//�ָ���������--���ָ���save֮ǰ������
	}

	//Ĭ�ϴ���
	QLabel::paintEvent(event);

}
