#include "WindowManager.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include "TalkWindowShell.h"
#include<QSqlQueryModel>

//单例模式，创建全局静态对象
Q_GLOBAL_STATIC(WindowManager,theInstance)


WindowManager::WindowManager()
	:QObject(nullptr)
	,m_talkwindowshell(nullptr)
{
}

WindowManager::~WindowManager()
{
}

//查找窗口名
QWidget * WindowManager::findWindowName(const QString & qsWindowName)
{
	if (m_windowMap.contains(qsWindowName))
	{
		return m_windowMap.value(qsWindowName);//在映射中找
	}
	
	return nullptr;
}

//根据窗口名称删除
void WindowManager::deleteWindowName(const QString & qsWindowName)
{
	m_windowMap.remove(qsWindowName);//在映射中删
}

//添加窗口名
void WindowManager::addWindowName(const QString & qsWindowName, QWidget * qWidget)
{
	if (!m_windowMap.contains(qsWindowName))//在映射中找
	{
		m_windowMap.insert(qsWindowName, qWidget);//在映射中插
	}
}

//单例 -->一定要static,返回当前实例
WindowManager * WindowManager::getInstance()
{
	return theInstance();
}

//添加新的聊天窗口-->有数据库后，有个uid就行了
void WindowManager::addNewTalkWindow(const QString & uid/*GroupType groupType, const QString & strPeople*/)
{
	if (m_talkwindowshell == nullptr)
	{
		m_talkwindowshell = new TalkWindowShell;
		connect(m_talkwindowshell, &TalkWindowShell::destroyed, [this](QObject* obj) {
			m_talkwindowshell = nullptr;//TalkWindowShell如果销毁，我们也进行置空
		});
	}

	QWidget* widget = findWindowName(uid);//单聊，uid就是QQ号（员工号），群聊（群号,部门号）
	if (!widget)//判断窗口是否存在，不存在就构建
	{
		TalkWindow* talkwindow = new TalkWindow(m_talkwindowshell, uid/*, groupType*/);
		TalkWindowItem* talkwindowItem = new TalkWindowItem(talkwindow);

		//判断是群聊还是单聊-->如果在部门表中，找得到记录，那么就是群聊，否则是单聊
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID = %1").arg(uid);//查部门名和部门签名
		sqlDepModel.setQuery(strSql);//设置模型查询语句
		int rows = sqlDepModel.rowCount();//获取行数 -->如果 行数是0，那么肯定是单聊

		QString strWindowName, strMsgLabel;//要知道名字，和签名

		if (rows == 0)//如果是单聊,sql语句更新重置
		{
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID = %1").arg(uid);//拿到单聊人的名字和个性签名
			sqlDepModel.setQuery(sql);//模型设置查询语句
		}

		QModelIndex indexDepIndex, signIndex; //部门索引，签名索引
		indexDepIndex = sqlDepModel.index(0, 0); //部门 0行0列（索引）
		signIndex = sqlDepModel.index(0, 1); //签名 0行1列(索引)
		
		//这里不太懂
	    strWindowName = sqlDepModel.data(signIndex).toString(); 
	    strMsgLabel = sqlDepModel.data(indexDepIndex).toString();

		talkwindow->setWindowName(strWindowName);//设置窗口名称
		talkwindowItem->setMsgLabelContent(strMsgLabel);//设置左侧联系人文本签名

		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem,uid);

		/*
		//根据分组类型
		switch (groupType) {
		case COMPANY:
		{
			//设置窗口名称  QStringLiteral和QString有什么区别?
			talkwindow->setWindowName(QStringLiteral("只要你努力，你就会发现不如开摆"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("公司群"));
			break;
		}
		case PERSONELGROUP:
		{
			talkwindow->setWindowName(QStringLiteral("公心、民心、爱心，事事在心"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("人事群"));
			break;
		}
		case MARKETGROUP:
		{
			talkwindow->setWindowName(QStringLiteral("今天工作不努力，明天努力找工作"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("市场群"));
			break;
		}
		case DEVELOPMENTGROUP:
		{
			talkwindow->setWindowName(QStringLiteral("只有两种编程语言：一种是天天挨骂的，另一种是没人用的"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("研发群"));
			break;
		}
		case PTOP://单聊
		{
			talkwindow->setWindowName(QStringLiteral(""));
			talkwindowItem->setMsgLabelContent(strPeople);
			break;
		}
		default:
			break;
		}

		//添加聊天窗口
		m_talkwindowshell->addTalkWindow(talkwindow,talkwindowItem, groupType);
	
	*/
	}
	else//有效
	{
		m_talkwindowshell->setCurrentWidget(widget);//设置右边当前聊天窗口
		QListWidgetItem* item = m_talkwindowshell->getTalkWindowItemMap().key(widget);//获取到左边窗口的值,是通过值来找key
		item->setSelected(true);//左侧聊天列表设为选择(主要为了改bug:在CCMain中双击，左侧会自动跳转)
	}

	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();
}
