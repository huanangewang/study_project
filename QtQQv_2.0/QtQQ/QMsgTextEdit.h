#pragma once

#include <QTextEdit>

//为了处理，我们在聊天中点击表情，然后表情图片会出现在聊天框中，以网页格式显示（要网址）

//做完后要在TalkWindow.ui中进行升级

class QMsgTextEdit : public QTextEdit
{
	Q_OBJECT
public:
	QMsgTextEdit(QWidget *parent);
	~QMsgTextEdit();

	//槽函数
private slots:
	void onEmotionImageFrameChange(int frame);//动画改变时响应的槽函数，frame动画帧

	//方法
public:
	void addEmotionUrl(int emotionNum);//添加表情，以网页格式显示，通过序号找到相应图片
	void deletAllEmotionImage();//资源回收，删除所有的表情图片，点击发送后，框框要清除

	//数据
private:
	QList<QString> m_listEmotionUrl;//保存的网址
	QMap<QMovie*, QString> m_emotionMap;//映射：动画和表情的映射关系（动图，表情名称）
};
