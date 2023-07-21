#include"manager.h"
#include<fstream>

//Ĭ�Ϲ���
Manager::Manager()
{

}

//�вι���
Manager::Manager(string name, string pwd)
{
	//��ʼ������Ա��Ϣ
	this->m_Name = name;
	this->m_Pwd = pwd;

	//��ʼ��ѧ������ ��ȡ�������ļ��� ѧ��
	this->initVector();

	//��ʼ���鱾���� ��ȡ�������ļ��� �鱾
	this->initVector1();
	
}

//�Ӳ˵�����
void Manager::operMenu()
{
	//�û��������
	cout << "��ӭ����Ա��" << this->m_Name << "��¼��" << endl;
	cout << "\t\t -------------------------------------\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          1.����û��˺�            |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          2.ɾ���û��˺�            |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          3.�޸��û��˺�            |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          4.�����û��˺�            |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          5.��ʾ�û��˺�            |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          0.ע����¼                |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t -------------------------------------\n";
	cout << "��ѡ�����Ĳ����� " << endl;
 }

//�Ӳ˵�����2 ͼ�����˵�����
void Manager::operMenu1() 
{
	//ͼ��������
	cout << "��ӭ����Ա��" << this->m_Name << "��¼��" << endl;
	cout << "\t\t -------------------------------------\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          1.�� �� ͼ ��             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          2.ɾ �� ͼ ��             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          3.�� �� ͼ ��             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          4.�� �� ͼ ��             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          5.�� ʾ ͼ ��             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          0.ע����¼                |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t -------------------------------------\n";
	cout << "��ѡ�����Ĳ����� " << endl;
}

//��ʼ��ѧ������
void Manager::initVector()
{
	//ȷ���������״̬
	vStu.clear();

	//��ȡ��Ϣ   ѧ��
	ifstream ifs;
	ifs.open(STUDENT_FILE, ios::in);
	if (!ifs.is_open())
	{
		cout << "�ļ���ȡʧ��" << endl;
		return;
	}

	Student s;
	//���ļ���һ���ж�������Ϣ�Ž�����vStu��
	while (ifs >> s.m_Id && ifs >> s.m_Name && ifs >> s.m_Pwd)
	{
		vStu.push_back(s);
	}
		cout << "��ǰѧ������Ϊ�� " << vStu.size() << endl;
	ifs.close();
}

//��ʼ���鱾����
void Manager::initVector1()
{
	//ȷ���������״̬
	vBook.clear();

	//��ȡ��Ϣ   ͼ��
	ifstream ifs;
	ifs.open(BOOK_FILE, ios::in);
	//�ж��ļ��Ƿ�򿪳ɹ�
	if (!ifs.is_open())
	{
		cout << "�ļ���ȡʧ��" << endl;
		return;
	}

	BookData bd;
	//���ļ���һ���ж�������Ϣ�Ž�����vStu��
	while (ifs >> bd.m_BookId && ifs >> bd.m_BookName && ifs>>bd.m_BookState )
	{
		vBook.push_back(bd);
	}
	cout << "��ǰ�鱾����Ϊ�� " << vBook.size() << endl;
	ifs.close();
}


//����ظ� ����1 ���ѧ��
bool Manager::checkRepeat(int id)
{
		//���ѧ��
		for (vector<Student>::iterator it = vStu.begin(); it != vStu.end(); it++)
		{
			if (id == it->m_Id)
			{
				return true;
			}
		}
	
	return false;
}

//����ظ� ����1 ���ѧ��,�Է������ظ��鼮��ŵ��鱾
bool Manager::checkRepeat1(int B_id)
{
	//���ѧ��
	for (vector<BookData>::iterator it = vBook.begin(); it != vBook.end(); it++)
	{
		if (B_id == (*it).m_BookId)
		{
			//�ظ���������
			return true;
		}
	}
	//û�ظ������ؼ�
	return false;
}

//����û��˺�
void Manager::addPerson()
{
	string fileName; //�����ļ���
	ofstream ofs;//�ļ���������

	//��ӵ���ѧ��
	fileName = STUDENT_FILE;

	//����׷�ӵķ�ʽ д�ļ�
	ofs.open(fileName, ios::out | ios::app);

	int id; //ѧ�� 
	string name; //����
	string pwd; //����

	cout << "������ѧ�ţ�" << endl;

	while (true)
	{
		cin >> id;
		bool ret = checkRepeat(id);
		if (ret) //���ظ�
		{
			cout << "ѧ���ظ�������������ѧ��: " << endl;
		}
		else
		{
			break;
		}

	}

	cout << "������������ " << endl;
	cin >> name;

	cout << "���������룺 " << endl;
	cin >> pwd;

	//���ļ����������
	ofs <<endl<< id << " " << name << " " << pwd << " " << endl;
	cout << "��ӳɹ�!" << endl;

	system("pause");
	system("cls");

	ofs.close();

	//���ó�ʼ�������ӿڣ����»�ȡ�ļ��е����ݣ���������
	this->initVector();
}

//ɾ���û��˺�  
void Manager::delectPerson()
{
	int id;
	cout << "��������Ҫɾ����ѧ��ѧ��:  " << endl;
	cin >> id;


	//���������൱��ָ�� it
	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)
	{
		if ((*it).m_Id == id)
		{
			//���˴�λ��ɾ�� it 
			vStu.erase(it);
			cout << "ɾ���ɹ���" << endl;
			break;
		}
		else  //δʵ��
		{
			cout << "��ѧ��������! " << endl;
		}
	}

	//����ֻ����vector���޸������ݣ���Ҫ������ͬ�����ļ�txt��

	ofstream ofs(STUDENT_FILE, ios::trunc);  //����ļ����ڣ���ôɾ�ɾ��������´���
	//��������û����������Ҫ�������޸������������ݽ��µ��ļ���
	ofs.close();


	ofstream ofs1;//�ļ���������
	//����׷�ӵķ�ʽ д�ļ�
	ofs1.open(STUDENT_FILE, ios::out | ios::app);

	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)
	{
		ofs1 << endl << (*it).m_Id << " " << (*it).m_Name << " " << (*it).m_Pwd << " " << endl;
	}

	ofs1.close();


	system("pause");
	system("cls");
}

//�޸��û��˺�   
void Manager::modPerson()
{
	int n = 0;
	cout << "������Ҫ�޸ĵ�ѧ��ѧ�ţ� " << endl;
	cin >> n;

	//���������൱��ָ�� it
	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)
	{
		if ((*it).m_Id == n)
		{
			cout << "��ѧ��ԭ����Ϣ���£�" << endl;
			cout << "ѧ��Ϊ��" << (*it).m_Id << " " << "����Ϊ��" << (*it).m_Name << " " <<
				" ����Ϊ��" << (*it).m_Pwd <<" " << endl;
		}
	}

	cout << "�������ѧ���޸ĺ��ѧ�ţ� " << endl;
	int id;
	cin >> id;
	vStu[n - 1].m_Id = id;

	cout << "�������ѧ���޸ĺ��������" << endl;
	string name;
	cin >> name;
	vStu[n - 1].m_Name = name;

	cout << "�������ѧ���޸ĺ�����룺 " << endl;
	string password;
	cin >> password;
	vStu[n - 1].m_Pwd = password;
	//����ֻ����vector���޸������ݣ���Ҫ������ͬ�����ļ�txt��

	ofstream ofs(STUDENT_FILE, ios::trunc);  //����ļ����ڣ���ôɾ�ɾ��������´���
	//��������û����������Ҫ�������޸������������ݽ��µ��ļ���
	ofs.close();


	ofstream ofs1;//�ļ���������
	//����׷�ӵķ�ʽ д�ļ�
	ofs1.open(STUDENT_FILE, ios::out | ios::app);
	
	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)
	{
		//����������д���ļ���
		ofs1 << endl << (*it).m_Id << " " << (*it).m_Name << " " << (*it).m_Pwd << " " << endl;
	}

	ofs1.close();


	cout << "�޸ĳɹ���" << endl;
	system("pause");
	system("cls");
}

//�����û��˺�
void Manager::findPerson()
{
	int id = 0;
	cout << "��������Ҫ���ҵ�ѧ��id: " << endl;
	cin >> id;

	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)   //�������İ취
	{
		if (id == (*it).m_Id)  //���ҵ��˸�ѧ����id
		{
			cout << "�ҵ���"<<(*it).m_Id << "��ѧ���û�����ѧ��������Ϣ���£�" << endl;
			cout << "  ѧ��Ϊ��" << (*it).m_Id
				 << "  ����Ϊ: " << (*it).m_Name
				 << "  ����Ϊ��" << (*it).m_Pwd << endl;
		}
	}
	system("pause");
	system("cls");
}

////���㷨�Ļ�����Ҫ�ĺ���
//void printStudent(Student& s)
//{
//	cout << "ѧ�ţ� " << s.m_Id << " ������ " << s.m_Name << " ���룺" << s.m_Pwd << endl;
//}

//��ʾ�û��˺�
void Manager::showPerson()
{

	//�鿴ѧ��
	cout << "����ѧ����Ϣ���£�" << endl;
	////�㷨����һ���ڶ��������������ķ�Χ���������Ǻ���
	//for_each(vStu.begin(), vStu.end(), printStudent);
	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)   //�������İ취
	{
		cout << "ѧ��Ϊ��" << (*it).m_Id
			<< "  ����Ϊ��" << (*it).m_Name
			<< "  ����Ϊ��" << (*it).m_Pwd << endl;

	}

	system("pause");
	system("cls");
}



//���ͼ��
void Manager::addBook()
{
	string fileName; //�����ļ���
	ofstream ofs;//�ļ���������

	//��ӵ����鱾
	fileName = BOOK_FILE;

	//����׷�ӵķ�ʽ д�ļ�
	ofs.open(fileName, ios::out | ios::app);

	int id; //�鱾���
	string name; //�鼮����
	int state;  //�鱾״̬

	cout << "�������鱾��ţ�" << endl;

	while (true)
	{
		cin >> id;
		bool ret = checkRepeat1(id);
		if (ret) //���ظ�
		{
			cout << "�鱾����ظ���������������: " << endl;
		}
		else
		{
			break;
		}
	}

	cout << "�������鼮���֣� " << endl;
	cin >> name;

	state = 0;

	//���ļ����������
	ofs << endl << id << " " 
		        << name << " " 
		        <<state <<" " << endl;
	cout << "��ӳɹ�!" << endl;

	system("pause");
	system("cls");

	ofs.close();

	//���ó�ʼ�������ӿڣ����»�ȡ�ļ��е����ݣ���������
	this->initVector1();

}

//ɾ��ͼ��
void Manager::delectBook()
{
	int id;
	cout << "��������Ҫɾ�����鱾���:  " << endl;
	cin >> id;


	//���������൱��ָ�� it
	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)
	{
		if ((*it).m_BookId == id)
		{
			//���˴�λ��ɾ�� it 
			vBook.erase(it);
			cout << "ɾ���ɹ���" << endl;
			break;
		}
		else  //δʵ��
		{
			cout << "���鱾������! " << endl;
		}
	}

	//����ֻ����vector���޸������ݣ���Ҫ������ͬ�����ļ�txt��

	ofstream ofs(BOOK_FILE, ios::trunc);  //����ļ����ڣ���ôɾ�ɾ��������´���
	//��������û����������Ҫ�������޸������������ݽ��µ��ļ���
	ofs.close();


	ofstream ofs1;//�ļ���������
	//����׷�ӵķ�ʽ д�ļ�
	ofs1.open(BOOK_FILE, ios::out | ios::app);

	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)
	{
		ofs1 << endl << (*it).m_BookId << " " 
			         << (*it).m_BookName  << " " << endl;
	}

	ofs1.close();

	//���ó�ʼ�������ӿڣ����»�ȡ�ļ��е����ݣ���������
	this->initVector1();
	system("pause");
	system("cls");
}

//�޸�ͼ��
void Manager::modBook()
{
	string tip;  //��ʾ�鱾��״̬
	int n = 0;
	cout << "������Ҫ�޸ĵ��鱾��ţ� " << endl;
	cin >> n;

	//���������൱��ָ�� it
	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)
	{
		if ((*it).m_BookState == 1)
		{
			//�ѽ��
			tip = "�ѽ����";
		}
		else
		{
			tip = "δ�����";
		}

		if ((*it).m_BookId == n)
		{
			cout << "���鼮ԭ����Ϣ���£�" << endl;
			cout << "�鱾���Ϊ��" << (*it).m_BookId << " " 
				 << "�鱾����Ϊ��" << (*it).m_BookName << " "  
				 <<"�鱾״̬Ϊ�� "<<tip <<" " << endl;
		}
	}

	cout << "��������鱾�޸ĺ�ı�ţ� " << endl;
	int id;
	cin >> id;
	vBook[n - 1].m_BookId = id;

	cout << "��������鱾�޸ĺ��������" << endl;
	string name;
	cin >> name;
	vBook[n - 1].m_BookName = name;

	cout << "��������鱾�޸ĺ��״̬��" << endl;
	cout << "����1���ѽ��״̬ " << endl;
	cout << "����0��δ���״̬ " << endl;
	int a;  // 1   0
	cin >> a;
	vBook[n - 1].m_BookState = a;

	//����ֻ����vector���޸������ݣ���Ҫ������ͬ�����ļ�txt��

	ofstream ofs(BOOK_FILE, ios::trunc);  //����ļ����ڣ���ôɾ�ɾ��������´���
	//��������û����������Ҫ�������޸������������ݽ��µ��ļ���
	ofs.close();


	ofstream ofs1;//�ļ���������
	//����׷�ӵķ�ʽ д�ļ�
	ofs1.open(BOOK_FILE, ios::out | ios::app);

	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)
	{
		//����������д���ļ���
		ofs1 << endl << (*it).m_BookId 
			<< " "   << (*it).m_BookName 
			<< " " <<(*it).m_BookState<<" " << endl;
	} 


	ofs1.close();

	//���ó�ʼ�������ӿڣ����»�ȡ�ļ��е����ݣ���������
	this->initVector1();

	cout << "�޸ĳɹ���" << endl;
	system("pause");
	system("cls");
}

//����ͼ��
void Manager::findBook()
{
	string tip;
	int id = 0;
	cout << "��������Ҫ���ҵ��鱾���: " << endl;
	cin >> id;

	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)   //�������İ취
	{
		if ((*it).m_BookState == 1)
		{
			//�ѽ��
			tip = "�ѽ����";
		}
		else
		{
			tip = "δ�����";
		}

		if (id == (*it).m_BookId)  //���ҵ��˸��鱾��id
		{
			cout << "�ҵ���" << (*it).m_BookId << "���鱾�����飬����Ϣ���£�" << endl;
			cout << "  �鱾���Ϊ��" << (*it).m_BookId
				<< "  �鱾����Ϊ: " << (*it).m_BookName
				<<"  �鱾״̬: "<<tip << endl;
		}
	}

	system("pause");
	system("cls");
}

//��ʾͼ��
void Manager::showBook()
{
	string tip;  //��ʾ�鱾��״̬

	//��ʾ�����鱾
	cout << "����ͼ����Ϣ���£�" << endl;
	////�㷨����һ���ڶ��������������ķ�Χ���������Ǻ���
	//for_each(vStu.begin(), vStu.end(), printStudent);
	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)   //�������İ취
	{
		if ((*it).m_BookState == 1)
		{
			//�ѽ��
			tip = "�ѽ����";
		}
		else
		{
			tip = "δ�����";
		}
		cout << "�鱾���Ϊ��" << (*it).m_BookId
			<< "  ����Ϊ��" << (*it).m_BookName 
			<<"  �鱾״̬: "<<tip << endl;

	}

	system("pause");
	system("cls");
}
