#include "NotifyManager.h"
#include "CommonUtils.h"

//初始化 实例
NotifyManager* NotifyManager::instance = nullptr;

NotifyManager::NotifyManager()
	:QObject(nullptr)
{
}

NotifyManager::~NotifyManager()
{
}

//获得实例
NotifyManager* NotifyManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new NotifyManager();
	}

	return instance;
}

//通知其他窗口
void NotifyManager::notifyOtherWindowChangeSkin(const QColor& color)
{
	emit signalSkinChanged(color);
	CommonUtils::setDefaultSkinColor(color);//改变默认皮肤颜色
}
