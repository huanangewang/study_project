#include "QMsgTextEdit.h"
#include <QMovie>
#include <QUrl>

QMsgTextEdit::QMsgTextEdit(QWidget *parent)
	: QTextEdit(parent)
{
}

QMsgTextEdit::~QMsgTextEdit()
{
	deletAllEmotionImage();//ɾ�����б���ͼƬ
}

//�����ı�ʱ��Ӧ�Ĳۺ�����frame����֡ ��̫������
void QMsgTextEdit::onEmotionImageFrameChange(int frame)
{
	//�ۺ����ڵ���sender(�źŷ�����)��ȡ�ı��ͼƬ,QUrl(m_emotionMap.value(movie))��ȡvalue,Ҳ���ǻ�ȡ��������ƣ�·����
	QMovie* movie = qobject_cast<QMovie*>(sender());

	document()->addResource(QTextDocument::ImageResource,
		QUrl(m_emotionMap.value(movie)), movie->currentPixmap());//����3��ģʽ
}

//��ӱ��飬����ҳ��ʽ��ʾ��ͨ������ҵ���ӦͼƬ
void QMsgTextEdit::addEmotionUrl(int emotionNum)
{
	const QString& imageName = QString("qrc:/Resources/MainWindow/emotion/%1.png").arg(emotionNum);
	const QString& flagName = QString("%1").arg(imageName);//ƴ��

	insertHtml(QString("<img src='%1' />").arg(flagName));//������ҳ(���˸�����),��̫��

	if (m_listEmotionUrl.contains(imageName))
	{
		return;
	}
	else
	{
		m_listEmotionUrl.append(imageName);//��ӽ�List
	}

	QMovie* apngMovie = new QMovie(imageName, "apng", this);//��ͼ����
	m_emotionMap.insert(apngMovie, flagName);//��Ӽ�ֵ������ӳ����

	//����֡�ı�ʱ������ź�
	connect(apngMovie, SIGNAL(frameChanged(int)), this, SLOT(onEmotionImageFrameChange(int)));
	apngMovie->start();//�ñ���ͼƬ������
	updateGeometry();//���¼�����״
}

//��Դ���գ�ɾ�����еı���ͼƬ��������ͺ󣬿��Ҫ���
void QMsgTextEdit::deletAllEmotionImage()
{
	//��Ҫ��ӳ������Map����ɾ��
	for (auto itor = m_emotionMap.constBegin();
		itor != m_emotionMap.constEnd(); ++itor)
	{
		delete itor.key();//ɾ
	}

	m_emotionMap.clear();
}
