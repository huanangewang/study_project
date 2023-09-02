#include "PoolTask.h"
#include"ReceiveFile.h"
#include "WindowManager.h"

#include <QSqlQuery>


extern QString gLoginEmployeeID;
extern QString gfileName; //文件内容
extern QString gfileData;//文件内容

PoolTask::PoolTask(TalkWindowShell *parent)
	:QRunnable()
{
	setAutoDelete(false);
}

PoolTask::~PoolTask()
{}


void PoolTask::run()
{
		//处理服务器udp广播收到的数据(解析)
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
	
}
