#include "TalkWindowItem.h"
#include "CommonUtils.h"

TalkWindowItem::TalkWindowItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initControl();//初始化控件
}

TalkWindowItem::~TalkWindowItem()
{}

//设置头像
void TalkWindowItem::setHeadPixmap(const QPixmap& pixmap)
{
	QPixmap mask = QPixmap(":/Resources/MainWindow/head_mask.png");
	//QPixmap head = QPixmap(":/Resources/MainWindow/xmb.png");

	//1.原图片 2.mask 3.尺寸   关于QPixmap的用法，一直不太理解
	const QPixmap& headpixmap = CommonUtils::getRoundImage(pixmap,mask,ui.headlabel->size());
	ui.headlabel->setPixmap(headpixmap);

}

//设置标签内容，是和人还是公司聊天
void TalkWindowItem::setMsgLabelContent(const QString & msg)
{
	ui.msgLabel->setText(msg);

}

//获取在和谁聊天
QString TalkWindowItem::getMsgLabelText()
{
	return ui.msgLabel->text();
}

//初始化控件
void TalkWindowItem::initControl()
{
	//关闭按钮一开始已经不可见
	ui.tclosebtn->setVisible(false);
	//点击关闭的话,聊天窗口接收到信号就关闭
	connect(ui.tclosebtn, SIGNAL(clicked(bool)), this, SIGNAL(signalCloseClicked()));

}

//进入事件
void TalkWindowItem::enterEvent(QEvent * event)
{
	//让关闭按钮可见
	ui.tclosebtn->setVisible(true);
	__super::enterEvent(event); //进行默认 进入事件的操作
}

//离开事件
void TalkWindowItem::leaveEvent(QEvent * event)
{
	//让关闭按钮不可见
	ui.tclosebtn->setVisible(false);
	__super::enterEvent(event); //进行默认 进入事件的操作
}

//尺寸重置
void TalkWindowItem::resizeEvent(QResizeEvent * event)
{
	__super::resizeEvent(event); //进行默认 进入事件的操作
}
