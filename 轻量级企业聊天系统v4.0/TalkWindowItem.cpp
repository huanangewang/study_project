#include "TalkWindowItem.h"
#include "CommonUtils.h"

TalkWindowItem::TalkWindowItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initControl();//��ʼ���ؼ�
}

TalkWindowItem::~TalkWindowItem()
{}

//����ͷ��
void TalkWindowItem::setHeadPixmap(const QPixmap& pixmap)
{
	QPixmap mask = QPixmap(":/Resources/MainWindow/head_mask.png");
	//QPixmap head = QPixmap(":/Resources/MainWindow/xmb.png");

	//1.ԭͼƬ 2.mask 3.�ߴ�   ����QPixmap���÷���һֱ��̫���
	const QPixmap& headpixmap = CommonUtils::getRoundImage(pixmap,mask,ui.headlabel->size());
	ui.headlabel->setPixmap(headpixmap);

}

//���ñ�ǩ���ݣ��Ǻ��˻��ǹ�˾����
void TalkWindowItem::setMsgLabelContent(const QString & msg)
{
	ui.msgLabel->setText(msg);

}

//��ȡ�ں�˭����
QString TalkWindowItem::getMsgLabelText()
{
	return ui.msgLabel->text();
}

//��ʼ���ؼ�
void TalkWindowItem::initControl()
{
	//�رհ�ťһ��ʼ�Ѿ����ɼ�
	ui.tclosebtn->setVisible(false);
	//����رյĻ�,���촰�ڽ��յ��źž͹ر�
	connect(ui.tclosebtn, SIGNAL(clicked(bool)), this, SIGNAL(signalCloseClicked()));

}

//�����¼�
void TalkWindowItem::enterEvent(QEvent * event)
{
	//�ùرհ�ť�ɼ�
	ui.tclosebtn->setVisible(true);
	__super::enterEvent(event); //����Ĭ�� �����¼��Ĳ���
}

//�뿪�¼�
void TalkWindowItem::leaveEvent(QEvent * event)
{
	//�ùرհ�ť���ɼ�
	ui.tclosebtn->setVisible(false);
	__super::enterEvent(event); //����Ĭ�� �����¼��Ĳ���
}

//�ߴ�����
void TalkWindowItem::resizeEvent(QResizeEvent * event)
{
	__super::resizeEvent(event); //����Ĭ�� �����¼��Ĳ���
}
