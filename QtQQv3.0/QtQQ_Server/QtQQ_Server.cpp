#include "QtQQ_Server.h"

#include <QFileDialog> //文件对话框


const int gtcpPort = 8888; //监听端口号
const int gudpPort = 6666;

QtQQ_Server::QtQQ_Server(QWidget *parent)
	: QDialog(parent)
	, m_pixPath("")
{
    ui.setupUi(this);

	if (!connectMysql())//连接数据库
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("数据库连接失败"));
		close();
		return;
	}

	setStatusMap();//状态映射
	setdepNameMap();//部门映射
	setOnlineMap();//在线状态映射
	initComboBoxData();//初始化组合框数据

	m_queryInfoModel = new QSqlQueryModel(this);
	m_queryInfoModel->setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees"));
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //不进行编辑触发，表格只读
	
	//初始化查询公司群所有员工信息
	m_employeeID = 0;
	m_depID = getCompDepID();
	m_compDepID = m_depID;


	updateTableData();//更新表格数据

	//用计时器触发更新我们的数据
	m_timer = new QTimer(this);
	m_timer->setInterval(1000);//设置间隔一千毫秒
	m_timer->start();
	connect(m_timer, &QTimer::timeout,this,&QtQQ_Server::onRefresh);//定时功能

	initTcpSocket();//初始化tcp套接字
	initUdpSocket();//初始化udp套接字

}

QtQQ_Server::~QtQQ_Server()
{}

//初始化tcp套接字
void QtQQ_Server::initTcpSocket()
{
	m_tcpServer = new TcpServer(gtcpPort);
	m_tcpServer->run();

	connect(m_tcpServer, &TcpServer::signalTcpMsgComes, this, &QtQQ_Server::onUDPbroadMsg);//接收到tcp客户端发来的信号后，要进行udp广播
}

//初始化udp套接字
void QtQQ_Server::initUdpSocket()
{
	m_udpSender = new QUdpSocket(this);

}

//获取公司群qq号
int QtQQ_Server::getCompDepID()
{
	return 2000;
}

//更新表格数据
void QtQQ_Server::updateTableData(int depID, int employeeID)
{
	ui.tableWidget->clear();
	if (depID && depID !=m_compDepID)//部门id是有效的--> 找该部门所有的员工 //不是公司群（不是2000）-->查询部门
	{
		m_queryInfoModel->setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees WHERE departmentID = %1").arg(depID));
	}
	else if (employeeID)//哪个员工--->精确查找
	{
		m_queryInfoModel->setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees WHERE employeeID = %1").arg(employeeID));
	}
	else  //公司群（是2000）-->查询所有
	{
		m_queryInfoModel->setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees"));
	}

	int rows = m_queryInfoModel->rowCount();//总行数（总记录数）
	int columns = m_queryInfoModel->columnCount(); //总列数（总字段数）
	QModelIndex index; //模型索引
	
	//设置表格行数、列数
	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);

	//设置表头
	QStringList headers;
	headers << QStringLiteral("部门")
			<< QStringLiteral("工号")
			<< QStringLiteral("员工姓名")
			<< QStringLiteral("员工签名")
			<< QStringLiteral("员工状态")
			<< QStringLiteral("员工头像")
			<< QStringLiteral("在线状态");
	ui.tableWidget->setHorizontalHeaderLabels(headers);

	//设置列等宽
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//准备工作差不多，开始添加数据
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < columns; j++)
		{
			index = m_queryInfoModel->index(i, j); //行、列
			QString strData = m_queryInfoModel->data(index).toString();//获取i行，j列的数据

			//获取字段名称,先拿出当前 行 的记录
			QSqlRecord record = m_queryInfoModel->record(i); //对应行 i,当前行的记录
			QString depRecordName = record.fieldName(j);//列，获取记录的名称

			if (depRecordName == QLatin1String("departmentID"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_depNameMap.value(strData)));//将所谓的2001转成对应的中文名字
				continue;//直接进入下一个循环
			}
			else if (depRecordName == QLatin1String("status"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_statusMap.value(strData)));
				continue;//直接进入下一个循环
			}
			else if (depRecordName == QLatin1String("online"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_onlineMap.value(strData)));
				continue;//直接进入下一个循环
			}

			//普通的添加
			ui.tableWidget->setItem(i, j, new QTableWidgetItem(strData));   //new QTableWidgetItem(strData)是 向导，要new出来

		}
	}

}
//状态映射
void QtQQ_Server::setStatusMap()
{
	m_onlineMap.insert(QStringLiteral("1"), QStringLiteral("离线"));
	m_onlineMap.insert(QStringLiteral("2"), QStringLiteral("在线"));
	m_onlineMap.insert(QStringLiteral("3"), QStringLiteral("忙碌"));//暂时未设计

}

//部门映射
void QtQQ_Server::setdepNameMap()
{
	m_depNameMap.insert(QStringLiteral("2001"), QStringLiteral("人事部"));
	m_depNameMap.insert(QStringLiteral("2002"), QStringLiteral("研发部"));
	m_depNameMap.insert(QStringLiteral("2003"), QStringLiteral("市场部"));

}

//在线状态映射
void QtQQ_Server::setOnlineMap()
{
	m_statusMap.insert(QStringLiteral("1"), QStringLiteral("有效"));
	m_statusMap.insert(QStringLiteral("0"), QStringLiteral("已注销"));

}

//初始化组合框的数据(有两个组合框)
void QtQQ_Server::initComboBoxData()
{
	QString itemText;//组合框项的文本
	//获取公司总的部门个数
	QSqlQueryModel queryDepModel;
	queryDepModel.setQuery(QString::fromLocal8Bit("SELECT * FROM tab_department"));
	int depCounts = queryDepModel.rowCount() - 1; //-1是为了减去公司群

	//上面那个组合框
	for (int i = 0; i < depCounts;i++)
	{
		itemText = ui.employeeDepBox->itemText(i); //索引0开始, 拿到 '人事部' '研发部'等等....
		QSqlQuery queryDepID(QString::fromLocal8Bit("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(itemText));//拿部门id
		queryDepID.exec();//执行
		queryDepID.first();//指向第一条

		//给员工所属部门组合框设置数据为相应的部门id号
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());//0是因为只有第一列，若干行
	}

	//下面那个组合框
	for (int i = 0;i < depCounts+1;i++)//+1是因为我可以通过公司群筛选
	{
		itemText = ui.departentBox->itemText(i);
		QSqlQuery queryDepID(QString::fromLocal8Bit("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(itemText));//拿部门id
		queryDepID.exec();//执行
		queryDepID.first();//指向第一条

	    //设置部门组合框数据为相应的id号
		ui.departentBox->setItemData(i, queryDepID.value(0).toInt());//0是因为只有第一列，若干行

	}

}

//刷新表格数据-->通过定时器
void QtQQ_Server::onRefresh()
{
	updateTableData(m_depID, m_employeeID);
}

//查询部门按钮点击(根据群qq号)
void QtQQ_Server::on_queryDepartmentBtn_clicked()
{
	m_employeeID = 0;//为了安全
	ui.queryIdLineEdit->clear();//为了好看，清空下面的那行数据

	m_depID = ui.departentBox->currentData().toInt();//因为有单独的函数实现了，这个控件的初始化，所以我们直接获取这里的id即可
	updateTableData(m_depID);//只传入部门id
}

//查询员工按钮点击(根据员工qq号查)
void QtQQ_Server::on_queryIdBtn_clicked()
{
	m_depID = m_compDepID;

	//检测员工qq号是否输入
	if (!ui.queryIdLineEdit->text().length())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("输入qq号为空"));
		ui.queryIdLineEdit->setFocus();//设置焦点
		return;
	}

	int employeeID = ui.queryIdLineEdit->text().toInt();//获取用户输入的员工号
	QSqlQuery queryID(QString("SELECT * FROM tab_employees WHERE employeeID = %1").arg(employeeID));
	queryID.exec();
	//queryID.first();

	if (!queryID.first())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入正确的员工号!"));
		ui.queryIdLineEdit->setFocus();//设置焦点
		return;
	}
	else //查到了
	{
		m_employeeID = employeeID; //因为在定时器定时刷新了，所以我们只需要更新他的值就行
	}
}

//注销按钮-->将数据库里的数据进行改变状态即可
void QtQQ_Server::on_logoutBtn_clicked()
{
	ui.queryIdLineEdit->clear();
	//检测员工qq号是否输入
	if (!ui.logoutIdLineEdit->text().length())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("输入qq号为空"));
		ui.logoutIdLineEdit->setFocus();//设置焦点
		return;
	}

	int employeeID = ui.logoutIdLineEdit->text().toInt();//获取用户输入的员工号
	QSqlQuery queryID(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1").arg(employeeID));
	queryID.exec();

	if (!queryID.first()) //查不到，提示
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入正确的员工号!"));
		ui.logoutIdLineEdit->setFocus();//设置焦点
		return;
	}
	else //查到了，注销-->将员工status 设置成0
	{
		//设置status，在表tab_employees中进行更新
		QSqlQuery sqlUpdate(QString("UPDATE tab_employees SET status = 0 WHERE employeeID = %1").arg(employeeID));
		sqlUpdate.exec();//执行

		////获取注销员工的姓名-->通过id号查数据库
		//QSqlQuery sqlQueryName(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1 ").arg(employeeID));
		QString strName = queryID.value(0).toString();

		//fromLocal8Bit VS中需要从本地编码转成uinco
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), 
			QString::fromLocal8Bit("员工：%1 的企业qq：%2 已被注销!")
			.arg(strName)
			.arg(employeeID));
		
		ui.logoutIdLineEdit->clear();
	}

}

//选择图片按钮(员工头像)//1、进行服务端显示图片的功能：，并修改类变量m_pixPath
void QtQQ_Server::on_selectPictureBtn_clicked()
{
	//图片路径进行记录保存。
	m_pixPath = QFileDialog::getOpenFileName(
		this,
		QString::fromLocal8Bit("选择头像"),
		".",//当前目录
		"*.png;;*.jpg" //格式
	);

	if (!m_pixPath.size())
	{
		//路径没有
		return;
	}

	//将头像显示到标签
	QPixmap pixmap;
	pixmap.load(m_pixPath);
	//为了匹配大小 -->这里不太懂 -->这里算比例:	widthRatio   heightRatio
	qreal widthRatio = (qreal)ui.headLabel->width() / (qreal)pixmap.width();
	qreal heightRatio = (qreal)ui.headLabel->height() / (qreal)pixmap.height();

	QSize size(pixmap.width() * widthRatio, pixmap.height()* heightRatio);//计算大小尺寸
	ui.headLabel->setPixmap(pixmap.scaled(size));//进行缩放,并将图片路径放入
}

//添加员工按钮 //2、插入数据功能 
//--->其实不仅仅要加入到表tab_employees中，还要加入到tab_accounts中，要不然登录不了客户端
void QtQQ_Server::on_addBtn_clicked()
{
	//检测员工姓名输入
	QString strName = ui.nameLineEdit->text();
	if (!strName.size())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入员工姓名！"));
		ui.nameLineEdit->setFocus();
		return;
	}

	//检测员工头像是否存在
	if (!m_pixPath.size())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选择员工头像路径！"));
		return;
	}

	//开始插入数据
	//获取员工QQ号-->最大的号
	QSqlQuery queryMaxEmployeeID(QString("SELECT MAX(employeeID) FROM tab_employees"));
	queryMaxEmployeeID.exec();
	queryMaxEmployeeID.first();

	int employeeID = queryMaxEmployeeID.value(0).toInt() + 1;//增1
	int depID = ui.employeeDepBox->currentData().toInt(); //部门id

	//将图片路径设置为xxx\xxx.png的格式，统一格式
	m_pixPath.replace("/","\\\\");//这里要转义,这4个斜杠代表了一个'\'

	QSqlQuery insertSql(QString("INSERT INTO tab_employees(departmentID,employeeID,employee_name,picture) VALUES(%1,%2,'%3','%4')")
					.arg(depID) //部门id
					.arg(employeeID) //员工id
					.arg(strName) //员工名字
					.arg(m_pixPath)); //头像路径

	//也要添加到表tab_accounts中，用于登录
	QSqlQuery tab_accounts_Insert(QString("INSERT INTO tab_accounts(employeeID,account,code) VALUES(%1,'%2','%3')")
					.arg(employeeID) //员工id
					.arg(strName)//员工名字(用英文)
					.arg(strName)); //密码和名字相同

	insertSql.exec();//执行
	tab_accounts_Insert.exec();

	QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("新增员工成功！"));
	
	m_pixPath = "";//清空
	ui.headLabel->setText(QStringLiteral("员工寸照"));//清空头像


}

//接收到客户端信息后，要广播的槽函数
void QtQQ_Server::onUDPbroadMsg(QByteArray& btData)
{
	//为了让每个客户端的端口不一样，前期测试200个，只能在一台主机上，没办法
	for (quint16 port = gudpPort; port < gudpPort + 200;  ++port)
	{
		m_udpSender->writeDatagram(btData, btData.size(), QHostAddress::Broadcast, port);//广播的方式,写报文
	}
}

//连接数据库
bool QtQQ_Server::connectMysql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");//数据库名
	db.setHostName("localhost");//主机名
	db.setUserName("root");//用户名
	db.setPassword("root");//密码
	db.setPort(3306);//端口

	if (db.open())
	{
		return true; //正确打开
	}
	else
	{
		return false;
	}
}