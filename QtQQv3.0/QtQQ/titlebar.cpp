#include "titlebar.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QFile>

#define BUTTON_HEIGHT 27	//��ť�߶�
#define BUTTON_WIDTH 27		//��ť���
#define TITLE_HEIGHT 27		//�������߶�

TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
	, m_isPressed(false)
	, m_buttonType(MIN_MAX_BUTTON)
{
	initControl();//��ʼ���������ؼ�
	initConnections();
	loadStyleSheet("Title");
}

TitleBar::~TitleBar()
{
}

//��ʼ���ؼ�
void TitleBar::initControl()
{
	//��ʼ��,ʵ��������
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonMin = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);

	//��ť���ù̶���С	27x27
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	//���ö�����
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");

	//���ò���	��ӵ�ˮƽ���ֹ�����QHBoxLayout
	QHBoxLayout *mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);

	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);

	mylayout->setContentsMargins(5, 0, 0, 0);	//���ò��ֵļ�϶
	mylayout->setSpacing(0);					//���ò����ﲿ����ļ�϶

	//�������ĳߴ����		Fixed��ֱ����̶�
	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setFixedHeight(TITLE_HEIGHT);				//�̶��������߶�
	setWindowFlags(Qt::FramelessWindowHint);	//�ޱ߿�
}

//�ź���۵�����
void TitleBar::initConnections()
{
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

//���ñ�����ͼ��
void TitleBar::setTitleIcon(const QString &filePath)
{
	QPixmap titleIcon(filePath);
	m_pIcon->setFixedSize(titleIcon.size());
	m_pIcon->setPixmap(titleIcon);
}

//���ñ���������
void TitleBar::setTitleContent(const QString &titleContent)
{
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

//���ñ������ĳ���
void TitleBar::setTitleWidth(int width)
{
	setFixedWidth(width);
}

//���ñ�������ť����
void TitleBar::setButtonType(ButtonType buttontype)
{
	m_buttonType = buttontype;

	switch (buttontype)
	{
	case MIN_BUTTON:
	{
		//���ò��ɼ�
		m_pButtonRestore->setVisible(false);
		m_pButtonMax->setVisible(false);
	}
	break;
	case MIN_MAX_BUTTON:
	{
		m_pButtonRestore->setVisible(false);
	}
	break;
	case ONLY_CLOSE_BUTTON:
	{
		m_pButtonRestore->setVisible(false);
		m_pButtonMax->setVisible(false);
		m_pButtonMin->setVisible(false);
	}
	break;
	default:
		break;
	}
}

//���洰�����ǰ���ڵ�λ�ü���С
void TitleBar::saveRestoreInfo(const QPoint &point, const QSize &size)
{
	m_restorePos = point;
	m_restoreSize = size;
}

//��ȡ�������ǰ���ڵ�λ�ü���С
void TitleBar::getRestoreInfo(QPoint &point, QSize &size)
{
	point = m_restorePos;
	size = m_restoreSize;
}

//���Ʊ����� ---- >�����¼�
//���ڽ����κθı䶼�ᴥ���ػ�
void TitleBar::paintEvent(QPaintEvent* event)
{
	//���ñ���ɫ
	QPainter painter(this);
	QPainterPath pathBack;					//��ͼ·��
	pathBack.setFillRule(Qt::WindingFill);	//����������
	pathBack.addRoundedRect(QRect(0, 0, width(), height()), 3, 3);//���Բ�Ǿ��ε���ͼ·��
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	//��������󻯻�ԭ�󣬴��ڳ��ȸı䣬��������Ӧ�����ı�
	//parentWidget()���ظ�����
	if (width() != parentWidget()->width())
	{
		setFixedWidth(parentWidget()->width());
	}

	QWidget::paintEvent(event);
}

//����¼�����
//˫����Ӧ�¼�����Ҫʵ��˫�����������������С���Ĳ���
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	//ֻ�д�����󻯡���С����ť����Ч
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		if (m_pButtonMax->isVisible())
			onButtonMaxClicked();
		else
			onButtonRestoreClicked();
	}
	//Ĭ�ϴ������
	return QWidget::mouseDoubleClickEvent(event);
}

//ͨ����갴�¡�����ƶ�������ͷ��¼�ʵ���϶��������ﵽ�ƶ�����Ч��
//�����󻯺�ԭ������󻯰�ť���ɻ�ԭ��ť
void TitleBar::mousePressEvent(QMouseEvent* event)
{
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		//�ڴ������ʱ��ֹ�϶�����
		if (m_pButtonMax->isVisible())
		{
			m_isPressed = true;
			m_startMovePos = event->globalPos();//globalPos()�����¼�����ʱ������ڵ�ȫ��λ��
		}
	}
	else
	{
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);
}

//����ƶ��¼�
void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
	if (m_isPressed)
	{
		QPoint movePoint = event->globalPos() - m_startMovePos;//Ҫ�ƶ�����
		QPoint widgetPos = parentWidget()->pos();
		m_startMovePos = event->globalPos();//����λ��
		parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
	}

	return QWidget::mouseMoveEvent(event);
}

//����ͷ��¼�
void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

//������ʽ��
void TitleBar::loadStyleSheet(const QString &sheetName)
{
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		setStyleSheet(styleSheet);
	}
}

//��С����� --->emit �����ź�
void TitleBar::onButtonMinClicked()
{
	emit signalButtonMinClicked();
}

//��ԭ���
void TitleBar::onButtonRestoreClicked()
{
	m_pButtonRestore->setVisible(false);
	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();
}

//��󻯵��
void TitleBar::onButtonMaxClicked()
{
	m_pButtonMax->setVisible(false);
	m_pButtonRestore->setVisible(true);
	emit signalButtonMaxClicked();
}

//�رյ��
void TitleBar::onButtonCloseClicked()
{
	emit signalButtonCloseClicked();
}