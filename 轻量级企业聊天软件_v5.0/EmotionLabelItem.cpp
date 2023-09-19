#include "EmotionLabelItem.h"

EmotionLabelItem::EmotionLabelItem(QWidget *parent)
	: QClickLabel(parent)
{
	initControl();
	connect(this, &QClickLabel::clicked, [this]() {
		emit emotionClicked(m_emotionName);//发送表情点击信号即可
	});
}

EmotionLabelItem::~EmotionLabelItem()
{}

//设置表情名称
void EmotionLabelItem::setEmotionName(int emotionName)
{
	m_emotionName = emotionName;
	QString imageName = QString(":/Resources/MainWindow/emotion/%1.png").arg(emotionName);//按数字

	m_apngMovie = new QMovie(imageName,"apng",this);//第二个参数：格式，动图
	m_apngMovie->start();
	m_apngMovie->stop();
	setMovie(m_apngMovie);

}

//初始化控件
void EmotionLabelItem::initControl()
{
	setAlignment(Qt::AlignCenter);//设置标签对齐方式-->居中对齐
	setObjectName("emotionLabelItem");
	setFixedSize(32, 32);//固定表情的大小

}
