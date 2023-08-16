#include "WindowManager.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include "TalkWindowShell.h"
#include<QSqlQueryModel>

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

//����µ����촰��-->�����ݿ���и�uid������
void WindowManager::addNewTalkWindow(const QString & uid/*GroupType groupType, const QString & strPeople*/)
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
		TalkWindow* talkwindow = new TalkWindow(m_talkwindowshell, uid/*, groupType*/);
		TalkWindowItem* talkwindowItem = new TalkWindowItem(talkwindow);

		//�ж���Ⱥ�Ļ��ǵ���-->����ڲ��ű��У��ҵõ���¼����ô����Ⱥ�ģ������ǵ���
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID = %1").arg(uid);//�鲿�����Ͳ���ǩ��
		sqlDepModel.setQuery(strSql);//����ģ�Ͳ�ѯ���
		int rows = sqlDepModel.rowCount();//��ȡ���� -->��� ������0����ô�϶��ǵ���

		QString strWindowName, strMsgLabel;//Ҫ֪�����֣���ǩ��

		if (rows == 0)//����ǵ���,sql����������
		{
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID = %1").arg(uid);//�õ������˵����ֺ͸���ǩ��
			sqlDepModel.setQuery(sql);//ģ�����ò�ѯ���
		}

		QModelIndex indexDepIndex, signIndex; //����������ǩ������
		indexDepIndex = sqlDepModel.index(0, 0); //���� 0��0�У�������
		signIndex = sqlDepModel.index(0, 1); //ǩ�� 0��1��(����)
		
		//���ﲻ̫��
	    strWindowName = sqlDepModel.data(signIndex).toString(); 
	    strMsgLabel = sqlDepModel.data(indexDepIndex).toString();

		talkwindow->setWindowName(strWindowName);//���ô�������
		talkwindowItem->setMsgLabelContent(strMsgLabel);//���������ϵ���ı�ǩ��

		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem,uid);

		/*
		//���ݷ�������
		switch (groupType) {
		case COMPANY:
		{
			//���ô�������  QStringLiteral��QString��ʲô����?
			talkwindow->setWindowName(QStringLiteral("ֻҪ��Ŭ������ͻᷢ�ֲ��翪��"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("��˾Ⱥ"));
			break;
		}
		case PERSONELGROUP:
		{
			talkwindow->setWindowName(QStringLiteral("���ġ����ġ����ģ���������"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("����Ⱥ"));
			break;
		}
		case MARKETGROUP:
		{
			talkwindow->setWindowName(QStringLiteral("���칤����Ŭ��������Ŭ���ҹ���"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("�г�Ⱥ"));
			break;
		}
		case DEVELOPMENTGROUP:
		{
			talkwindow->setWindowName(QStringLiteral("ֻ�����ֱ�����ԣ�һ�������찤��ģ���һ����û���õ�"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("�з�Ⱥ"));
			break;
		}
		case PTOP://����
		{
			talkwindow->setWindowName(QStringLiteral(""));
			talkwindowItem->setMsgLabelContent(strPeople);
			break;
		}
		default:
			break;
		}

		//������촰��
		m_talkwindowshell->addTalkWindow(talkwindow,talkwindowItem, groupType);
	
	*/
	}
	else//��Ч
	{
		m_talkwindowshell->setCurrentWidget(widget);//�����ұߵ�ǰ���촰��
		QListWidgetItem* item = m_talkwindowshell->getTalkWindowItemMap().key(widget);//��ȡ����ߴ��ڵ�ֵ,��ͨ��ֵ����key
		item->setSelected(true);//��������б���Ϊѡ��(��ҪΪ�˸�bug:��CCMain��˫���������Զ���ת)
	}

	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();
}
