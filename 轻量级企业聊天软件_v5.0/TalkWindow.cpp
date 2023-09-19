#include "TalkWindow.h"
#include "RootContactItem.h"
#include "ContactItem.h"
#include "CommonUtils.h"
#include "WindowManager.h"
#include "SendFile.h"


#include <QToolTip>
#include <QFile>
#include <QMessageBox>
#include<QSqlQueryModel>
#include <QSqlQuery>
#include<QModelIndex>

extern QString gLoginEmployeeID;

TalkWindow::TalkWindow(QWidget *parent, const QString& uid/*, GroupType groupType8*/)
	: QWidget(parent)
	,m_talkId(uid)
	//,m_groupType(groupType) //��ʼ���б�
{
	ui.setupUi(this);
	//�����ʱ��Ҫ��WindowManager���addWindowName,���ӳ�������
	//������Դ򿪶����ͬ���ڵ�����
	WindowManager::getInstance()->addWindowName(m_talkId, this);
	setAttribute(Qt::WA_DeleteOnClose);//���ô�������

	initGroupTalkStatus();//��ʼ��Ⱥ��״̬
	initControl();
}

TalkWindow::~TalkWindow()
{
	//���մ�����Դ
	WindowManager::getInstance()->deleteWindowName(m_talkId);
}

//��ӱ���ͼƬ
void TalkWindow::addEmotionImage(int emotionNum)
{
	ui.textEdit->setFocus();//���ý���
	ui.textEdit->addEmotionUrl(emotionNum);
}

//���ô�������
void TalkWindow::setWindowName(const QString & name)
{
	ui.nameLabel->setText(name);
}

//Ϊ�˷���˽������ m_talkid
QString TalkWindow::getTalkId()
{
	return m_talkId;
}

//�ļ��򿪰�ť
void  TalkWindow::onFileOpenBtnClicked(bool)
{
	SendFile* sendFile = new SendFile(this);
	sendFile->show();//ui����
}

//������Ϣ
void TalkWindow::onSendBtnClicked(bool)
{
	//������Ϣ����Ϊ��,Ҫת�ɴ��ı�
	if (ui.textEdit->toPlainText().isEmpty())
	{
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)), QString::fromLocal8Bit("������Ϣ����Ϊ��~~"),
			this, QRect(0, 0, 120, 100), 2000);//2000����ʾ��ʱ�䳤��2s �� 
		return;
	}
	  QString html = ui.textEdit->document()->toHtml();//���ı�ת��html���ַ������ͣ�,����ɾ�������ú�const

	//Ŀ���ǽ��������������������ʧЧ��bug
	//�ļ�html���û�����壬Ҳ���ǼȲ��Ǳ�����Ϣ�����Ҳ������ı�����������壬msgFont.txt
	if (!html.contains(".png") && !html.contains("</span>"))
	{
		//û���ֺ�
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");

		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			//�����滻
			fontHtml.replace("%1", text);
			file.close();
		}
		else //���ļ�ʧ��
		{
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ")
				, QString::fromLocal8Bit("δ�ҵ�msgFont.txt"));
			return;
		}

		if (!html.contains(fontHtml)) //û�а����Ļ�,�滻�ɰ��������xxx
		{
			html.replace(text, fontHtml);
		}
	}


	ui.msgWidget->appendMsg(html);//׷����Ϣ

	ui.textEdit->clear();
	ui.textEdit->deletAllEmotionImage();

}

//��ʼ���ؼ�
void TalkWindow::initControl()
{
	//�ұ߷��������óߴ�,�������ã����Ǵ�ֱ����ˮƽ
	QList <int> rightWidgetSize;
	rightWidgetSize << 600 << 138;
	ui.bodySplitter->setSizes(rightWidgetSize);

	ui.textEdit->setFontPointSize(10);//��������
	ui.textEdit->setFocus();

	connect(ui.sysmin,SIGNAL(clicked(bool)),parent(),SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));

	connect(ui.faceBtn, SIGNAL(clicked(bool)), parent(), SLOT(onEmotionBtnClicked(bool)));//���鰴ť
	connect(ui.sendBtn, SIGNAL(clicked(bool)),this, SLOT(onSendBtnClicked(bool)));//���Ͱ�ť

	//����˫��
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int))
		, this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

	connect(ui.fileopenBtn, SIGNAL(clicked(bool)), this, SLOT(onFileOpenBtnClicked(bool)));//�ļ��򿪰�ť

	//�ж��Ƿ���Ⱥ��
	if (m_isGroupTalk)
	{
		initTalkWindow();//��ʼ��Ⱥ��
	}
	else
	{
		initPtoPTalk();//��ʼ������
	}

	/*
	//�������촰�ڹ���ʱ���û�ָ����Ⱥ��һ�������в�ͬ�Ĺ���
	switch (m_groupType)
	{
	case COMPANY:
	{
		initCompanyTalk();//��ʼ����˾Ⱥ����
		break;
	}
	case PERSONELGROUP:
	{
		initPersonTalk();//��ʼ�����²�����
		break;
	}
	case MARKETGROUP:
	{
		initMarketTalk();//��ʼ���г�������
		break;
	}
	case DEVELOPMENTGROUP:
	{
		initDevelopTalk();//��ʼ���з�������
		break;
	}
	case PTOP://����
	{
		initPtoPTalk();//��ʼ����������
		break;
	}
	default:
		break;
	}
	*/
}
//��ʼ��Ⱥ��״̬
void TalkWindow::initGroupTalkStatus()
{
	//ͨ��������Ա��id�ţ�Ҳ����uid(m_talkid),�����ݿ����ѯ,����tab_department����в�ѯ����õ�����Ⱥ�ģ�������
	QSqlQueryModel sqlDepModel;
	QString strSql = QString("SELECT * FROM tab_department WHERE departmentID = %1").arg(m_talkId);//������û��departmentID��m_talkId
	sqlDepModel.setQuery(strSql);//����ģ�͵�sql���

	int rows = sqlDepModel.rowCount();
	if (rows == 0)
	{
		//����
		m_isGroupTalk = false;
	}
	//Ⱥ��
	else
	{
		m_isGroupTalk = true;
	}
}


//��ȡ��˾��id��-->�Թ�˾Ⱥ���� ���⴦��
int TalkWindow::getComDepID()
{
	//ͨ����˾Ⱥ����ѯ��˾Ⱥ��id
	QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
		.arg(QString::fromLocal8Bit("��˾Ⱥ")));
	queryDepID.exec();
	queryDepID.first();//ָ��������һ��

	return queryDepID.value(0).toInt();

}

//���Ա����Ϣ
void TalkWindow::addPeopInfo(QTreeWidgetItem * pRootGroupItem, int employeeID)
{
	//��ϵ�����ͷ�񡣡���
	QTreeWidgetItem* pChild = new QTreeWidgetItem();

	//����ӽڵ� --��һֱ��̫������ΪʲôҪ����д
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID); //Ŀ��Ϊ���޸����������ʾ��ͷ���bug
	//pChild->setData(0, Qt::UserRole + 1, QString::number((int)pChild));
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);//��ϵ�����ͷ��ǩ��ʲô��

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");//�ȼ��ؿյ���Դ
	//const QPixmap image(":/Resources/MainWindow/xmb.png");

	//��ȡ�û�����ǩ����ͷ��
	QString strName, strSign, strPicturePath;
	QSqlQueryModel queryInfoModel;//��ѯģ��
	queryInfoModel.setQuery(QString("SELECT employee_name,employee_sign,picture FROM tab_employees WHERE employeeID = %1")
		.arg(employeeID));//�������Ҫ�ܲ�ѯ�� �û�����ǩ����ͷ��
	QModelIndex nameIndex, signIndex, pictureIndex; //���������
	nameIndex = queryInfoModel.index(0, 0); //�У���
	signIndex = queryInfoModel.index(0, 1);
	pictureIndex = queryInfoModel.index(0, 2);

	//������Ҫ��ֵ
	strName =		 queryInfoModel.data(nameIndex).toString();
	strSign =		 queryInfoModel.data(signIndex).toString();
	strPicturePath = queryInfoModel.data(pictureIndex).toString();

	////����
	//static int i = 0;
	//pContactItem->setHeadPixmap(CommonUtils::getRoundImage(image, pix1, pContactItem->getHeadLabelSize()));//����Բͷ��
	//pContactItem->setUserName(QString::fromLocal8Bit("���ϸ���С��%1").arg(i++));
	//pContactItem->setSignName(QString::fromLocal8Bit("л������˧"));

	QImage imageHead;
	imageHead.load(strPicturePath);//�������ݿ��е�ͷ��·��

	//��������
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(QPixmap::fromImage(imageHead), pix1, pContactItem->getHeadLabelSize()));//����Բͷ��
	pContactItem->setUserName(strName);
	pContactItem->setSignName(strSign);

	pRootGroupItem->addChild(pChild);//����ǰ������ӵ�������
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);//��ӵ�����,������Ŀ����(pContactItem)

	QString str = pContactItem->getUserName();
	m_groupPerpleMap.insert(pChild, str);//�´Σ�������Ⱥ���� ˫����ĳ���˵�ʱ�򣬾Ϳ����õ�����˵����֣����������µĴ��ڱ���



}


//���촰���е�˫���Ҳദ��-->������
void TalkWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column)
{
	//�ж�����͸���
	bool isChild = item->data(0, Qt::UserRole).toBool();
	if (isChild)
	{
		QString talkId = item->data(0, Qt::UserRole + 1).toString();
		if (talkId == gLoginEmployeeID)
		{
			return;
		}
		QString strPeopleName = m_groupPerpleMap.value(item);//���value������key
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString()/*, PTOP, strPeopleName*/);//��������Ӧ����UserRole+1
	}

}

/*
//��ʼ����˾Ⱥ����
void TalkWindow::initCompanyTalk()
{
	//��ΪȺ�кܶ��Ա���ȹ���һ����
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//������û�У�ָʾ����������չ��������

	pRootItem->setData(0, Qt::UserRole, 0);//������������0,����data,�������ָ������
	RootContactItem* pItemName = new RootContactItem(false,ui.treeWidget);

	ui.treeWidget->setFixedHeight(646); //shell�ܸ�-shellͷ�ߣ�talkwindow �� titlewidget��

	int nEmployeeNum = 50;//�����ܹ�50��
	QString qsGroupName = QString::fromLocal8Bit("��˾Ⱥ %1/%2").arg(0).arg(nEmployeeNum); //����Ĳ�������
	pItemName->setText(qsGroupName);//���ø��ı�

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//Ĭ��չ��
	pRootItem->setExpanded(true);

	for (int i = 0;i < nEmployeeNum;i++)
	{
		addPeopInfo(pRootItem);//����ǰ��pRootItem���
	}

}

//��ʼ�����²�����
void TalkWindow::initPersonTalk()
{
	//��ΪȺ�кܶ��Ա���ȹ���һ����
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//������û�У�ָʾ����������չ��������

	pRootItem->setData(0, Qt::UserRole, 0);//������������0,����data,�������ָ������
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646); //shell�ܸ�-shellͷ�ߣ�talkwindow �� titlewidget��

	int nEmployeeNum = 5;//�����ܹ�5��
	QString qsGroupName = QString::fromLocal8Bit("���²� %1/%2").arg(0).arg(nEmployeeNum); //����Ĳ�������
	pItemName->setText(qsGroupName);//���ø��ı�

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//Ĭ��չ��
	pRootItem->setExpanded(true);

	for (int i = 0;i < nEmployeeNum;i++)
	{
		addPeopInfo(pRootItem);//����ǰ��pRootItem���
	}
}

//��ʼ���г�������
void TalkWindow::initMarketTalk()
{
	//��ΪȺ�кܶ��Ա���ȹ���һ����
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//������û�У�ָʾ����������չ��������

	pRootItem->setData(0, Qt::UserRole, 0);//������������0,����data,�������ָ������
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646); //shell�ܸ�-shellͷ�ߣ�talkwindow �� titlewidget��

	int nEmployeeNum = 8;//�����ܹ�50��
	QString qsGroupName = QString::fromLocal8Bit("�г��� %1/%2").arg(0).arg(nEmployeeNum); //����Ĳ�������
	pItemName->setText(qsGroupName);//���ø��ı�

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//Ĭ��չ��
	pRootItem->setExpanded(true);

	for (int i = 0;i < nEmployeeNum;i++)
	{
		addPeopInfo(pRootItem);//����ǰ��pRootItem���
	}
}

//��ʼ���з�������
void TalkWindow::initDevelopTalk()
{
	//��ΪȺ�кܶ��Ա���ȹ���һ����
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//������û�У�ָʾ����������չ��������

	pRootItem->setData(0, Qt::UserRole, 0);//������������0,����data,�������ָ������
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646); //shell�ܸ�-shellͷ�ߣ�talkwindow �� titlewidget��

	int nEmployeeNum = 32;//�����ܹ�50��
	QString qsGroupName = QString::fromLocal8Bit("�з��� %1/%2").arg(0).arg(nEmployeeNum); //����Ĳ�������
	pItemName->setText(qsGroupName);//���ø��ı�

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//Ĭ��չ��
	pRootItem->setExpanded(true);

	for (int i = 0;i < nEmployeeNum;i++)
	{
		addPeopInfo(pRootItem);//����ǰ��pRootItem���
	}
}

*/


//��ʼ����������
void TalkWindow::initPtoPTalk()
{
	//���ұ߷�һ��ͼƬ
	QPixmap pixSkin;
	pixSkin.load(":/Resources/MainWindow/skin.png");

	ui.widget->setFixedSize(pixSkin.size());//���óߴ�

	//ͼƬһ��ű�ǩ��
	QLabel* skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());//���ñ�ǩ��С

}

//��ʼ��Ⱥ��
void TalkWindow::initTalkWindow()
{
	//1.�������
	//��ΪȺ�кܶ��Ա���ȹ���һ����
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//������û�У�ָʾ����������չ��������

	pRootItem->setData(0, Qt::UserRole, 0);//������������0,����data,�������ָ������
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646); //shell�ܸ�-shellͷ�ߣ�talkwindow �� titlewidget��

	//�õ���ǰ����Ⱥ����
	QString strGroupName;                                     // SQL���֪ʶ���䣺where ��ֵ���ҵķ���,����ͨ��id��
	QSqlQuery queryGroupName(QString("SELECT department_name FROM tab_department WHERE departmentID = %1").arg(m_talkId));//Ϊ�˲��Ҳ�������
	queryGroupName.exec();//����
	if (queryGroupName.first())
	{
		strGroupName = queryGroupName.value(0).toString();
	}

	//����Ҫ��ȡԱ������
	//������-->����ǹ�˾Ⱥ��Ҫ�����⴦����Ϊû��ͨ��2000(��˾Ⱥid)���ж��м��ˣ��Ͼ�û�˵Ĳ���id��2000 ,ֻ��2001-2003
	QSqlQueryModel queryEmployeeModel;
	if (getComDepID() == m_talkId.toInt())//�����������˾Ⱥ
	{
		queryEmployeeModel.setQuery("SELECT employeeID FROM tab_employees WHERE status = 1");//��������״̬Ϊ1��Ա��
	}
	else
	{
		//����������һ������״̬Ϊ1��Ա����һ�����Ҷ�Ӧ����id��Ա��
		queryEmployeeModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status = 1 AND departmentID =%1")
			.arg(m_talkId));
	}
	//������ֻ��Ҫ������ѯ����м��м���
	int nEmployeeNum = queryEmployeeModel.rowCount();
	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3")
		.arg(strGroupName)
		.arg(0)
		.arg(nEmployeeNum); //1��Ⱥ�� 2�������������� 3��������


	pItemName->setText(qsGroupName);//���ø��ı�
	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//Ĭ��չ��
	pRootItem->setExpanded(true);

	//��ӳ�Ա��Ϣ(Ҫͨ��Ա��id)�����ڲ���ֻ����һ�����������
	//2023-08-29 BUG�޸ģ�����ʼ���Լ��Ĵ��ڣ����ܺ��Լ�����
	for (int i = 0;i < nEmployeeNum;i++)
	{
		QModelIndex modelIndex = queryEmployeeModel.index(i, 0); //�У�i  �У�0  -->��ǰ�����ڵ�ģ������
		int employeeID = queryEmployeeModel.data(modelIndex).toInt();//�õ�Ա��id��
		addPeopInfo(pRootItem,employeeID);//����ǰ��pRootItem���,����ӽڵ�
	}
}
