#pragma once

#include <QObject>

class NotifyManager : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();

signals:
	//皮肤改变的信号
	void signalSkinChanged(const QColor& color);

public:
	static NotifyManager* getInstance();//获得操作的静态 实例
	void notifyOtherWindowChangeSkin(const QColor& color);//通知其他窗口改变颜色

private:
	static NotifyManager* instance;//操作的静态 实例
};
