#include "SkinWindow.h"
#include "QClickLabel.h"
#include "NotifyManager.h"

//修改父类成BasicWindow
SkinWindow::SkinWindow(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	loadStyleSheet("SkinWindow");
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();//初始化控件
}

SkinWindow::~SkinWindow()
{
}

//初始化控件
void SkinWindow::initControl()
{
	//颜色放容器(默认12种),s容器直接初始化
	QList<QColor> colorList = {
		QColor(22,154,218), QColor(40,138,221), QColor(49,166,107), QColor(218,67,68),
		QColor(177,99,158), QColor(107,81,92),  QColor(89,92,160),  QColor(21,156,199),
		QColor(79,169,172), QColor(155,183,154),QColor(128,77,77),  QColor(240,188,189)
	};
	
	//3行(row)，4列(column)
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			//自定义的类 QClickLabel ,重写了鼠标按下事件，实现了点击标签label,可以实现发送点击信号
			QClickLabel *label = new QClickLabel(this);//label设置颜色即可，也就是用label
			label->setCursor(Qt::PointingHandCursor);//设置光标形状，手指

			//这里需要单例去，通知其他窗口进行改变,捕获：行，列，颜色容器 instance是静态的，只有一份
			connect(label, &QClickLabel::clicked, [row, column, colorList]() {
				NotifyManager::getInstance()->notifyOtherWindowChangeSkin(colorList.at(row * 4 + column));
			});

			label->setFixedSize(84, 84);

			//设置选择标签页面的各个标签颜色
			QPalette palette;
			palette.setColor(QPalette::Background, colorList.at(row * 4 + column));
			label->setAutoFillBackground(true);//背景自动填充
			label->setPalette(palette);//设置调色板

			ui.gridLayout->addWidget(label, row, column);//构造完成后，还要记得放在布局管理器种
		}
	}

	//关闭和最小化连接信号槽
	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked()), this, SLOT(onShowClose()));
}

//重写关闭才有用-->为了关皮肤界面，而不是关整个运用程序
void SkinWindow::onShowClose()
{
	this->close();
}
