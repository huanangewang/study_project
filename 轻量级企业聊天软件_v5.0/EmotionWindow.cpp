#include "EmotionWindow.h"
#include "CommonUtils.h"
#include <QStyleOption>
#include <QPainter>

//表情的 行 列
const int emotionRow = 12;
const int emotionColumn = 14;


EmotionWindow::EmotionWindow(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);//窗体风格，没边框，子窗口
	setAttribute(Qt::WA_TranslucentBackground);//透明属性
	setAttribute(Qt::WA_DeleteOnClose);//对窗体资源进行回收

	ui.setupUi(this);
	initControl();//初始化控件
}

EmotionWindow::~EmotionWindow()
{}

//初始化控件
void EmotionWindow::initControl()
{
	CommonUtils::loadStyleSheet(this, "EmotionWindow");//加载自定义样式

	//加载表情图片 12x14
	for (int row = 0;row < emotionRow;row++)
	{
		for (int column = 0;column < emotionColumn;column++)
		{
			EmotionLabelItem* label = new EmotionLabelItem(this);
			label->setEmotionName(row * emotionColumn + column); //比如 0*3 + 2

			connect(label, &EmotionLabelItem::emotionClicked, this, &EmotionWindow::onAddEmotion); //只要收到标签的点击信号（自定义的），就添加表情
			ui.gridLayout->addWidget(label, row, column);//添加到布局管理器

		}
	}
}

//添加表情槽函数
void EmotionWindow::onAddEmotion(int emotionNum)
{
	//在发送信息那进行添加表情，这里只需要实现，发送个信号即可
	this->hide();
	emit signalEmotionWindowHide();//表情窗口隐藏了
	emit signalEmotionItemClicked(emotionNum);//哪个表情被点击了//
}


//重绘
void EmotionWindow::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;//选项
	opt.init(this);

	QPainter painter(this);//画家

	//让其没边框 参数1 ：部件，参数2 ：选项 参数3 ：画家 参数4：部件
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	__super::paintEvent(event);//进行其他的绘图事件操作

}