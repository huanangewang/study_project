#include "QtQQ_Server.h"

#include <QFileDialog> //�ļ��Ի���


const int gtcpPort = 8888; //�����˿ں�
const int gudpPort = 6666;

QtQQ_Server::QtQQ_Server(QWidget *parent)
	: QDialog(parent)
	, m_pixPath("")
{
    ui.setupUi(this);

	if (!connectMysql())//�������ݿ�
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("���ݿ�����ʧ��"));
		close();
		return;
	}

	setStatusMap();//״̬ӳ��
	setdepNameMap();//����ӳ��
	setOnlineMap();//����״̬ӳ��
	initComboBoxData();//��ʼ����Ͽ�����

	m_queryInfoModel = new QSqlQueryModel(this);
	m_queryInfoModel->setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees"));
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //�����б༭���������ֻ��
	
	//��ʼ����ѯ��˾Ⱥ����Ա����Ϣ
	m_employeeID = 0;
	m_depID = getCompDepID();
	m_compDepID = m_depID;


	updateTableData();//���±������

	//�ü�ʱ�������������ǵ�����
	m_timer = new QTimer(this);
	m_timer->setInterval(1000);//���ü��һǧ����
	m_timer->start();
	connect(m_timer, &QTimer::timeout,this,&QtQQ_Server::onRefresh);//��ʱ����

	initTcpSocket();//��ʼ��tcp�׽���
	initUdpSocket();//��ʼ��udp�׽���

}

QtQQ_Server::~QtQQ_Server()
{}

//��ʼ��tcp�׽���
void QtQQ_Server::initTcpSocket()
{
	m_tcpServer = new TcpServer(gtcpPort);
	m_tcpServer->run();

	connect(m_tcpServer, &TcpServer::signalTcpMsgComes, this, &QtQQ_Server::onUDPbroadMsg);//���յ�tcp�ͻ��˷������źź�Ҫ����udp�㲥
}

//��ʼ��udp�׽���
void QtQQ_Server::initUdpSocket()
{
	m_udpSender = new QUdpSocket(this);

}

//��ȡ��˾Ⱥqq��
int QtQQ_Server::getCompDepID()
{
	return 2000;
}

//���±������
void QtQQ_Server::updateTableData(int depID, int employeeID)
{
	ui.tableWidget->clear();
	if (depID && depID !=m_compDepID)//����id����Ч��--> �Ҹò������е�Ա�� //���ǹ�˾Ⱥ������2000��-->��ѯ����
	{
		m_queryInfoModel->setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees WHERE departmentID = %1").arg(depID));
	}
	else if (employeeID)//�ĸ�Ա��--->��ȷ����
	{
		m_queryInfoModel->setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees WHERE employeeID = %1").arg(employeeID));
	}
	else  //��˾Ⱥ����2000��-->��ѯ����
	{
		m_queryInfoModel->setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees"));
	}

	int rows = m_queryInfoModel->rowCount();//���������ܼ�¼����
	int columns = m_queryInfoModel->columnCount(); //�����������ֶ�����
	QModelIndex index; //ģ������
	
	//���ñ������������
	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);

	//���ñ�ͷ
	QStringList headers;
	headers << QStringLiteral("����")
			<< QStringLiteral("����")
			<< QStringLiteral("Ա������")
			<< QStringLiteral("Ա��ǩ��")
			<< QStringLiteral("Ա��״̬")
			<< QStringLiteral("Ա��ͷ��")
			<< QStringLiteral("����״̬");
	ui.tableWidget->setHorizontalHeaderLabels(headers);

	//�����еȿ�
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//׼��������࣬��ʼ�������
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < columns; j++)
		{
			index = m_queryInfoModel->index(i, j); //�С���
			QString strData = m_queryInfoModel->data(index).toString();//��ȡi�У�j�е�����

			//��ȡ�ֶ�����,���ó���ǰ �� �ļ�¼
			QSqlRecord record = m_queryInfoModel->record(i); //��Ӧ�� i,��ǰ�еļ�¼
			QString depRecordName = record.fieldName(j);//�У���ȡ��¼������

			if (depRecordName == QLatin1String("departmentID"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_depNameMap.value(strData)));//����ν��2001ת�ɶ�Ӧ����������
				continue;//ֱ�ӽ�����һ��ѭ��
			}
			else if (depRecordName == QLatin1String("status"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_statusMap.value(strData)));
				continue;//ֱ�ӽ�����һ��ѭ��
			}
			else if (depRecordName == QLatin1String("online"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_onlineMap.value(strData)));
				continue;//ֱ�ӽ�����һ��ѭ��
			}

			//��ͨ�����
			ui.tableWidget->setItem(i, j, new QTableWidgetItem(strData));   //new QTableWidgetItem(strData)�� �򵼣�Ҫnew����

		}
	}

}
//״̬ӳ��
void QtQQ_Server::setStatusMap()
{
	m_onlineMap.insert(QStringLiteral("1"), QStringLiteral("����"));
	m_onlineMap.insert(QStringLiteral("2"), QStringLiteral("����"));
	m_onlineMap.insert(QStringLiteral("3"), QStringLiteral("æµ"));//��ʱδ���

}

//����ӳ��
void QtQQ_Server::setdepNameMap()
{
	m_depNameMap.insert(QStringLiteral("2001"), QStringLiteral("���²�"));
	m_depNameMap.insert(QStringLiteral("2002"), QStringLiteral("�з���"));
	m_depNameMap.insert(QStringLiteral("2003"), QStringLiteral("�г���"));

}

//����״̬ӳ��
void QtQQ_Server::setOnlineMap()
{
	m_statusMap.insert(QStringLiteral("1"), QStringLiteral("��Ч"));
	m_statusMap.insert(QStringLiteral("0"), QStringLiteral("��ע��"));

}

//��ʼ����Ͽ������(��������Ͽ�)
void QtQQ_Server::initComboBoxData()
{
	QString itemText;//��Ͽ�����ı�
	//��ȡ��˾�ܵĲ��Ÿ���
	QSqlQueryModel queryDepModel;
	queryDepModel.setQuery(QString::fromLocal8Bit("SELECT * FROM tab_department"));
	int depCounts = queryDepModel.rowCount() - 1; //-1��Ϊ�˼�ȥ��˾Ⱥ

	//�����Ǹ���Ͽ�
	for (int i = 0; i < depCounts;i++)
	{
		itemText = ui.employeeDepBox->itemText(i); //����0��ʼ, �õ� '���²�' '�з���'�ȵ�....
		QSqlQuery queryDepID(QString::fromLocal8Bit("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(itemText));//�ò���id
		queryDepID.exec();//ִ��
		queryDepID.first();//ָ���һ��

		//��Ա������������Ͽ���������Ϊ��Ӧ�Ĳ���id��
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());//0����Ϊֻ�е�һ�У�������
	}

	//�����Ǹ���Ͽ�
	for (int i = 0;i < depCounts+1;i++)//+1����Ϊ�ҿ���ͨ����˾Ⱥɸѡ
	{
		itemText = ui.departentBox->itemText(i);
		QSqlQuery queryDepID(QString::fromLocal8Bit("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(itemText));//�ò���id
		queryDepID.exec();//ִ��
		queryDepID.first();//ָ���һ��

	    //���ò�����Ͽ�����Ϊ��Ӧ��id��
		ui.departentBox->setItemData(i, queryDepID.value(0).toInt());//0����Ϊֻ�е�һ�У�������

	}

}

//ˢ�±������-->ͨ����ʱ��
void QtQQ_Server::onRefresh()
{
	updateTableData(m_depID, m_employeeID);
}

//��ѯ���Ű�ť���(����Ⱥqq��)
void QtQQ_Server::on_queryDepartmentBtn_clicked()
{
	m_employeeID = 0;//Ϊ�˰�ȫ
	ui.queryIdLineEdit->clear();//Ϊ�˺ÿ�������������������

	m_depID = ui.departentBox->currentData().toInt();//��Ϊ�е����ĺ���ʵ���ˣ�����ؼ��ĳ�ʼ������������ֱ�ӻ�ȡ�����id����
	updateTableData(m_depID);//ֻ���벿��id
}

//��ѯԱ����ť���(����Ա��qq�Ų�)
void QtQQ_Server::on_queryIdBtn_clicked()
{
	m_depID = m_compDepID;

	//���Ա��qq���Ƿ�����
	if (!ui.queryIdLineEdit->text().length())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����qq��Ϊ��"));
		ui.queryIdLineEdit->setFocus();//���ý���
		return;
	}

	int employeeID = ui.queryIdLineEdit->text().toInt();//��ȡ�û������Ա����
	QSqlQuery queryID(QString("SELECT * FROM tab_employees WHERE employeeID = %1").arg(employeeID));
	queryID.exec();
	//queryID.first();

	if (!queryID.first())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��������ȷ��Ա����!"));
		ui.queryIdLineEdit->setFocus();//���ý���
		return;
	}
	else //�鵽��
	{
		m_employeeID = employeeID; //��Ϊ�ڶ�ʱ����ʱˢ���ˣ���������ֻ��Ҫ��������ֵ����
	}
}

//ע����ť-->�����ݿ�������ݽ��иı�״̬����
void QtQQ_Server::on_logoutBtn_clicked()
{
	ui.queryIdLineEdit->clear();
	//���Ա��qq���Ƿ�����
	if (!ui.logoutIdLineEdit->text().length())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����qq��Ϊ��"));
		ui.logoutIdLineEdit->setFocus();//���ý���
		return;
	}

	int employeeID = ui.logoutIdLineEdit->text().toInt();//��ȡ�û������Ա����
	QSqlQuery queryID(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1").arg(employeeID));
	queryID.exec();

	if (!queryID.first()) //�鲻������ʾ
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��������ȷ��Ա����!"));
		ui.logoutIdLineEdit->setFocus();//���ý���
		return;
	}
	else //�鵽�ˣ�ע��-->��Ա��status ���ó�0
	{
		//����status���ڱ�tab_employees�н��и���
		QSqlQuery sqlUpdate(QString("UPDATE tab_employees SET status = 0 WHERE employeeID = %1").arg(employeeID));
		sqlUpdate.exec();//ִ��

		////��ȡע��Ա��������-->ͨ��id�Ų����ݿ�
		//QSqlQuery sqlQueryName(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1 ").arg(employeeID));
		QString strName = queryID.value(0).toString();

		//fromLocal8Bit VS����Ҫ�ӱ��ر���ת��uinco
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), 
			QString::fromLocal8Bit("Ա����%1 ����ҵqq��%2 �ѱ�ע��!")
			.arg(strName)
			.arg(employeeID));
		
		ui.logoutIdLineEdit->clear();
	}

}

//ѡ��ͼƬ��ť(Ա��ͷ��)//1�����з������ʾͼƬ�Ĺ��ܣ������޸������m_pixPath
void QtQQ_Server::on_selectPictureBtn_clicked()
{
	//ͼƬ·�����м�¼���档
	m_pixPath = QFileDialog::getOpenFileName(
		this,
		QString::fromLocal8Bit("ѡ��ͷ��"),
		".",//��ǰĿ¼
		"*.png;;*.jpg" //��ʽ
	);

	if (!m_pixPath.size())
	{
		//·��û��
		return;
	}

	//��ͷ����ʾ����ǩ
	QPixmap pixmap;
	pixmap.load(m_pixPath);
	//Ϊ��ƥ���С -->���ﲻ̫�� -->���������:	widthRatio   heightRatio
	qreal widthRatio = (qreal)ui.headLabel->width() / (qreal)pixmap.width();
	qreal heightRatio = (qreal)ui.headLabel->height() / (qreal)pixmap.height();

	QSize size(pixmap.width() * widthRatio, pixmap.height()* heightRatio);//�����С�ߴ�
	ui.headLabel->setPixmap(pixmap.scaled(size));//��������,����ͼƬ·������
}

//���Ա����ť //2���������ݹ��� 
//--->��ʵ������Ҫ���뵽��tab_employees�У���Ҫ���뵽tab_accounts�У�Ҫ��Ȼ��¼���˿ͻ���
void QtQQ_Server::on_addBtn_clicked()
{
	//���Ա����������
	QString strName = ui.nameLineEdit->text();
	if (!strName.size())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("������Ա��������"));
		ui.nameLineEdit->setFocus();
		return;
	}

	//���Ա��ͷ���Ƿ����
	if (!m_pixPath.size())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��ѡ��Ա��ͷ��·����"));
		return;
	}

	//��ʼ��������
	//��ȡԱ��QQ��-->���ĺ�
	QSqlQuery queryMaxEmployeeID(QString("SELECT MAX(employeeID) FROM tab_employees"));
	queryMaxEmployeeID.exec();
	queryMaxEmployeeID.first();

	int employeeID = queryMaxEmployeeID.value(0).toInt() + 1;//��1
	int depID = ui.employeeDepBox->currentData().toInt(); //����id

	//��ͼƬ·������Ϊxxx\xxx.png�ĸ�ʽ��ͳһ��ʽ
	m_pixPath.replace("/","\\\\");//����Ҫת��,��4��б�ܴ�����һ��'\'

	QSqlQuery insertSql(QString("INSERT INTO tab_employees(departmentID,employeeID,employee_name,picture) VALUES(%1,%2,'%3','%4')")
					.arg(depID) //����id
					.arg(employeeID) //Ա��id
					.arg(strName) //Ա������
					.arg(m_pixPath)); //ͷ��·��

	//ҲҪ��ӵ���tab_accounts�У����ڵ�¼
	QSqlQuery tab_accounts_Insert(QString("INSERT INTO tab_accounts(employeeID,account,code) VALUES(%1,'%2','%3')")
					.arg(employeeID) //Ա��id
					.arg(strName)//Ա������(��Ӣ��)
					.arg(strName)); //�����������ͬ

	insertSql.exec();//ִ��
	tab_accounts_Insert.exec();

	QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����Ա���ɹ���"));
	
	m_pixPath = "";//���
	ui.headLabel->setText(QStringLiteral("Ա������"));//���ͷ��


}

//���յ��ͻ�����Ϣ��Ҫ�㲥�Ĳۺ���
void QtQQ_Server::onUDPbroadMsg(QByteArray& btData)
{
	//Ϊ����ÿ���ͻ��˵Ķ˿ڲ�һ����ǰ�ڲ���200����ֻ����һ̨�����ϣ�û�취
	for (quint16 port = gudpPort; port < gudpPort + 200;  ++port)
	{
		m_udpSender->writeDatagram(btData, btData.size(), QHostAddress::Broadcast, port);//�㲥�ķ�ʽ,д����
	}
}

//�������ݿ�
bool QtQQ_Server::connectMysql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");//���ݿ���
	db.setHostName("localhost");//������
	db.setUserName("root");//�û���
	db.setPassword("root");//����
	db.setPort(3306);//�˿�

	if (db.open())
	{
		return true; //��ȷ��
	}
	else
	{
		return false;
	}
}