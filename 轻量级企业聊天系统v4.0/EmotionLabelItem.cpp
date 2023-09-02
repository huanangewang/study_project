#include "EmotionLabelItem.h"

EmotionLabelItem::EmotionLabelItem(QWidget *parent)
	: QClickLabel(parent)
{
	initControl();
	connect(this, &QClickLabel::clicked, [this]() {
		emit emotionClicked(m_emotionName);//���ͱ������źż���
	});
}

EmotionLabelItem::~EmotionLabelItem()
{}

//���ñ�������
void EmotionLabelItem::setEmotionName(int emotionName)
{
	m_emotionName = emotionName;
	QString imageName = QString(":/Resources/MainWindow/emotion/%1.png").arg(emotionName);//������

	m_apngMovie = new QMovie(imageName,"apng",this);//�ڶ�����������ʽ����ͼ
	m_apngMovie->start();
	m_apngMovie->stop();
	setMovie(m_apngMovie);

}

//��ʼ���ؼ�
void EmotionLabelItem::initControl()
{
	setAlignment(Qt::AlignCenter);//���ñ�ǩ���뷽ʽ-->���ж���
	setObjectName("emotionLabelItem");
	setFixedSize(32, 32);//�̶�����Ĵ�С

}
