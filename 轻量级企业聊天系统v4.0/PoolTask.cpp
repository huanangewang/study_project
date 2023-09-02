#include "PoolTask.h"
#include"ReceiveFile.h"
#include "WindowManager.h"

#include <QSqlQuery>


extern QString gLoginEmployeeID;
extern QString gfileName; //�ļ�����
extern QString gfileData;//�ļ�����

PoolTask::PoolTask(TalkWindowShell *parent)
	:QRunnable()
{
	setAutoDelete(false);
}

PoolTask::~PoolTask()
{}


void PoolTask::run()
{
		//���������udp�㲥�յ�������(����)
		while (m_udpReceiver->hasPendingDatagrams())//udp�˿����Ƿ���δ��������� bool
		{
			const static int groupFlagWidth = 1;	//Ⱥ�ı�־ռλ�����ԶΪ1
			const static int groupWidth = 4;		//Ⱥqq�ſ��
			const static int employeeWidth = 5;		//Ա��qq�ſ��
			const static int msgTypeWidth = 1;		//��Ϣ���Ϳ��
			const static int msgLengthWidth = 5;	//�ı���Ϣ���ȵĿ��
			const static int pictureWidth = 3;		//����ͼƬ���

			//�ó�����
			QByteArray btData;
			btData.resize(m_udpReceiver->pendingDatagramSize());//δ�������ݵĴ�С(����Ҫ��������ݴ�С)
			m_udpReceiver->readDatagram(btData.data(), btData.size());//��ȡudp����(���ݡ���С)

			QString strData = btData.data();//�õ�ȫ������
			QString strWindowID;//���촰��ID��Ⱥ�ľ���Ⱥ�ţ����ľ���Ա��id��
			QString strSendEmployeeID, strRecevieEmployeeID;//���ͼ����ն˵�qq��
			QString strMsg;//����

			int msgLen;//���ݳ���
			int msgType;//��������

			strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);//��ȡ,��Ⱥ�ı�־��ʼ����ȡid�ų���

			//�������˷�����ģ������Լ���������Ϣ��Ҳ���Ա��������㲥���Լ����������������
			if (strSendEmployeeID == gLoginEmployeeID)
			{
				return;
			}

			if (btData[0] == '1') //Ⱥ��
			{
				//�õ�Ⱥ�Ĵ�������id
				strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth); //ͨ������Ķ���Ŀ�Ⱥͽ�����ݰ���ʽ���н�ȡ

				//�ж���Ϣ����,��Ϊ��QChar���ͣ�������'1'������
				QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];
				if (cMsgType == '1')//�ı���Ϣ
				{
					msgType = 1;//��Ϣ����
					msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();//��ȡ��ռλΪ5����Ϣ�����Ƕ���

					//��ȡ��Ϣ--��ͨ����ȡ���õ�����Ϣ����
					strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth + msgLengthWidth, msgLen);
				}
				else if (cMsgType == '0')//������Ϣ
				{
					msgType = 0;
					int posImages = strData.indexOf("images");//����������ַ�����һ�����ֵ�λ��
					//int imagesWidth = QString("images").length();

					//��ȡ��Ϣ,��ȡ���ұ�(������ ��:077)
					strMsg = strData.right(strData.length() - posImages - QString("images").length());

				}
				else if (cMsgType == '2')//�ļ���Ϣ
				{
					msgType = 2;
					int byteWidth = QString("bytes").length();//bytes����
					int posBytes = strData.indexOf("bytes");//bytes��λ�ã������Ӵ���һ�����ֵ�λ��
					int posData_begin = strData.indexOf("data_begin"); //data_begin��λ�ã������Ӵ���һ�����ֵ�λ��
					int data_beginWidth = QString("data_begin").length();

					//��ȡ�ļ����� ,ͨ��data_begin �� bytes ֮����ŵ� �������ļ�����
					QString fileName = strData.mid(posBytes + byteWidth, posData_begin - posBytes - byteWidth);
					gfileName = fileName;//����ȫ�ֱ���

					//��ȡ�ļ�����
					int dataLengthWidth;
					int posData = posData_begin + data_beginWidth;//�����ļ����ݵ�λ��
					strMsg = strData.mid(posData);//��ȡ����
					gfileData = strMsg;//����ȫ�ֱ���

					//����employeeID��ȡ����������
					QString senderName;
					int employeeID = strSendEmployeeID.toInt();
					//�鷢��������ͨ�������ݣ�id
					QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID =%1").arg(employeeID));
					querySenderName.exec();
					if (querySenderName.first()) //�鵽��
					{
						senderName = querySenderName.value(0).toString();//������
					}

					//Ⱥ���� �����ļ��ĺ�������
					ReceiveFile* recvFile = new ReceiveFile(this);
					//������˾ܾ�����ֱ�ӷ��ؼ���
					connect(recvFile, &ReceiveFile::refuseFile, [this]()
					{
						return;
					});
					QString msgLabel = QString::fromLocal8Bit("�յ�����") + senderName + QString::fromLocal8Bit("�������ļ����Ƿ���ܣ�");
					recvFile->setMsg(msgLabel);
					recvFile->show();//��ʾ
				}

			}
			else//����
			{
				//�õ������ߵ�qq��
				strRecevieEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);
				strWindowID = strSendEmployeeID;

				//���Ƿ����ҵ���Ϣ��������
				if (strRecevieEmployeeID != gLoginEmployeeID)
				{
					return;
				}

				//��ȡ��Ϣ������
				QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
				if (cMsgType == '1')//�ı���Ϣ
				{
					msgType = 1;

					//�ı���Ϣ����
					msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
						+ msgTypeWidth, msgLengthWidth).toInt();

					//�ı���Ϣ
					strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
						+ msgTypeWidth + msgLengthWidth, msgLen);
				}
				else if (cMsgType == '0')//������Ϣ
				{
					msgType = 0;
					int posImages = strData.indexOf("images");//����images������ַ�����һ�����ֵ�λ��
					int imagesWidth = QString("images").length();
					//��ȡ��Ϣ,��ȡ���ұ�(������ ��:077)
					strMsg = strData.mid(posImages + imagesWidth);
				}
				else if (cMsgType == '2')//�ļ���Ϣ-->���ơ�����
				{
					msgType = 2;
					int bytesWidth = QString("bytes").length();//bytes����
					int posBytes = strData.indexOf("bytes");//bytes��λ�ã������Ӵ���һ�����ֵ�λ��
					int data_beginWidth = QString("data_begin").length();//data_begin����
					int posData_begin = strData.indexOf("data_begin");//data_begin��λ�ã������Ӵ���һ�����ֵ�λ��


					//��ȡ�ļ����� ,ͨ��data_begin �� bytes ֮����ŵ� �������ļ�����
					QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
					gfileName = fileName;//����ȫ�ֱ���

					//��ȡ�ļ�����
					int dataLengthWidth;
					int posData = posData_begin + data_beginWidth;//�����ļ����ݵ�λ��
					strMsg = strData.mid(posData_begin + data_beginWidth);//��ȡ����
					gfileData = strMsg;//����ȫ�ֱ���

					//����employeeID��ȡ����������
					QString senderName;
					int employeeID = strSendEmployeeID.toInt();
					//�鷢��������ͨ�������ݣ�id
					QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID =%1").arg(employeeID));
					querySenderName.exec();
					if (querySenderName.first()) //�鵽��
					{
						senderName = querySenderName.value(0).toString();//������
					}

					//�����д��� �����ļ��ĺ������� 
					ReceiveFile* recvFile = new ReceiveFile(this);
					//������˾ܾ�����ֱ�ӷ��ؼ���
					connect(recvFile, &ReceiveFile::refuseFile, [this]()
					{
						return;
					});
					QString msgLabel = QString::fromLocal8Bit("�յ�����") + senderName + QString::fromLocal8Bit("�������ļ����Ƿ���ܣ�");
					recvFile->setMsg(msgLabel);
					recvFile->show();//��ʾ


				}

			}
			//���ܵ�����Ϣ����ʾ����ҳ��,�����촰����Ϊ�����
			//�Ȼ�ȡ���ڵ�ַ
			QWidget* widget = WindowManager::getInstance()->findWindowName(strWindowID);
			if (widget)//���촰�ڴ���
			{
				this->setCurrentWidget(widget);

				//ͬ������������촰��
				QListWidgetItem* item = m_talkwindowItemMap.key(widget);
				item->setSelected(true);
			}
			else//���촰��δ��
			{
				return;
			}

			//�ļ���Ϣ��������
			if (msgType != 2)
			{
				int sendEmployeeID = strSendEmployeeID.toInt();
				handleReceivedMsg(sendEmployeeID, msgType, strMsg);//����Ϣ׷�ӵ���ҳ��
			}
		}
	
}
