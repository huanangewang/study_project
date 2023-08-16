#include "TalkWindow.h"
#include "RootContactItem.h"
#include "ContactItem.h"
#include "CommonUtils.h"
#include "WindowManager.h"
#include <QToolTip>
#include <QFile>
#include <QMessageBox>

TalkWindow::TalkWindow(QWidget *parent, const QString& uid/*, GroupType groupType8*/)
	: QWidget(parent)
	,m_talkId(uid)
	//,m_groupType(groupType) //初始化列表
{
	ui.setupUi(this);
	//构造的时候，要将WindowManager里的addWindowName,里的映射添加上
	//解决可以打开多次相同窗口的问题
	WindowManager::getInstance()->addWindowName(m_talkId, this);
	initControl();
	setAttribute(Qt::WA_DeleteOnClose);//设置窗口属性
}

TalkWindow::~TalkWindow()
{
	//回收窗口资源
	WindowManager::getInstance()->deleteWindowName(m_talkId);
}

//添加表情图片
void TalkWindow::addEmotionImage(int emotionNum)
{
	ui.textEdit->setFocus();//设置焦点
	ui.textEdit->addEmotionUrl(emotionNum);
}

//设置窗口名字
void TalkWindow::setWindowName(const QString & name)
{
	ui.nameLabel->setText(name);
}

//发送信息
void TalkWindow::onSendBtnClicked(bool)
{
	//发送信息不能为空,要转成纯文本
	if (ui.textEdit->toPlainText().isEmpty())
	{
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)), QString::fromLocal8Bit("发送信息不能为空~~"),
			this, QRect(0, 0, 120, 100), 2000);//2000是显示的时间长度2s ； 
		return;
	}
	  QString html = ui.textEdit->document()->toHtml();//将文本转成html（字符串类型）,这里删除了引用和const

	//目的是解决，输入表情后，输入文字失效的bug
	//文件html如果没有字体，也就是既不是表情信息，并且不包含文本，则添加字体，msgFont.txt
	if (!html.contains(".png") && !html.contains("</span>"))
	{
		//没有字号
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");

		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			//数据替换
			fontHtml.replace("%1", text);
			file.close();
		}
		else //打开文件失败
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示")
				, QString::fromLocal8Bit("未找到msgFont.txt"));
			return;
		}

		if (!html.contains(fontHtml)) //没有包含的话,替换成包含字体的xxx
		{
			html.replace(text, fontHtml);
		}
	}


	ui.msgWidget->appendMsg(html);//追加信息

	ui.textEdit->clear();
	ui.textEdit->deletAllEmotionImage();

}

//初始化控件
void TalkWindow::initControl()
{
	//右边分类器设置尺寸,挨个设置，看是垂直还是水平
	QList <int> rightWidgetSize;
	rightWidgetSize << 600 << 138;
	ui.bodySplitter->setSizes(rightWidgetSize);

	ui.textEdit->setFontPointSize(10);//设置字体
	ui.textEdit->setFocus();

	connect(ui.sysmin,SIGNAL(clicked(bool)),parent(),SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));

	connect(ui.faceBtn, SIGNAL(clicked(bool)), parent(), SLOT(onEmotionBtnClicked(bool)));//表情按钮
	connect(ui.sendBtn, SIGNAL(clicked(bool)),this, SLOT(onSendBtnClicked(bool)));//发送按钮

	//树被双击
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int))
		, this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

	/*
	//根据聊天窗口构造时，用户指定的群不一样，进行不同的构造
	switch (m_groupType)
	{
	case COMPANY:
	{
		initCompanyTalk();//初始化公司群聊天
		break;
	}
	case PERSONELGROUP:
	{
		initPersonTalk();//初始化人事部聊天
		break;
	}
	case MARKETGROUP:
	{
		initMarketTalk();//初始化市场部聊天
		break;
	}
	case DEVELOPMENTGROUP:
	{
		initDevelopTalk();//初始化研发部聊天
		break;
	}
	case PTOP://单聊
	{
		initPtoPTalk();//初始化单独聊天
		break;
	}
	default:
		break;
	}
	*/
}

//添加员工信息
void TalkWindow::addPeopInfo(QTreeWidgetItem * pRootGroupItem)
{
	//联系项，设置头像。。。
	QTreeWidgetItem* pChild = new QTreeWidgetItem();

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");//先加载空的资源
	const QPixmap image(":/Resources/MainWindow/xmb.png");

	//添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole+1, QString::number((int)pChild));
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);//联系项，设置头像，签名什么的


	//测试
	static int i = 0;
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(image, pix1, pContactItem->getHeadLabelSize()));//设置圆头像
	pContactItem->setUserName(QString::fromLocal8Bit("华南歌王小号%1").arg(i++));
	pContactItem->setSignName(QString::fromLocal8Bit("谢铭滨真帅"));

	pRootGroupItem->addChild(pChild);//将当前子项添加到根项里
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);//添加到树里,设置项目部件(pContactItem)

	QString strName = pContactItem->getUserName();
	m_groupPerpleMap.insert(pChild, strName);//下次，我们在群聊中 双击打开某个人的时候，就可以拿到这个人的名字，拿来设置新的窗口标题



}


//聊天窗口中的双击右侧处理-->处理单聊
void TalkWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column)
{
	//判断子项和根项
	bool isChild = item->data(0, Qt::UserRole).toBool();
	if (isChild)
	{
		QString strPeopleName = m_groupPerpleMap.value(item);//获得value，传入key
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString()/*, PTOP, strPeopleName*/);//窗口名称应该用UserRole+1
	}

}

/*
//初始化公司群聊天
void TalkWindow::initCompanyTalk()
{
	//因为群有很多成员，先构造一个根
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//不管有没有，指示器都能用于展开和收缩

	pRootItem->setData(0, Qt::UserRole, 0);//根项数据设置0,设置data,用于区分根项，子项
	RootContactItem* pItemName = new RootContactItem(false,ui.treeWidget);

	ui.treeWidget->setFixedHeight(646); //shell总高-shell头高（talkwindow 的 titlewidget）

	int nEmployeeNum = 50;//假如总共50人
	QString qsGroupName = QString::fromLocal8Bit("公司群 %1/%2").arg(0).arg(nEmployeeNum); //假设的测试数据
	pItemName->setText(qsGroupName);//设置根文本

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//默认展开
	pRootItem->setExpanded(true);

	for (int i = 0;i < nEmployeeNum;i++)
	{
		addPeopInfo(pRootItem);//给当前的pRootItem添加
	}

}

//初始化人事部聊天
void TalkWindow::initPersonTalk()
{
	//因为群有很多成员，先构造一个根
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//不管有没有，指示器都能用于展开和收缩

	pRootItem->setData(0, Qt::UserRole, 0);//根项数据设置0,设置data,用于区分根项，子项
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646); //shell总高-shell头高（talkwindow 的 titlewidget）

	int nEmployeeNum = 5;//假如总共5人
	QString qsGroupName = QString::fromLocal8Bit("人事部 %1/%2").arg(0).arg(nEmployeeNum); //假设的测试数据
	pItemName->setText(qsGroupName);//设置根文本

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//默认展开
	pRootItem->setExpanded(true);

	for (int i = 0;i < nEmployeeNum;i++)
	{
		addPeopInfo(pRootItem);//给当前的pRootItem添加
	}
}

//初始化市场部聊天
void TalkWindow::initMarketTalk()
{
	//因为群有很多成员，先构造一个根
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//不管有没有，指示器都能用于展开和收缩

	pRootItem->setData(0, Qt::UserRole, 0);//根项数据设置0,设置data,用于区分根项，子项
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646); //shell总高-shell头高（talkwindow 的 titlewidget）

	int nEmployeeNum = 8;//假如总共50人
	QString qsGroupName = QString::fromLocal8Bit("市场部 %1/%2").arg(0).arg(nEmployeeNum); //假设的测试数据
	pItemName->setText(qsGroupName);//设置根文本

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//默认展开
	pRootItem->setExpanded(true);

	for (int i = 0;i < nEmployeeNum;i++)
	{
		addPeopInfo(pRootItem);//给当前的pRootItem添加
	}
}

//初始化研发部聊天
void TalkWindow::initDevelopTalk()
{
	//因为群有很多成员，先构造一个根
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//不管有没有，指示器都能用于展开和收缩

	pRootItem->setData(0, Qt::UserRole, 0);//根项数据设置0,设置data,用于区分根项，子项
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646); //shell总高-shell头高（talkwindow 的 titlewidget）

	int nEmployeeNum = 32;//假如总共50人
	QString qsGroupName = QString::fromLocal8Bit("研发部 %1/%2").arg(0).arg(nEmployeeNum); //假设的测试数据
	pItemName->setText(qsGroupName);//设置根文本

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//默认展开
	pRootItem->setExpanded(true);

	for (int i = 0;i < nEmployeeNum;i++)
	{
		addPeopInfo(pRootItem);//给当前的pRootItem添加
	}
}

*/
//初始化单独聊天
void TalkWindow::initPtoPTalk()
{
	//在右边放一张图片
	QPixmap pixSkin;
	pixSkin.load(":/Resources/MainWindow/skin.png");

	ui.widget->setFixedSize(pixSkin.size());//设置尺寸

	//图片一般放标签上
	QLabel* skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());//设置标签大小

}
