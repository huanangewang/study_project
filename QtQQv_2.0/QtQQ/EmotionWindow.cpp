#include "EmotionWindow.h"
#include "CommonUtils.h"
#include <QStyleOption>
#include <QPainter>

//����� �� ��
const int emotionRow = 12;
const int emotionColumn = 14;


EmotionWindow::EmotionWindow(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);//������û�߿��Ӵ���
	setAttribute(Qt::WA_TranslucentBackground);//͸������
	setAttribute(Qt::WA_DeleteOnClose);//�Դ�����Դ���л���

	ui.setupUi(this);
	initControl();//��ʼ���ؼ�
}

EmotionWindow::~EmotionWindow()
{}

//��ʼ���ؼ�
void EmotionWindow::initControl()
{
	CommonUtils::loadStyleSheet(this, "EmotionWindow");//�����Զ�����ʽ

	//���ر���ͼƬ 12x14
	for (int row = 0;row < emotionRow;row++)
	{
		for (int column = 0;column < emotionColumn;column++)
		{
			EmotionLabelItem* label = new EmotionLabelItem(this);
			label->setEmotionName(row * emotionColumn + column); //���� 0*3 + 2

			connect(label, &EmotionLabelItem::emotionClicked, this, &EmotionWindow::onAddEmotion); //ֻҪ�յ���ǩ�ĵ���źţ��Զ���ģ�������ӱ���
			ui.gridLayout->addWidget(label, row, column);//��ӵ����ֹ�����

		}
	}
}

//��ӱ���ۺ���
void EmotionWindow::onAddEmotion(int emotionNum)
{
	//�ڷ�����Ϣ�ǽ�����ӱ��飬����ֻ��Ҫʵ�֣����͸��źż���
	this->hide();
	emit signalEmotionWindowHide();//���鴰��������
	emit signalEmotionItemClicked(emotionNum);//�ĸ����鱻�����//
}


//�ػ�
void EmotionWindow::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;//ѡ��
	opt.init(this);

	QPainter painter(this);//����

	//����û�߿� ����1 ������������2 ��ѡ�� ����3 ������ ����4������
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	__super::paintEvent(event);//���������Ļ�ͼ�¼�����

}