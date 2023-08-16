#include "TalkWindowShell.h"
#include "CommonUtils.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"

#include "EmotionWindow.h"
#include <qlistwidget.h>
#include <QSqlQueryModel>


//�޸Ļ��� BasicWindow
TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//�������ԣ������ڹرգ�Ҫ������Դ����
	initControl();//��ʼ���ؼ�

}

TalkWindowShell::~TalkWindowShell()
{
	
	delete m_emotionWindow;
	m_emotionWindow=nullptr;
	
}

//�������-->���촰��  ,����2 �����ں���һ������
//���������һ��һ��һ�Զ�
void TalkWindowShell::addTalkWindow(TalkWindow* talkWindow, TalkWindowItem * talkWindowItem,const QString& uid/* GroupType grouptype*/)
{
	ui.rightStackedWidget->addWidget(talkWindow);//˫�������һ�����촰��
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()),
		talkWindow, SLOT(onSetEmotionBtnStatus()));// onSetEmotionBtnStatus���ñ��鰴ť״̬

	QListWidgetItem* aItem = new QListWidgetItem(ui.listWidget);//����б�
	m_talkwindowItemMap.insert(aItem, talkWindow); //key:aItem ��value:talkWindow

	aItem->setSelected(true);//��ѡ��

	//ͨ�����ݿ�����ͷ��
	//�ж���Ⱥ�Ļ��ǵ���
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(uid);
	sqlDepModel.setQuery(strQuery);//���ò�ѯ��������û�н��
	int rows = sqlDepModel.rowCount(); //0�о��ǵ��ģ�����Ⱥ��

	if (rows == 0)//����,Ҫ����sql���
	{
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid);
		sqlDepModel.setQuery(strQuery);
	}

	QModelIndex index;//ģ������
	index = sqlDepModel.index(0, 0);//�У��У����ͷ�������·��

	QImage img;
	img.load(sqlDepModel.data(index).toString());
 	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));//����ͷ�� TODO:�ĳ�Pixmap

	ui.listWidget->addItem(aItem);//����б���(���)
	ui.listWidget->setItemWidget(aItem, talkWindowItem);//���ò���

	onTalkWindowItemClicked(aItem);//��ǰ������

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked, 
		[talkWindowItem,talkWindow,aItem,this]()
	{
		//Ҫ��ӳ���Ƴ����б����Ƴ������촰�ڹرգ�talkWindowItem��Դ�ͷ�
		//�ȴ������
		m_talkwindowItemMap.remove(aItem);
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));
		delete talkWindowItem;
		//�����ұ�
		ui.rightStackedWidget->removeWidget(talkWindow);
		if (ui.rightStackedWidget->count() < 1)
		{
			this->close();
		}
	});
}

//���õ�ǰ����Ĵ���(���ĸ����ڽ�������)
void TalkWindowShell::setCurrentWidget(QWidget * widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

//Ϊ�����ⲿ�������˽�г�Ա
const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::getTalkWindowItemMap() const
{
	return m_talkwindowItemMap;
}

//��ʼ���ؼ�
void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");//������ʽ��
	setWindowTitle(QString::fromLocal8Bit("˧��-���촰��"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();		//���ر��鴰��

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;//���154�����أ��ұ�...
	ui.splitter->setSizes(leftWidgetSize);	//���������óߴ�

	ui.listWidget->setStyle(new CustomProxyStyle(this));//���÷��

	//��౻����Ļ������߱��鱻ѡ��
	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}
//���鰴ť�����
void TalkWindowShell::onEmotionBtnClicked(bool)
{
	//��Ҫ�Ǳ��鴰�ڿɼ������ǲ��ɼ�
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//����ǰ�ؼ������λ�ã�ת��Ϊ��Ļ�ľ���λ��

	emotionPoint.setX(emotionPoint.x() + 170);//ƫ�Ʊ��鴰�ڣ�Ϊ�˺ÿ�
	emotionPoint.setY(emotionPoint.y() + 220);

	m_emotionWindow->move(emotionPoint);
}

//ѡ���ĸ�����
void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	//��������TalkWindow��,Ĭ�Ϸ����ǲ������ͣ�����ǿת
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());

	if (curTalkWindow)
	{
		//��ӱ���ͼƬ
		curTalkWindow->addEmotionImage(emotionNum);

	}
}

//��ʵ������ߵ��б���
void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem* item)
{
	//���ұ������ҳ�沿��������Ҫ��ʾ�Ĳ��� -->��ӳ�䱣���� ���ڵĵ�ַ
	QWidget* talkwindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkwindowWidget);//�����ұߴ���
}