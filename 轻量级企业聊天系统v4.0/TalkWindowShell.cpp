#include "TalkWindowShell.h"
#include "EmotionWindow.h"
#include "TalkWindow.h"
#include "CommonUtils.h"
#include <qlistwidget.h>
#include "TalkWindowItem.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFile>
#include <QSqlQuery>
#include "WindowManager.h"
#include "ReceiveFile.h"
#include"PoolTask.h"
#include<QThreadPool>

extern QString gLoginEmployeeID;
const int gUdpPort = 6666;

QString gfileName; //文件内容
QString gfileData;//文件内容

//修改基类 BasicWindow
TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//设置属性，当窗口关闭，要进行资源回收
	initControl();//初始化控件
	initTcpSocket();//初始化tcp
	initUdpSocket();//初始化udp


	QFile file("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size())//如果js中没数据，则写
	{
		QStringList employeesIdList;
		getEmployeesID(employeesIdList);//直接传值，这里是引用传参，会改变employeesIdList的
		if (!createJSFile(employeesIdList))//写JS文件
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("更新js文件失败"));
		}
	}
}

TalkWindowShell::~TalkWindowShell()
{
	delete m_emotionWindow;
	m_emotionWindow = nullptr;
}

//添加聊天-->聊天窗口  ,参数2 ：你在和哪一项聊天
//两个情况，一对一，一对多
void TalkWindowShell::addTalkWindow(TalkWindow* talkWindow, TalkWindowItem * talkWindowItem, const QString& uid/* GroupType grouptype*/)
{
	ui.rightStackedWidget->addWidget(talkWindow);//双击后，添加一个聊天窗口
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()),
		talkWindow, SLOT(onSetEmotionBtnStatus()));// onSetEmotionBtnStatus设置表情按钮状态

	QListWidgetItem* aItem = new QListWidgetItem(ui.listWidget);//左侧列表
	m_talkwindowItemMap.insert(aItem, talkWindow); //key:aItem ；value:talkWindow

	aItem->setSelected(true);//被选中

	//通过数据库设置头像
	//判断是群聊还是单聊
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(uid);
	sqlDepModel.setQuery(strQuery);//设置查询，看看有没有结果
	int rows = sqlDepModel.rowCount(); //0行就是单聊，否则群聊

	if (rows == 0)//单聊,要更改sql语句
	{
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid);
		sqlDepModel.setQuery(strQuery);
	}

	QModelIndex index;//模型索引
	index = sqlDepModel.index(0, 0);//行，列，获得头像的索引路径

	QImage img;
	img.load(sqlDepModel.data(index).toString());
	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));//设置头像 TODO:改成Pixmap

	ui.listWidget->addItem(aItem);//添加列表项(左边)
	ui.listWidget->setItemWidget(aItem, talkWindowItem);//设置部件

	onTalkWindowItemClicked(aItem);//当前这项被点击

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked,
		[talkWindowItem, talkWindow, aItem, this]()
	{
		//要将映射移除，列表项移除，聊天窗口关闭，talkWindowItem资源释放
		//先处理左边
		m_talkwindowItemMap.remove(aItem);
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));
		delete talkWindowItem;
		//处理右边
		ui.rightStackedWidget->removeWidget(talkWindow);
		if (ui.rightStackedWidget->count() < 1)
		{
			this->close();
		}
	});
}

//设置当前聊天的窗口(和哪个窗口进行聊天)
void TalkWindowShell::setCurrentWidget(QWidget * widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

//为了在外部访问这个私有成员
const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::getTalkWindowItemMap() const
{
	return m_talkwindowItemMap;
}

//初始化控件
void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");//加载样式表
	setWindowTitle(QString::fromLocal8Bit("帅滨-聊天窗口"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();		//隐藏表情窗口

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;//左边154个像素，右边...
	ui.splitter->setSizes(leftWidgetSize);	//分类器设置尺寸

	ui.listWidget->setStyle(new CustomProxyStyle(this));//设置风格

	//左侧被点击的话，或者表情被选中
	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}

//初始化tcp套接字 --->1对1的
void TalkWindowShell::initTcpSocket()
{
	m_tcpClientSocket = new QTcpSocket(this);
	m_tcpClientSocket->connectToHost("127.0.0.1", gtcpPort);//与服务器主机建立连接 gtcpPort:6666

}

//初始化udp套接字 --->只绑定端口，进行数据接收,谁发来都可以，只要端口有数据
void TalkWindowShell::initUdpSocket()
{
	m_udpReceiver = new QUdpSocket(this);

	//多端口进行接收
	for (quint16 port = gUdpPort; port < gUdpPort + 200; ++port)
	{
		if (m_udpReceiver->bind(port, QUdpSocket::ShareAddress))//端口、模式
		{
			break;//绑定成功后返回
		}
	}
	//一旦端口上有数据来了，就进行数据处理 
	//TODO:进行线程池扔任务即可，线程池内自动启子线程执行 2023-08-30
	connect(m_udpReceiver, &QUdpSocket::readyRead, this, &TalkWindowShell::onProcessPendingData);
}


//获取所有员工QQ号,为了改变employeesIdList，这里修改成引用传参
void TalkWindowShell::getEmployeesID(QStringList& employeesIdList)
{
	//之前写了个，返回局部变量的引用，自然没用
	QSqlQueryModel queryModel;
	queryModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status = 1"));//查找状态为1 的员工
	int employeesNum = queryModel.rowCount(); //看看有几个员工(id)
	QModelIndex index; //模型要拿数据，还需要索引
	//添加到List列表中
	for (int i = 0;i < employeesNum;i++)
	{
		index = queryModel.index(i, 0); // 行，列
		employeesIdList << queryModel.data(index).toString(); //拿数据,添加到链表中
	}

}

//创建（写JS文件）
bool TalkWindowShell::createJSFile(QStringList& employeesList)
{
	//读取.txt文件数据
	QString strFileTxt = "Resources/MainWindow/MsgHtml/msgtmpl.txt"; //先读原始数据
	QFile fileRead(strFileTxt);//将读到的txt数据读到文件中
	QString strFile;

	if (fileRead.open(QIODevice::ReadOnly))
	{
		strFile = fileRead.readAll();
		fileRead.close();
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("你可能未加载js文件，读取msgtmpl.txt失败"));
		return false;
	}

	//替换数据前，先拿原始数据
	//替换数据（external0,appendHtml0 用作追加信息的脚本对象，也就是自己发信息用的，不进行替换）
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");//写入js的目标地址(js信息 更新在这个位置)

	if (fileWrite.open(QIODevice::WriteOnly) | QIODevice::Truncate) //后面的意思是，进行覆盖
	{
		//下面拿原始数据：strSourceInitNull ;strSourceInit; strSourceNew ;strSourceRecvHtml
		QString strSourceInitNull = "var external = null;";//更新空值
		QString strSourceInit = "external = channel.objects.external;";//通道的对象,//初始值
		//更新newWebChannel
		QString strSourceNew =
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
		}\
		); \
		";
		//更新追加的脚本 --> 这里直接读recvHtml.txt就行(读文件方式) ,因为这里的脚本中有双引号(特殊情况特殊处理)
		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly))
		{
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("读取recvHtml.txt失败"));
			return false;
		}

		//保存替换后的脚本
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;

		//遍历每个员工号
		for (int i = 0;i < employeesList.length();i++)
		{
			//编辑替换后的空值
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i))); //%1用员工号id替换
			strReplaceInitNull += strInitNull;//将替换后的结果，放入strReplaceInitNull
			strReplaceInitNull += "\n";//换行

			//编辑替换后的初始值
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i))); //%1用员工号id替换
			strReplaceInit += strInit;//将替换后的结果，放入strReplaceInit
			strReplaceInit += "\n";//换行

			//编辑替换后的newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i))); //%1用员工号id替换
			strReplaceNew += strNew;//将替换后的结果，放入strReplaceNew
			strReplaceNew += "\n";//换行

			//编辑替换后的 recvHtml
			QString strRecvHtml = strSourceRecvHtml;
			strRecvHtml.replace("external", QString("external_%1").arg(employeesList.at(i))); //%1用员工号id替换
			strRecvHtml.replace("external", QString("external_%1").arg(i));//让脚本名称也不一样，不然运行都是同一个脚本 
			strReplaceRecvHtml += strRecvHtml; //将替换后的结果，放入strReplaceRecvHtml
			strReplaceRecvHtml += "\n";//换行
		}

		//开始真正的替换 ，替换完后，strFile中就是我们需要的最终数据了
		strFile.replace(strSourceInitNull, strReplaceInitNull);
		strFile.replace(strSourceInit, strReplaceInit);
		strFile.replace(strSourceNew, strReplaceNew);
		strFile.replace(strSourceRecvHtml, strReplaceRecvHtml);

		QTextStream stream(&fileWrite); //操作写的文件 fileWrite是QFile文件,之前已经已只写的方式打开，并覆盖
		stream << strFile;//写入
		fileWrite.close();
		return true;

	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("写 msgtmpl.js 失败"));
		return false;
	}

}

//处理收到的信息，整到网页上
void TalkWindowShell::handleReceivedMsg(int senderEmployeeID, int msgType, QString strMsg)
{
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setText(strMsg);

	if (msgType == 1)//文本信息
	{
		msgTextEdit.document()->toHtml();
	}
	else if (msgType == 0)//表情信息
	{
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;

		for (int i = 0; i < emotionNum; i++)
		{
			msgTextEdit.addEmotionUrl(strMsg.mid(i * emotionWidth, emotionWidth).toInt());
		}
	}

	QString html = msgTextEdit.document()->toHtml();

	//文本html如果没有字体则添加字体
	if (!html.contains(".png") && !html.contains("</span>"))
	{
		QString fontHtml;
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			fontHtml.replace("%1", strMsg);
			file.close();
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示")
				, QString::fromLocal8Bit("文件 msgFont.txt 不存在！"));
			return;
		}

		if (!html.contains(fontHtml))
		{
			html.replace(strMsg, fontHtml);
		}
	}

	TalkWindow* talkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	talkWindow->ui.msgWidget->appendMsg(html, QString::number(senderEmployeeID));

}

//表情按钮被点击
void TalkWindowShell::onEmotionBtnClicked(bool)
{
	//主要是表情窗口可见，还是不可见
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//将当前控件的相对位置，转换为屏幕的绝对位置

	emotionPoint.setX(emotionPoint.x() + 170);//偏移表情窗口，为了好看
	emotionPoint.setY(emotionPoint.y() + 220);

	m_emotionWindow->move(emotionPoint);
}

/*
1、文本数据包格式：群聊标志 + 发信息员工的id号(qq号) + 收消息（接收信息人的id号）（或者群qq号）
 +信息类型(文本、表情、文件) + 数据长度 + 数据
	 msgType:0-->表情信息  1-->文本信息 2-->文件信息
2、表情数据包格式：群聊标志 + 发信息员工的id号(qq号)+ 收消息（接收信息人的id号）（或者群qq号）
 +信息类型(文本、表情、文件) + 表情个数 + images + 数据
*/

//收到TalkWindow 发来的发送信息 信号后， 要实现的槽函数
void TalkWindowShell::updateSendTcpMsg(QString& strData, int& msgType, QString fileName)
{
	//先获取你在哪里聊天窗口  拿到数据包，定义格式，到时候方便其他客户端接收数据的时候进行解析
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget()); //获得右边的活动窗口
	QString talkId = curTalkWindow->getTalkId();

	QString strGroupFlag;//群聊标志
	QString strSend; //发送的数据

	if (talkId.length() == 4) //这是群qq号的长度
	{
		strGroupFlag = "1";//是,talkId是群qq号
	}
	else
	{
		strGroupFlag = "0";//不是,talkId就是某个员工的qq号
	}
	//2023-08-18 Bug解决
	int nstrDataLength = strData.length();//发hellowrold，这里就是10,原始数据长度
	int dataLength = QString::number(nstrDataLength).length();//数据长度 "10" 其实应该所占字符串也就两个，而不应该是10
	//const int sourceDataLength = dataLength;//原始数据长度
	QString strdataLength; //保存数据长度

	if (msgType == 1)//发送的是文本信息
	{
		//文本信息长度约定为5-->不够就用0补足5位
		if (dataLength == 1)
		{
			strdataLength = "0000" + QString::number(nstrDataLength); //补零
		}
		else if (dataLength == 2)
		{
			strdataLength = "000" + QString::number(nstrDataLength); //补零
		}
		else if (dataLength == 3)
		{
			strdataLength = "00" + QString::number(nstrDataLength); //补零
		}
		else if (dataLength == 4)
		{
			strdataLength = "0" + QString::number(nstrDataLength); //补零
		}
		else if (dataLength == 5)
		{
			strdataLength = QString::number(nstrDataLength);
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("不合理的数据长度"));
		}

		//组织数据包
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strdataLength + strData;
	}
	else if (msgType == 0)//发送的是表情信息
	{
		//组织数据包
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData; //strData中已经包含了 表情个数 + images + 数据
	}
	else if (msgType == 2)//文件信息
	{   /*
			文件数据包格式：群聊标志 + 发信息员工qq号 + 收消息员工qq号（群qq号）+
			消息类型(2) + 文件长度 + "bytes "+文件名称 + "data_begin" + 文件内容
		*/

		QString strLength = QString::number(strData.toUtf8().length()); //获取字节数组,并获得它的长度,toUtf8：将QString转成字节数组
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2" + strLength + fileName + "data_begin" + strData;//组织数据包
	}

	//前面组好包后，就要真正向客户端写入数据，都用字节数组形式
	QByteArray dataBt;
	dataBt.resize(strSend.length());//重置字节数组长度
	dataBt = strSend.toUtf8();//toUtf8：将QString转成字节数组
	m_tcpClientSocket->write(dataBt);//开始写数据（核心代码）


}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem * item)
{
	QWidget* talkwindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkwindowWidget);
}

//选了哪个表情
void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	//方法都在TalkWindow里,默认返回是部件类型，这里强转
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());

	if (curTalkWindow)
	{
		//添加表情图片
		curTalkWindow->addEmotionImage(emotionNum);

	}
}

/*  数据包的格式约定：

	文本数据包格式：群聊标志 + 发信息员工的id + 收消息员工id号（也可能是群id号）+ 信息类型(1) + 数据长度+数据;
	表情数据包格式：群聊标志 + 发信息员工的id + 收消息员工id号（也可能是群id号）+ 信息类型(0) + 表情个数 + images +表情名称(3位数);
	文件数据包格式：群聊标志 + 发信息员工的id + 收消息员工id号（也可能是群id号）+ 信息类型(2)
	+ 文件字节数 + bytes +文件名 + data_begin +文件数据;

	群聊标志占1位：0表示单聊，1表示群聊
	信息类型占1位，0表示表情信息，1表示文本信息，2表示文件信息
	qq号占5位，qq群号占4位，数据长度占5位，表情名称占3位
	（当群聊标志为1，则数据包中无收消息人的qq号，而是收消息 群qq号,
	当群聊标志为0，则数据包中没用收消息群qq号，而是收消息的人的qq号

	比如：群聊文本信息：1 10001 2001 1 00005 Hello 表示10001这个人向群2001 发送文本信息 Hello
		  单聊图片信息：0 10001 10002 0 1 images 060        表示QQ10001向QQ10002发送表情60.png
		  群聊文件信息：1 10005 2000 2 10 bytes test.txt data_begin   表示qq10005向群2000发送文件(test.txt)信息
*/


//处理服务器udp广播收到的数据(解析)
void TalkWindowShell::onProcessPendingData()
{
	while (m_udpReceiver->hasPendingDatagrams())//udp端口中是否有未处理的数据 bool
	{
		const static int groupFlagWidth = 1;	//群聊标志占位宽度永远为1
		const static int groupWidth = 4;		//群qq号宽度
		const static int employeeWidth = 5;		//员工qq号宽度
		const static int msgTypeWidth = 1;		//信息类型宽度
		const static int msgLengthWidth = 5;	//文本信息长度的宽度
		const static int pictureWidth = 3;		//表情图片宽度
	
		//拿出数据
		QByteArray btData;
		btData.resize(m_udpReceiver->pendingDatagramSize());//未处理数据的大小(即将要处理的数据大小)
		m_udpReceiver->readDatagram(btData.data(), btData.size());//读取udp数据(数据、大小)
	
		QString strData = btData.data();//拿到全部数据
		QString strWindowID;//聊天窗口ID，群聊就是群号，单聊就是员工id号
		QString strSendEmployeeID, strRecevieEmployeeID;//发送及接收端的qq号
		QString strMsg;//数据
	
		int msgLen;//数据长度
		int msgType;//数据类型
	
		strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);//截取,从群聊标志开始，截取id号长度
	
		//不仅别人发给你的，还有自己发出的信息，也可以被服务器广播给自己，这种情况不处理
		if (strSendEmployeeID == gLoginEmployeeID)
		{
			return;
		}
	
		if (btData[0] == '1') //群聊
		{
			//拿到群聊窗口名字id
			strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth); //通过上面的定义的宽度和结合数据包格式进行截取
	
			//判断信息类型,因为是QChar类型，所以用'1'单引号
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];
			if (cMsgType == '1')//文本信息
			{
				msgType = 1;//消息类型
				msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();//获取那占位为5的信息长度是多少
	
				//提取信息--》通过截取，拿到了信息内容
				strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (cMsgType == '0')//表情信息
			{
				msgType = 0;
				int posImages = strData.indexOf("images");//返回这个子字符串第一个出现的位置
				//int imagesWidth = QString("images").length();
	
				//提取信息,截取到右边(表情名 如:077)
				strMsg = strData.right(strData.length() - posImages - QString("images").length());
	
			}
			else if (cMsgType == '2')//文件信息
			{
				msgType = 2;
				int byteWidth = QString("bytes").length();//bytes长度
				int posBytes = strData.indexOf("bytes");//bytes的位置，返回子串第一个出现的位置
				int posData_begin = strData.indexOf("data_begin"); //data_begin的位置，返回子串第一个出现的位置
				int data_beginWidth = QString("data_begin").length();
	
				//截取文件名称 ,通过data_begin 和 bytes 之间夹着的 ，就是文件名称
				QString fileName = strData.mid(posBytes + byteWidth, posData_begin - posBytes - byteWidth);
				gfileName = fileName;//赋给全局变量
	
				//截取文件内容
				int dataLengthWidth;
				int posData = posData_begin + data_beginWidth;//真正文件数据的位置
				strMsg = strData.mid(posData);//提取数据
				gfileData = strMsg;//赋给全局变量
	
				//根据employeeID获取发送者姓名
				QString senderName;
				int employeeID = strSendEmployeeID.toInt();
				//查发送者姓名通过查数据，id
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID =%1").arg(employeeID));
				querySenderName.exec();
				if (querySenderName.first()) //查到了
				{
					senderName = querySenderName.value(0).toString();//拿名字
				}
	
				//群聊中 接收文件的后续操作
				ReceiveFile* recvFile = new ReceiveFile(this);
				//如果点了拒绝，就直接返回即可
				connect(recvFile, &ReceiveFile::refuseFile, [this]()
				{
					return;
				});
				QString msgLabel = QString::fromLocal8Bit("收到来自") + senderName + QString::fromLocal8Bit("发来的文件，是否接受？");
				recvFile->setMsg(msgLabel);
				recvFile->show();//显示
			}
	
		}
		else//单聊
		{
			//拿到接收者的qq号
			strRecevieEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);
			strWindowID = strSendEmployeeID;
	
			//不是发给我的信息不做处理
			if (strRecevieEmployeeID != gLoginEmployeeID)
			{
				return;
			}
	
			//获取信息的类型
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
			if (cMsgType == '1')//文本信息
			{
				msgType = 1;
	
				//文本信息长度
				msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth, msgLengthWidth).toInt();
	
				//文本信息
				strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (cMsgType == '0')//表情信息
			{
				msgType = 0;
				int posImages = strData.indexOf("images");//返回images这个子字符串第一个出现的位置
				int imagesWidth = QString("images").length();
				//提取信息,截取到右边(表情名 如:077)
				strMsg = strData.mid(posImages + imagesWidth);
			}
			else if (cMsgType == '2')//文件信息-->名称、内容
			{
				msgType = 2;
				int bytesWidth = QString("bytes").length();//bytes长度
				int posBytes = strData.indexOf("bytes");//bytes的位置，返回子串第一个出现的位置
				int data_beginWidth = QString("data_begin").length();//data_begin长度
				int posData_begin = strData.indexOf("data_begin");//data_begin的位置，返回子串第一个出现的位置
	
	
				//截取文件名称 ,通过data_begin 和 bytes 之间夹着的 ，就是文件名称
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gfileName = fileName;//赋给全局变量
	
				//截取文件内容
				int dataLengthWidth;
				int posData = posData_begin + data_beginWidth;//真正文件数据的位置
				strMsg = strData.mid(posData_begin + data_beginWidth);//提取数据
				gfileData = strMsg;//赋给全局变量
	
				//根据employeeID获取发送者姓名
				QString senderName;
				int employeeID = strSendEmployeeID.toInt();
				//查发送者姓名通过查数据，id
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID =%1").arg(employeeID));
				querySenderName.exec();
				if (querySenderName.first()) //查到了
				{
					senderName = querySenderName.value(0).toString();//拿名字
				}
	
				//单聊中处理 接收文件的后续操作 
				ReceiveFile* recvFile = new ReceiveFile(this);
				//如果点了拒绝，就直接返回即可
				connect(recvFile, &ReceiveFile::refuseFile, [this]()
				{
					return;
				});
				QString msgLabel = QString::fromLocal8Bit("收到来自") + senderName + QString::fromLocal8Bit("发来的文件，是否接受？");
				recvFile->setMsg(msgLabel);
				recvFile->show();//显示
	
	
			}
	
		}
		//将受到的信息，显示到网页上,将聊天窗口置为活动窗口
		//先获取窗口地址
		QWidget* widget = WindowManager::getInstance()->findWindowName(strWindowID);
		if (widget)//聊天窗口存在
		{
			this->setCurrentWidget(widget);
	
			//同步激活左侧聊天窗口
			QListWidgetItem* item = m_talkwindowItemMap.key(widget);
			item->setSelected(true);
		}
		else//聊天窗口未打开
		{
			return;
		}
	
		//文件信息另作处理
		if (msgType != 2)
		{
			int sendEmployeeID = strSendEmployeeID.toInt();
			handleReceivedMsg(sendEmployeeID, msgType, strMsg);//将信息追加到网页上
		}
	}
	

	//// 创建一个线程池任务
	//PoolTask* pendingDataTask = new PoolTask;
	//// 将任务放入线程池中
	//QThreadPool::globalInstance()->start(pendingDataTask);
}