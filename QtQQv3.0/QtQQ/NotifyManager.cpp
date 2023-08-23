#include "NotifyManager.h"
#include "CommonUtils.h"

//��ʼ�� ʵ��
NotifyManager* NotifyManager::instance = nullptr;

NotifyManager::NotifyManager()
	:QObject(nullptr)
{
}

NotifyManager::~NotifyManager()
{
}

//���ʵ��
NotifyManager* NotifyManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new NotifyManager();
	}

	return instance;
}

//֪ͨ��������
void NotifyManager::notifyOtherWindowChangeSkin(const QColor& color)
{
	emit signalSkinChanged(color);
	CommonUtils::setDefaultSkinColor(color);//�ı�Ĭ��Ƥ����ɫ
}
