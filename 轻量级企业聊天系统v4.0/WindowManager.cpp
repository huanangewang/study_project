#include "WindowManager.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include <QSqlQueryModel>

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

//为了访问私有数据
TalkWindowShell * WindowManager::getTalkWindowShell()
{
	return m_talkwindowshell;
}

//获取正在创建的聊天窗口id
QString WindowManager::getCreatingTalkId()
{
	return m_strCreatingTalkId;
}

//添加新的聊天窗口-->有数据库后，有个uid就行了
void WindowManager::addNewTalkWindow(const QString & uid)
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
		m_strCreatingTalkId = uid;//正在创建的聊天窗口（qq号）

		TalkWindow* talkwindow = new TalkWindow(m_talkwindowshell, uid/*, groupType*/);
		TalkWindowItem* talkwindowItem = new TalkWindowItem(talkwindow);

		m_strCreatingTalkId = "";

		//判断是群聊还是单聊-->如果在部门表中，找得到记录，那么就是群聊，否则是单聊
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID = %1").arg(uid);//查部门名和部门签名
		sqlDepModel.setQuery(strSql);//设置模型查询语句
		int rows = sqlDepModel.rowCount();//获取行数 -->如果 行数是0，那么肯定是单聊

		QString strWindowName, strMsgLabel;//要知道名字，和签名

		if (rows == 0)//单聊 //如果是单聊,sql语句更新重置
		{
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID = %1").arg(uid);
			sqlDepModel.setQuery(sql);//模型设置查询语句
		}

		QModelIndex indexDepIndex, signIndex;//部门索引，签名索引
		indexDepIndex = sqlDepModel.index(0, 0);	//部门 0行0列（索引）
		signIndex = sqlDepModel.index(0, 1);		//签名 0行1列(索引)

		//这里不太懂
		strWindowName = sqlDepModel.data(signIndex).toString();//设置窗口名称
		strMsgLabel = sqlDepModel.data(indexDepIndex).toString();//设置左侧联系人文本签名

		talkwindow->setWindowName(strWindowName);//窗口名称
		talkwindowItem->setMsgLabelContent(strMsgLabel);//左侧联系人文本显示
		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem,uid);

	}
	else//有效
	{
		QListWidgetItem* item = m_talkwindowshell->getTalkWindowItemMap().key(widget);//获取到左边窗口的值,是通过值来找key
		item->setSelected(true);//左侧聊天列表设为选择(主要为了改bug:在CCMain中双击，左侧会自动跳转)
		
		m_talkwindowshell->setCurrentWidget(widget);//设置右侧当前聊天窗口
	}

	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();
}
