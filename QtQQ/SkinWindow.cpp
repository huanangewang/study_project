#include "SkinWindow.h"
#include "QClickLabel.h"
#include "NotifyManager.h"

//�޸ĸ����BasicWindow
SkinWindow::SkinWindow(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	loadStyleSheet("SkinWindow");
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();//��ʼ���ؼ�
}

SkinWindow::~SkinWindow()
{
}

//��ʼ���ؼ�
void SkinWindow::initControl()
{
	//��ɫ������(Ĭ��12��),s����ֱ�ӳ�ʼ��
	QList<QColor> colorList = {
		QColor(22,154,218), QColor(40,138,221), QColor(49,166,107), QColor(218,67,68),
		QColor(177,99,158), QColor(107,81,92),  QColor(89,92,160),  QColor(21,156,199),
		QColor(79,169,172), QColor(155,183,154),QColor(128,77,77),  QColor(240,188,189)
	};
	
	//3��(row)��4��(column)
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			//�Զ������ QClickLabel ,��д����갴���¼���ʵ���˵����ǩlabel,����ʵ�ַ��͵���ź�
			QClickLabel *label = new QClickLabel(this);//label������ɫ���ɣ�Ҳ������label
			label->setCursor(Qt::PointingHandCursor);//���ù����״����ָ

			//������Ҫ����ȥ��֪ͨ�������ڽ��иı�,�����У��У���ɫ���� instance�Ǿ�̬�ģ�ֻ��һ��
			connect(label, &QClickLabel::clicked, [row, column, colorList]() {
				NotifyManager::getInstance()->notifyOtherWindowChangeSkin(colorList.at(row * 4 + column));
			});

			label->setFixedSize(84, 84);

			//����ѡ���ǩҳ��ĸ�����ǩ��ɫ
			QPalette palette;
			palette.setColor(QPalette::Background, colorList.at(row * 4 + column));
			label->setAutoFillBackground(true);//�����Զ����
			label->setPalette(palette);//���õ�ɫ��

			ui.gridLayout->addWidget(label, row, column);//������ɺ󣬻�Ҫ�ǵ÷��ڲ��ֹ�������
		}
	}

	//�رպ���С�������źŲ�
	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked()), this, SLOT(onShowClose()));
}

//��д�رղ�����-->Ϊ�˹�Ƥ�����棬�����ǹ��������ó���
void SkinWindow::onShowClose()
{
	this->close();
}
