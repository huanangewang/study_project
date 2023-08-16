#include "QMsgTextEdit.h"
#include <QMovie>
#include <QUrl>

QMsgTextEdit::QMsgTextEdit(QWidget *parent)
	: QTextEdit(parent)
{
}

QMsgTextEdit::~QMsgTextEdit()
{
	deletAllEmotionImage();//删除所有表情图片
}

//动画改变时响应的槽函数，frame动画帧 不太懂这里
void QMsgTextEdit::onEmotionImageFrameChange(int frame)
{
	//槽函数内调用sender(信号发送者)获取改变的图片,QUrl(m_emotionMap.value(movie))获取value,也就是获取表情的名称（路径）
	QMovie* movie = qobject_cast<QMovie*>(sender());

	document()->addResource(QTextDocument::ImageResource,
		QUrl(m_emotionMap.value(movie)), movie->currentPixmap());//参数3：模式
}

//添加表情，以网页格式显示，通过序号找到相应图片
void QMsgTextEdit::addEmotionUrl(int emotionNum)
{
	const QString& imageName = QString("qrc:/Resources/MainWindow/emotion/%1.png").arg(emotionNum);
	const QString& flagName = QString("%1").arg(imageName);//拼接

	insertHtml(QString("<img src='%1' />").arg(flagName));//插入网页(插了个表情),不太懂

	if (m_listEmotionUrl.contains(imageName))
	{
		return;
	}
	else
	{
		m_listEmotionUrl.append(imageName);//添加进List
	}

	QMovie* apngMovie = new QMovie(imageName, "apng", this);//动图创建
	m_emotionMap.insert(apngMovie, flagName);//添加键值到表情映射中

	//数据帧改变时发射的信号
	connect(apngMovie, SIGNAL(frameChanged(int)), this, SLOT(onEmotionImageFrameChange(int)));
	apngMovie->start();//让表情图片动起来
	updateGeometry();//更新几何形状
}

//资源回收，删除所有的表情图片，点击发送后，框框要清除
void QMsgTextEdit::deletAllEmotionImage()
{
	//主要对映射容器Map进行删除
	for (auto itor = m_emotionMap.constBegin();
		itor != m_emotionMap.constEnd(); ++itor)
	{
		delete itor.key();//删
	}

	m_emotionMap.clear();
}
