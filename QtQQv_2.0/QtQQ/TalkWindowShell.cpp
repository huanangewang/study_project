#include "TalkWindowShell.h"
#include "CommonUtils.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"

#include "EmotionWindow.h"
#include <qlistwidget.h>
#include <QSqlQueryModel>


//修改基类 BasicWindow
TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//设置属性，当窗口关闭，要进行资源回收
	initControl();//初始化控件

}

TalkWindowShell::~TalkWindowShell()
{
	
	delete m_emotionWindow;
	m_emotionWindow=nullptr;
	
}

//添加聊天-->聊天窗口  ,参数2 ：你在和哪一项聊天
//两个情况，一对一，一对多
void TalkWindowShell::addTalkWindow(TalkWindow* talkWindow, TalkWindowItem * talkWindowItem,const QString& uid/* GroupType grouptype*/)
{
	ui.rightStackedWidget->addWidget(talkWindow);//双击后，添加一个聊天窗口
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()),
		talkWindow, SLOT(onSetEmotionBtnStatus()));// onSetEmotionBtnStatus设置表情按钮状态

	QListWidgetItem* aItem = new QListWidgetItem(ui.listWidget);//左侧列表
	m_talkwindowItemMap.insert(aItem, talkWindow); //key:aItem ；value:talkWindow

	aItem->setSelected(true);//被选中

	//通过数据库设置头像
	//判断是群聊还是单聊
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(uid);
	sqlDepModel.setQuery(strQuery);//设置查询，看看有没有结果
	int rows = sqlDepModel.rowCount(); //0行就是单聊，否则群聊

	if (rows == 0)//单聊,要更改sql语句
	{
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid);
		sqlDepModel.setQuery(strQuery);
	}

	QModelIndex index;//模型索引
	index = sqlDepModel.index(0, 0);//行，列，获得头像的索引路径

	QImage img;
	img.load(sqlDepModel.data(index).toString());
 	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));//设置头像 TODO:改成Pixmap

	ui.listWidget->addItem(aItem);//添加列表项(左边)
	ui.listWidget->setItemWidget(aItem, talkWindowItem);//设置部件

	onTalkWindowItemClicked(aItem);//当前这项被点击

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked, 
		[talkWindowItem,talkWindow,aItem,this]()
	{
		//要将映射移除，列表项移除，聊天窗口关闭，talkWindowItem资源释放
		//先处理左边
		m_talkwindowItemMap.remove(aItem);
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));
		delete talkWindowItem;
		//处理右边
		ui.rightStackedWidget->removeWidget(talkWindow);
		if (ui.rightStackedWidget->count() < 1)
		{
			this->close();
		}
	});
}

//设置当前聊天的窗口(和哪个窗口进行聊天)
void TalkWindowShell::setCurrentWidget(QWidget * widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

//为了在外部访问这个私有成员
const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::getTalkWindowItemMap() const
{
	return m_talkwindowItemMap;
}

//初始化控件
void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");//加载样式表
	setWindowTitle(QString::fromLocal8Bit("帅滨-聊天窗口"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();		//隐藏表情窗口

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;//左边154个像素，右边...
	ui.splitter->setSizes(leftWidgetSize);	//分类器设置尺寸

	ui.listWidget->setStyle(new CustomProxyStyle(this));//设置风格

	//左侧被点击的话，或者表情被选中
	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}
//表情按钮被点击
void TalkWindowShell::onEmotionBtnClicked(bool)
{
	//主要是表情窗口可见，还是不可见
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//将当前控件的相对位置，转换为屏幕的绝对位置

	emotionPoint.setX(emotionPoint.x() + 170);//偏移表情窗口，为了好看
	emotionPoint.setY(emotionPoint.y() + 220);

	m_emotionWindow->move(emotionPoint);
}

//选了哪个表情
void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	//方法都在TalkWindow里,默认返回是部件类型，这里强转
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());

	if (curTalkWindow)
	{
		//添加表情图片
		curTalkWindow->addEmotionImage(emotionNum);

	}
}

//其实就是左边的列表部件
void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem* item)
{
	//将右边这个多页面部件，设置要显示的部件 -->用映射保存了 窗口的地址
	QWidget* talkwindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkwindowWidget);//更新右边窗体
}