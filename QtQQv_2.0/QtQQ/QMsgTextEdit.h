#pragma once

#include <QTextEdit>

//Ϊ�˴��������������е�����飬Ȼ�����ͼƬ�������������У�����ҳ��ʽ��ʾ��Ҫ��ַ��

//�����Ҫ��TalkWindow.ui�н�������

class QMsgTextEdit : public QTextEdit
{
	Q_OBJECT
public:
	QMsgTextEdit(QWidget *parent);
	~QMsgTextEdit();

	//�ۺ���
private slots:
	void onEmotionImageFrameChange(int frame);//�����ı�ʱ��Ӧ�Ĳۺ�����frame����֡

	//����
public:
	void addEmotionUrl(int emotionNum);//��ӱ��飬����ҳ��ʽ��ʾ��ͨ������ҵ���ӦͼƬ
	void deletAllEmotionImage();//��Դ���գ�ɾ�����еı���ͼƬ��������ͺ󣬿��Ҫ���

	//����
private:
	QList<QString> m_listEmotionUrl;//�������ַ
	QMap<QMovie*, QString> m_emotionMap;//ӳ�䣺�����ͱ����ӳ���ϵ����ͼ���������ƣ�
};
