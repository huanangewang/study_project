#include "WindowManager.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include <QSqlQueryModel>

//����ģʽ������ȫ�־�̬����
Q_GLOBAL_STATIC(WindowManager,theInstance)


WindowManager::WindowManager()
	:QObject(nullptr)
	,m_talkwindowshell(nullptr)
{
}

WindowManager::~WindowManager()
{
}

//���Ҵ�����
QWidget * WindowManager::findWindowName(const QString & qsWindowName)
{
	if (m_windowMap.contains(qsWindowName))
	{
		return m_windowMap.value(qsWindowName);//��ӳ������
	}
	
	return nullptr;
}

//���ݴ�������ɾ��
void WindowManager::deleteWindowName(const QString & qsWindowName)
{
	m_windowMap.remove(qsWindowName);//��ӳ����ɾ
}

//��Ӵ�����
void WindowManager::addWindowName(const QString & qsWindowName, QWidget * qWidget)
{
	if (!m_windowMap.contains(qsWindowName))//��ӳ������
	{
		m_windowMap.insert(qsWindowName, qWidget);//��ӳ���в�
	}
}

//���� -->һ��Ҫstatic,���ص�ǰʵ��
WindowManager * WindowManager::getInstance()
{
	return theInstance();
}

//Ϊ�˷���˽������
TalkWindowShell * WindowManager::getTalkWindowShell()
{
	return m_talkwindowshell;
}

//��ȡ���ڴ��������촰��id
QString WindowManager::getCreatingTalkId()
{
	return m_strCreatingTalkId;
}

//����µ����촰��-->�����ݿ���и�uid������
void WindowManager::addNewTalkWindow(const QString & uid)
{
	if (m_talkwindowshell == nullptr)
	{
		m_talkwindowshell = new TalkWindowShell;
		connect(m_talkwindowshell, &TalkWindowShell::destroyed, [this](QObject* obj) {
			m_talkwindowshell = nullptr;//TalkWindowShell������٣�����Ҳ�����ÿ�
		});
	}

	QWidget* widget = findWindowName(uid);//���ģ�uid����QQ�ţ�Ա���ţ���Ⱥ�ģ�Ⱥ��,���źţ�
	if (!widget)//�жϴ����Ƿ���ڣ������ھ͹���
	{
		m_strCreatingTalkId = uid;//���ڴ��������촰�ڣ�qq�ţ�

		TalkWindow* talkwindow = new TalkWindow(m_talkwindowshell, uid/*, groupType*/);
		TalkWindowItem* talkwindowItem = new TalkWindowItem(talkwindow);

		m_strCreatingTalkId = "";

		//�ж���Ⱥ�Ļ��ǵ���-->����ڲ��ű��У��ҵõ���¼����ô����Ⱥ�ģ������ǵ���
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID = %1").arg(uid);//�鲿�����Ͳ���ǩ��
		sqlDepModel.setQuery(strSql);//����ģ�Ͳ�ѯ���
		int rows = sqlDepModel.rowCount();//��ȡ���� -->��� ������0����ô�϶��ǵ���

		QString strWindowName, strMsgLabel;//Ҫ֪�����֣���ǩ��

		if (rows == 0)//���� //����ǵ���,sql����������
		{
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID = %1").arg(uid);
			sqlDepModel.setQuery(sql);//ģ�����ò�ѯ���
		}

		QModelIndex indexDepIndex, signIndex;//����������ǩ������
		indexDepIndex = sqlDepModel.index(0, 0);	//���� 0��0�У�������
		signIndex = sqlDepModel.index(0, 1);		//ǩ�� 0��1��(����)

		//���ﲻ̫��
		strWindowName = sqlDepModel.data(signIndex).toString();//���ô�������
		strMsgLabel = sqlDepModel.data(indexDepIndex).toString();//���������ϵ���ı�ǩ��

		talkwindow->setWindowName(strWindowName);//��������
		talkwindowItem->setMsgLabelContent(strMsgLabel);//�����ϵ���ı���ʾ
		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem,uid);

	}
	else//��Ч
	{
		QListWidgetItem* item = m_talkwindowshell->getTalkWindowItemMap().key(widget);//��ȡ����ߴ��ڵ�ֵ,��ͨ��ֵ����key
		item->setSelected(true);//��������б���Ϊѡ��(��ҪΪ�˸�bug:��CCMain��˫���������Զ���ת)
		
		m_talkwindowshell->setCurrentWidget(widget);//�����Ҳ൱ǰ���촰��
	}

	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();
}
