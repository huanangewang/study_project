#include "CCMainWindow.h"
#include "SkinWindow.h"
#include"SysTray.h"
#include"QClickLabel.h"
#include "NotifyManager.h"
#include "RootContactItem.h"
#include"ContactItem.h"

#include <QProxyStyle>
#include <QPainter>
#include <QTimer>
#include<QString>
#include <QHBoxLayout>
#include <QEvent>

class CustomProxyStyle :public QProxyStyle
{
public:
	// 重写 drawPrimitive方法
	// 当边框矩形，获取焦点，就不进行绘制，直接返回
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget = nullptr) const
	{
		// 针对，边框获取焦点时，进行处理
		// 如果元素 等于 边框矩形，就直接返回，不绘制它
		if (element == PE_FrameFocusRect)
		{
			return;
		}
		else
		{
			// 如果不是，就进行 默认操作
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};

//将基类修改成我们自定义的类
//2023.08.05 晚上11点更改：初始化QQ等级
CCMainWindow::CCMainWindow(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::Tool);//设置窗口风格，工具窗口风格
	loadStyleSheet("CCMainWindow");//加载我们已经存在的css样式
	initControl();//初始化控件
	initTimer();//初始化计时器
}

CCMainWindow::~CCMainWindow()
{
}

//初始化定时器---->暂时用于等级更新，后期可以用多线程，并用数据库
void CCMainWindow::initTimer()
{
	QTimer *timer = new QTimer(this);
	timer->setInterval(5000);//设置间隔，5000ms
	connect(timer, &QTimer::timeout, [this]()
	{
		//时间到了就升级
		static int level = 0;
		if (level == 99) level = 0;
		level += 1;
		//设置图像
		setLevelPixmap(level);
	});
	timer->start();
}

void CCMainWindow::initControl()
{
	//树获取焦点时不绘制边框
	ui.treeWidget->setStyle(new CustomProxyStyle);
	/////测试代码///////
	setLevelPixmap(0);
	setHeadPixmap(":/Resources/MainWindow/xmb.png");
	setStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");//在线

	QHBoxLayout *appupLayout = new QHBoxLayout;//水平布局管理器
	appupLayout->setContentsMargins(0, 0, 0, 0);//设置间隙
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_7.png", "app_7"));//addWidget添加部件
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_2.png", "app_2"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_3.png", "app_3"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_4.png", "app_4"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_5.png", "app_5"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_6.png", "app_6"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));//实际功能：皮肤
	appupLayout->addStretch();
	appupLayout->setSpacing(2);//布局间空隙
	ui.appWidget->setLayout(appupLayout);//界面设置布局

	//底部app图标设置
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));
	ui.bottomLayout_up->addStretch();//加伸缩量，右边会被空隙填充

	//事件过滤器--->1.个性签名  2.好友搜索
	ui.lineEdit->installEventFilter(this);//this是监视对象
	ui.searchLineEdit->installEventFilter(this);


	//最小化---->要做系统托盘，不能直接消失了
	connect(ui.sysmin,   SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));

	//窗口皮肤颜色改变，也要更新搜索框的样式
	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [=](){
		updataSeachSheet();
		});

	SysTray* systray = new SysTray(this);//系统托盘图标

	initContactTree();//初始化联系人树

}

//更新搜索的样式
void CCMainWindow::updataSeachSheet()
{
	//设置成原来的rgb颜色
	QString sheet ="QWidget#searchWidget{background-color:rgba(%1,%2,%3,50);border-bottom:1px solid rgba(%1,%2,%3,30)}\
					 QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/search_icon.png)}";
	ui.searchWidget->setStyleSheet(sheet
										.arg(m_colorBackGround.red())
										.arg(m_colorBackGround.green())
										.arg(m_colorBackGround.blue()));

}

//添加公司部门  ,1.根项 2.部门名字--> 一次加一个子项
void CCMainWindow::addCompanyDeps(QTreeWidgetItem* pRootGroupItem, const QString& sDeps)
{
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");//默认白圆头

	QTreeWidgetItem* pChild = new QTreeWidgetItem;
	pChild->setData(0, Qt::UserRole, 1);//添加子节点,子项数据设置为1

	ContactItem* pContactItem = new ContactItem(ui.treeWidget);//ContactItem是封装好的类，用来显示各个好友的头像，个性签名等
	pContactItem->setHeadPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"),
								pix,  pContactItem->getHeadLabelSize() ) );//设置头像，并且是圆头像
	pContactItem->setUserName(sDeps);//设置用户名

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

}

//设置用户名
void CCMainWindow::setUserName(const QString & username)
{
	ui.nameLabel->adjustSize();//根据内容调整
	//如果文本过长，进行省略...	 1.文本, 2.枚举，3.指定宽度
	  QString name = ui.nameLabel->fontMetrics().elidedText(username,
		Qt::ElideRight,
		ui.nameLabel->width());//fontMetrics()返回QFontMetrics类对象
	ui.nameLabel->setText(name);
}

//设置等级
void CCMainWindow::setLevelPixmap(int level)
{
	QPixmap levelPixmap(ui.levelBtn->size());//拿一个位图,初始化大小(图图)
	levelPixmap.fill(Qt::transparent);//设置透明

	QPainter painter(&levelPixmap);
	painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

	//考虑到等级有一位数，两位数
	int unitNum = level % 10;//个位数
	int tenNum = level / 10; //十位数

	//十位,截取图片中的部分进行绘制
	//drawPixmap(绘制点x，绘制点y,图片，图片左上角x,图片左上角y,拷贝的宽度，拷贝的高度)
	//先画十位，只需要绘制图片的一部分,图片里，每个数字都是7个像素高，宽是6个像素
	painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), tenNum * 6, 0, 6, 7);

	//个位
	painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unitNum * 6, 0, 6, 7);

	ui.levelBtn->setIcon(levelPixmap);//设置图标
	ui.levelBtn->setIconSize(ui.levelBtn->size());//适应大小
}

//设置头像
void CCMainWindow::setHeadPixmap(const QString & headPath)
{
	//主要也是用位图进行设置
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");//圆空白
	ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));//源，图像，大小
}

//设置状态
void CCMainWindow::setStatusMenuIcon(const QString & statusPath)
{
	QPixmap statusBtnPixmap(ui.stausBtn->size());//以按钮大小设置大小
	statusBtnPixmap.fill(Qt::transparent);//transparent:透明

	QPainter painter(&statusBtnPixmap);//用画家绘制,里面放绘图设备(在哪绘制 )
	painter.drawPixmap(4, 4, QPixmap(statusPath));//画图位置，图片

	ui.stausBtn->setIcon(statusBtnPixmap);
	ui.stausBtn->setIconSize(ui.stausBtn->size());
}

//添加应用部件(app图片路径，app部件对象名)--->拓展功能可以慢慢实现：2023.08.06
//思路就是用信号槽，点击部件，会触发
QWidget * CCMainWindow::addOtherAppExtension(const QString & appPath, const QString & appName)
{
	QPushButton* btn = new QPushButton(this);//每个app都用一个小按钮
	btn->setFixedSize(20, 20);//图标固定大小 20x20

	//下面在按钮上设置图标
	QPixmap pixmap(btn->size());
	pixmap.fill(Qt::transparent);//透明填充

	QPainter painter(&pixmap);//用画家绘制,里面放绘图设备(在哪绘制 )
	QPixmap appPixmap(appPath);//算间隙
	//( btn->width() - appPixmap.width() )这个是间隙
	painter.drawPixmap((btn->width() - appPixmap.width()) / 2,
		(btn->height() - appPixmap.height()) / 2, appPixmap);
	btn->setIcon(pixmap);//对按钮设置图标
	btn->setIconSize(btn->size());//指定尺寸

	btn->setObjectName(appName);//设置对象名
	btn->setProperty("hasborder", true);//边框属性：有边框

	//仅对皮肤---data:2023.08.06
	connect(btn, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);
	return btn;
}

//初始化联系人树
void CCMainWindow::initContactTree()
{
	//单击、双击、展开与收缩时的信号
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onItemClicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(onItemExpanded(QTreeWidgetItem*)));
	connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(onItemCollapsed(QTreeWidgetItem*)));

	//根节点(用于分组)
	QTreeWidgetItem* pRootGroupItem = new QTreeWidgetItem;
	pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//不管有没有子项都进行展示（策略）
	pRootGroupItem->setData(0, Qt::UserRole, 0);//根项数据设为0

	RootContactItem* pItemName = new RootContactItem(true, ui.treeWidget); //自定义类 继承Label 实现文本和一些特定动画(旋转图片)
	QString strGroupName = QString::fromLocal8Bit("皇家434");//fromLocal8Bit将本地编码转换成utf8
	pItemName->setText(strGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootGroupItem);//顶级项（第一个）
	ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName);//1.根节点、2.列、3.设置的部件

	QStringList sCompDeps;//公司部门
	sCompDeps << QString::fromLocal8Bit("公司群");
	sCompDeps << QString::fromLocal8Bit("人事部");
	sCompDeps << QString::fromLocal8Bit("研发部");
	sCompDeps << QString::fromLocal8Bit("市场部");

	//批量添加
	for (int nIndex = 0; nIndex < sCompDeps.length(); nIndex++)
	{
		addCompanyDeps(pRootGroupItem, sCompDeps.at(nIndex));//根节点，
	}

}


//设置用户名大小事件
void CCMainWindow::resizeEvent(QResizeEvent * event)
{
	setUserName(QString::fromLocal8Bit("华南歌王滨"));//用常量字符串 本地--->unico
	BasicWindow::resizeEvent(event);
}

//事件过滤器返回值是bool,1.监视的对象 2.事件
bool CCMainWindow::eventFilter(QObject * obj, QEvent * event)
{
	if (ui.searchLineEdit == obj) //好友搜索是被监视的对象
	{
		if (event->type() == QEvent::FocusIn)//键盘焦点事件
		{
			//css语法
			QString sheet = "QWidget#searchWidget{background-color:rgb(255,255,255);border-bottom:1px solid rgba(%1,%2,%3,100)}\
							 QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png)}\
							 QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png)}\
							 QPushButton#searchBtn:pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhighdown.png)}";
			//设置颜色，和我们的背景色一样,当键盘焦点触发的时候
			ui.searchWidget->setStyleSheet(sheet
												.arg(m_colorBackGround.red())
												.arg(m_colorBackGround.green())
												.arg(m_colorBackGround.blue()));
		}
		//解决焦点不在后，样式没变化的bug
		else if (event->type() == QEvent::FocusOut)
		{
			updataSeachSheet();
		}
	}

	return false;
}

//点击，column列,点击分类，会展开
void CCMainWindow::onItemClicked(QTreeWidgetItem * item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		item->setExpanded(!item->isExpanded());//未展开则展开子项
	}
}

//双击	
void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column)
{


}

//拓展 --->bug 为什么没进来？可能信号槽没连接成功 --->再次点击，但没复原
//bug解决--->收缩，展开都只有一个参数
void CCMainWindow::onItemExpanded(QTreeWidgetItem * item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		//参1 哪一项，参2 哪一列   dynamic_cast c++用法 将基类对象指针（或引用）转换到继承类指针
		RootContactItem* prootItem = dynamic_cast <RootContactItem*>(ui.treeWidget->itemWidget(item,0));
		if (prootItem)
		{
			prootItem->setExpanded(true);//设置左边角度 尖尖的动画，转成90度
		}
	}
}

//伸缩
void CCMainWindow::onItemCollapsed(QTreeWidgetItem * item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		//参1 哪一项，参2 哪一列   dynamic_cast c++用法 将基类对象指针（或引用）转换到继承类指针
		RootContactItem* prootItem = dynamic_cast <RootContactItem*>(ui.treeWidget->itemWidget(item, 0));
		if (prootItem)
		{
			prootItem->setExpanded(false);//设置左边角度 尖尖的动画，转成90度
		}
	}
}


//点击app图标了
void CCMainWindow::onAppIconClicked()
{
	//换肤实现，在SkinWindow（另一个.cpp中实现） 
	//判断信号发送者的对象名是否是app_skin
	if (sender()->objectName() == "app_skin")
	{
		SkinWindow* skinWindow = new SkinWindow;
		skinWindow->show();
	}
}