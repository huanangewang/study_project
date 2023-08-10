#include "basicwindow.h"
#include "CommonUtils.h"
#include "NotifyManager.h"

#include <QDesktopWidget>
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	m_colorBackGround = CommonUtils::getDefaultSkinColor();//获取默认颜色值，读配置文件
	setWindowFlags(Qt::FramelessWindowHint);//设置窗口无边框
	setAttribute(Qt::WA_TranslucentBackground, true);//透明效果
	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)),
		this, SLOT(onSignalSkinChanged(const QColor&)));//改变皮肤

}

BasicWindow::~BasicWindow()
{
}

//皮肤更改
void BasicWindow::onSignalSkinChanged(const QColor &color)
{
	m_colorBackGround = color;//更新背景色
	loadStyleSheet(m_styleName);
}

//设置标题栏标题
void BasicWindow::setTitleBarTitle(const QString &title, const QString &icon)
{
	_titleBar->setTitleIcon(icon);//设置图标
	_titleBar->setTitleContent(title);//设置内容
}

//初始化标题栏
void BasicWindow::initTitleBar(ButtonType buttontype)
{
	_titleBar = new TitleBar(this);
	_titleBar->setButtonType(buttontype);
	_titleBar->move(0, 0);

	//先连接槽函数(将信号和槽连接起来) ---->再单独去实现槽函数里的内容-
	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

//加载样式表
void BasicWindow::loadStyleSheet(const QString &sheetName)
{
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);

	if (file.isOpen())
	{
		setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());//读取样式

		//获取用户当前的皮肤RGB值
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		//qss设置
		qsstyleSheet += QString("QWidget[titleskin=true]\
								{background-color:rgb(%1,%2,%3);\
								border-top-left-radius:4px;}\
								QWidget[bottomskin=true]\
								{border-top:1px solid rgba(%1,%2,%3,100);\
								background-color:rgba(%1,%2,%3,50);\
								border-bottom-left-radius:4px;\
								border-bottom-right-radius:4px;}")
			.arg(r).arg(g).arg(b);
		setStyleSheet(qsstyleSheet);
	}

	file.close();
}

//背景图
void BasicWindow::initBackGroundColor()
{
	QStyleOption opt;
	opt.init(this);

	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);//画刷
}

//子类化部件时，需要重写绘图事件设置背景图
void BasicWindow::paintEvent(QPaintEvent* event)
{
	initBackGroundColor();
	QDialog::paintEvent(event);
}


//头像转圆头像 src:图片来源  mask：圆头像 maskSize:头像大小
QPixmap BasicWindow::getRoundImage(const QPixmap &src, QPixmap &mask, QSize maskSize)
{
	if (maskSize == QSize(0, 0))
	{
		maskSize = mask.size();
	}
	else
	{
		//KeepAspectRatio尽可能大的矩形进行缩放,操作后的结果给mask
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	//保存转换后的图像
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));//指定一个尺寸maksSize
	painter.end();

	return QPixmap::fromImage(resultImage);
}

//槽函数实现
void BasicWindow::onShowClose(bool)
{
	close();
}

void BasicWindow::onShowMin(bool)
{
	showMinimized();
}

void BasicWindow::onShowHide(bool)
{
	hide();
}

//正常显示
void BasicWindow::onShowNormal(bool)
{
	show();
	activateWindow();//活动的
}

void BasicWindow::onShowQuit(bool)
{
	QApplication::quit();//应用程序退出
}

//鼠标移动事件
void BasicWindow::mouseMoveEvent(QMouseEvent *e)
{
	//左键,移动窗口
	if (m_mousePressed && (e->buttons() && Qt::LeftButton))
	{
		move(e->globalPos() - m_mousePoint);//当前 - 之前的(e->globalPos() 事件发生时的全局坐标,相对于屏幕左上角(0,0); )
		e->accept();//接受事件操作
	}
}

//鼠标按下事件
void BasicWindow::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		//pos() 事件发生时相对于窗口左上角(0,0)的偏移
		m_mousePressed = true;
		m_mousePoint = e->globalPos() - pos();
		e->accept();//表示当前鼠标事件进行一个接收
	}
}

//鼠标释放事件
void BasicWindow::mouseReleaseEvent(QMouseEvent*)
{
	m_mousePressed = false;
}

//槽函数实现
//最小化
void BasicWindow::onButtonMinClicked()
{
	if (Qt::Tool == (windowFlags() & Qt::Tool))
	{
		hide();
	}
	else
	{
		showMinimized();
	}
}

//复原
void BasicWindow::onButtonRestoreClicked()
{
	QPoint windowPos;//窗口位置
	QSize windowSize;//窗口大小
	_titleBar->getRestoreInfo(windowPos, windowSize);
	setGeometry(QRect(windowPos, windowSize));
}

//最大化
void BasicWindow::onButtonMaxClicked()
{
	//保存窗口最大化前的 位置和大小，以方便复原
	_titleBar->saveRestoreInfo(pos(), QSize(width(), height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3,
		desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(factRect);
}

//关闭
void BasicWindow::onButtonCloseClicked()
{
	close();
}