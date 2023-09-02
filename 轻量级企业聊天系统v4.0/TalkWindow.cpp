#include "TalkWindow.h"
#include "RootContactItem.h"
#include "ContactItem.h"
#include "CommonUtils.h"
#include "WindowManager.h"
#include "SendFile.h"


#include <QToolTip>
#include <QFile>
#include <QMessageBox>
#include<QSqlQueryModel>
#include <QSqlQuery>
#include<QModelIndex>

extern QString gLoginEmployeeID;

TalkWindow::TalkWindow(QWidget *parent, const QString& uid/*, GroupType groupType8*/)
	: QWidget(parent)
	,m_talkId(uid)
	//,m_groupType(groupType) //初始化列表
{
	ui.setupUi(this);
	//构造的时候，要将WindowManager里的addWindowName,里的映射添加上
	//解决可以打开多次相同窗口的问题
	WindowManager::getInstance()->addWindowName(m_talkId, this);
	setAttribute(Qt::WA_DeleteOnClose);//设置窗口属性

	initGroupTalkStatus();//初始化群聊状态
	initControl();
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

//为了访问私有数据 m_talkid
QString TalkWindow::getTalkId()
{
	return m_talkId;
}

//文件打开按钮
void  TalkWindow::onFileOpenBtnClicked(bool)
{
	SendFile* sendFile = new SendFile(this);
	sendFile->show();//ui界面
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

	connect(ui.fileopenBtn, SIGNAL(clicked(bool)), this, SLOT(onFileOpenBtnClicked(bool)));//文件打开按钮

	//判断是否是群聊
	if (m_isGroupTalk)
	{
		initTalkWindow();//初始化群聊
	}
	else
	{
		initPtoPTalk();//初始化单聊
	}

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
//初始化群聊状态
void TalkWindow::initGroupTalkStatus()
{
	//通过传来的员工id号，也就是uid(m_talkid),在数据库里查询,进入tab_department表进行查询，查得到就是群聊，否则单聊
	QSqlQueryModel sqlDepModel;
	QString strSql = QString("SELECT * FROM tab_department WHERE departmentID = %1").arg(m_talkId);//看看有没有departmentID是m_talkId
	sqlDepModel.setQuery(strSql);//设置模型的sql语句

	int rows = sqlDepModel.rowCount();
	if (rows == 0)
	{
		//单聊
		m_isGroupTalk = false;
	}
	//群聊
	else
	{
		m_isGroupTalk = true;
	}
}


//获取公司的id号-->对公司群进行 特殊处理
int TalkWindow::getComDepID()
{
	//通过公司群，查询公司群的id
	QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
		.arg(QString::fromLocal8Bit("公司群")));
	queryDepID.exec();
	queryDepID.first();//指向结果集第一条

	return queryDepID.value(0).toInt();

}

//添加员工信息
void TalkWindow::addPeopInfo(QTreeWidgetItem * pRootGroupItem, int employeeID)
{
	//联系项，设置头像。。。
	QTreeWidgetItem* pChild = new QTreeWidgetItem();

	//添加子节点 --》一直不太懂这里为什么要这样写
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID); //目的为了修复左侧栏不显示人头像的bug
	//pChild->setData(0, Qt::UserRole + 1, QString::number((int)pChild));
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);//联系项，设置头像，签名什么的

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");//先加载空的资源
	//const QPixmap image(":/Resources/MainWindow/xmb.png");

	//获取用户名，签名，头像
	QString strName, strSign, strPicturePath;
	QSqlQueryModel queryInfoModel;//查询模型
	queryInfoModel.setQuery(QString("SELECT employee_name,employee_sign,picture FROM tab_employees WHERE employeeID = %1")
		.arg(employeeID));//这条语句要能查询： 用户名，签名，头像
	QModelIndex nameIndex, signIndex, pictureIndex; //三项的索引
	nameIndex = queryInfoModel.index(0, 0); //行，列
	signIndex = queryInfoModel.index(0, 1);
	pictureIndex = queryInfoModel.index(0, 2);

	//返回需要的值
	strName =		 queryInfoModel.data(nameIndex).toString();
	strSign =		 queryInfoModel.data(signIndex).toString();
	strPicturePath = queryInfoModel.data(pictureIndex).toString();

	////测试
	//static int i = 0;
	//pContactItem->setHeadPixmap(CommonUtils::getRoundImage(image, pix1, pContactItem->getHeadLabelSize()));//设置圆头像
	//pContactItem->setUserName(QString::fromLocal8Bit("华南歌王小号%1").arg(i++));
	//pContactItem->setSignName(QString::fromLocal8Bit("谢铭滨真帅"));

	QImage imageHead;
	imageHead.load(strPicturePath);//加载数据库中的头像路径

	//设置数据
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(QPixmap::fromImage(imageHead), pix1, pContactItem->getHeadLabelSize()));//设置圆头像
	pContactItem->setUserName(strName);
	pContactItem->setSignName(strSign);

	pRootGroupItem->addChild(pChild);//将当前子项添加到根项里
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);//添加到树里,设置项目部件(pContactItem)

	QString str = pContactItem->getUserName();
	m_groupPerpleMap.insert(pChild, str);//下次，我们在群聊中 双击打开某个人的时候，就可以拿到这个人的名字，拿来设置新的窗口标题



}


//聊天窗口中的双击右侧处理-->处理单聊
void TalkWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column)
{
	//判断子项和根项
	bool isChild = item->data(0, Qt::UserRole).toBool();
	if (isChild)
	{
		QString talkId = item->data(0, Qt::UserRole + 1).toString();
		if (talkId == gLoginEmployeeID)
		{
			return;
		}
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

//初始化群聊
void TalkWindow::initTalkWindow()
{
	//1.构造根项
	//因为群有很多成员，先构造一个根
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//不管有没有，指示器都能用于展开和收缩

	pRootItem->setData(0, Qt::UserRole, 0);//根项数据设置0,设置data,用于区分根项，子项
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646); //shell总高-shell头高（talkwindow 的 titlewidget）

	//拿到当前聊天群组名
	QString strGroupName;                                     // SQL语句知识补充：where 是值得找的方法,这里通过id找
	QSqlQuery queryGroupName(QString("SELECT department_name FROM tab_department WHERE departmentID = %1").arg(m_talkId));//为了查找部门名称
	queryGroupName.exec();//运行
	if (queryGroupName.first())
	{
		strGroupName = queryGroupName.value(0).toString();
	}

	//下面要获取员工数量
	//拿人数-->如果是公司群，要做特殊处理，因为没法通过2000(公司群id)来判断有几人，毕竟没人的部门id是2000 ,只有2001-2003
	QSqlQueryModel queryEmployeeModel;
	if (getComDepID() == m_talkId.toInt())//特殊情况，公司群
	{
		queryEmployeeModel.setQuery("SELECT employeeID FROM tab_employees WHERE status = 1");//查找所有状态为1的员工
	}
	else
	{
		//两个条件：一个是找状态为1的员工，一个是找对应部门id的员工
		queryEmployeeModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status = 1 AND departmentID =%1")
			.arg(m_talkId));
	}
	//接下来只需要看看查询结果有几行即可
	int nEmployeeNum = queryEmployeeModel.rowCount();
	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3")
		.arg(strGroupName)
		.arg(0)
		.arg(nEmployeeNum); //1、群名 2、在线人数数量 3、总人数


	pItemName->setText(qsGroupName);//设置根文本
	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//默认展开
	pRootItem->setExpanded(true);

	//添加成员信息(要通过员工id)，现在不能只传入一个根项就行了
	//2023-08-29 BUG修改，不初始化自己的窗口，不能和自己聊天
	for (int i = 0;i < nEmployeeNum;i++)
	{
		QModelIndex modelIndex = queryEmployeeModel.index(i, 0); //行：i  列：0  -->当前行所在的模型索引
		int employeeID = queryEmployeeModel.data(modelIndex).toInt();//拿到员工id号
		addPeopInfo(pRootItem,employeeID);//给当前的pRootItem添加,添加子节点
	}
}
