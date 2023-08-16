#include "TalkWindow.h"
#include "RootContactItem.h"
#include "ContactItem.h"
#include "CommonUtils.h"
#include "WindowManager.h"
#include <QToolTip>
#include <QFile>
#include <QMessageBox>

TalkWindow::TalkWindow(QWidget *parent, const QString& uid/*, GroupType groupType8*/)
	: QWidget(parent)
	,m_talkId(uid)
	//,m_groupType(groupType) //��ʼ���б�
{
	ui.setupUi(this);
	//�����ʱ��Ҫ��WindowManager���addWindowName,���ӳ�������
	//������Դ򿪶����ͬ���ڵ�����
	WindowManager::getInstance()->addWindowName(m_talkId, this);
	initControl();
	setAttribute(Qt::WA_DeleteOnClose);//���ô�������
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

//���Ա����Ϣ
void TalkWindow::addPeopInfo(QTreeWidgetItem * pRootGroupItem)
{
	//��ϵ�����ͷ�񡣡���
	QTreeWidgetItem* pChild = new QTreeWidgetItem();

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");//�ȼ��ؿյ���Դ
	const QPixmap image(":/Resources/MainWindow/xmb.png");

	//����ӽڵ�
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole+1, QString::number((int)pChild));
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);//��ϵ�����ͷ��ǩ��ʲô��


	//����
	static int i = 0;
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(image, pix1, pContactItem->getHeadLabelSize()));//����Բͷ��
	pContactItem->setUserName(QString::fromLocal8Bit("���ϸ���С��%1").arg(i++));
	pContactItem->setSignName(QString::fromLocal8Bit("л������˧"));

	pRootGroupItem->addChild(pChild);//����ǰ������ӵ�������
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);//��ӵ�����,������Ŀ����(pContactItem)

	QString strName = pContactItem->getUserName();
	m_groupPerpleMap.insert(pChild, strName);//�´Σ�������Ⱥ���� ˫����ĳ���˵�ʱ�򣬾Ϳ����õ�����˵����֣����������µĴ��ڱ���



}


//���촰���е�˫���Ҳദ��-->������
void TalkWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column)
{
	//�ж�����͸���
	bool isChild = item->data(0, Qt::UserRole).toBool();
	if (isChild)
	{
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
