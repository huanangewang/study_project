#include"student.h"
#include<fstream>

//Ĭ�Ϲ���
Student::Student()
{

}

//�вι���  ������ѧ�š�����������
Student::Student(int id, string name, string pwd)
{
	//��ʼ������
	this->m_Id = id;
	this->m_Name = name;
	this->m_Pwd = pwd;

	//��ʼ���鱾���� ��ȡ�������ļ��� �鱾
	this->initVector1();



}

//�˵�����
void Student::operMenu()    //��
{
	cout << "��ӭѧ������" << this->m_Name << "��¼��" << endl;
	cout << "\t\t ----------------------------------\n";
	cout << "\t\t|                                  |\n";
	cout << "\t\t|          1.��    ��              |\n";
	cout << "\t\t|                                  |\n";
	cout << "\t\t|          2.��    ��              |\n";
	cout << "\t\t|                                  |\n";
	cout << "\t\t|          0.ע����¼              |\n";
	cout << "\t\t|                                  |\n";
	cout << "\t\t ----------------------------------\n";
	cout << "��ѡ�����Ĳ����� " << endl;
 }


//�Ӳ˵�����2 ͼ�����˵�����   
void Student::operMenu1()
{
	//ͼ��������
	cout << "��ӭѧ����" << this->m_Name << "��¼��" << endl;
	cout << "\t\t -------------------------------------\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          1.�� ʾ ͼ ��             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          2.�� �� ͼ ��             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          3.�� �� ͼ ��             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          0.ע����¼                |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t -------------------------------------\n";
	cout << "��ѡ�����Ĳ����� " << endl;
}


//��ʼ���鱾���� ��ȡ�������ļ��� �鱾
void Student::initVector1()
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
	//bd.m_BookState = 0;  //δ���
	//���ļ���һ���ж�������Ϣ�Ž�����vStu��
	while (ifs >> bd.m_BookId && ifs >> bd.m_BookName && ifs>>bd.m_BookState)
	{
		vBook.push_back(bd);
	}
	cout << "��ǰ�鱾����Ϊ�� " << vBook.size() << endl;
	ifs.close();
}


//����
void Student::returnBook()
{
	//���ı��,Ҳ���ǽ�1�ĳ�0
	//ͬ���ļ���¼
	int id = 0;
	cout << "��������Ҫ�黹���鱾���: " << endl;
	cin >> id;
	string tip;  //�鱾״̬��ʾ

	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)   //�������İ취
	{
		if (id == (*it).m_BookId)  //���ҵ��˸��鱾��id
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
			cout << "�ҵ���" << (*it).m_BookId << "���鱾�����飬����Ϣ���£�" << endl;
			cout << "  �鱾���Ϊ��" << (*it).m_BookId
				<< "  �鱾����Ϊ: " << (*it).m_BookName
				<< "  �鱾״̬Ϊ�� " << tip << endl;
			system("pause");

			//����
			//��ʵ���Ǹ����鱾״̬
			cout << "��ȷ�Ϲ黹������" << endl;
			cout << "1.ȷ�ϣ�" << endl;
			cout << "2.���أ�" << endl;
			int select = 0;
			cin >> select;
			if (select == 1)//�û�ȷ�ϻ���
			{
				(*it).m_BookState = 0;  //���δ���״̬

			}

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
			<< (*it).m_BookName << " "
			<< (*it).m_BookState << endl;
	}

	ofs1.close();

	//����
	this->initVector1();

	system("pause");
	system("cls");
}


//����ͼ��
void Student::findBook()
{
	int id = 0;
	cout << "��������Ҫ���ҵ��鱾���: " << endl;
	cin >> id;
	string tip;  //�鱾״̬��ʾ

	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)   //�������İ취
	{
		if (id == (*it).m_BookId)  //���ҵ��˸��鱾��id
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
			cout << "�ҵ���" << (*it).m_BookId << "���鱾�����飬����Ϣ���£�" << endl;
			cout << "  �鱾���Ϊ��" << (*it).m_BookId
				<< "  �鱾����Ϊ: " << (*it).m_BookName
				<<"  �鱾״̬Ϊ�� "<<tip << endl;
			     system("pause");

				 //Ҫδ���״̬�ſ��Խ��,Ҳ����=0
				 if ((*it).m_BookState == 0)
				 {
					 //����
					 cout << "����Ҫ���Ȿ����" << endl;
					 cout << "1.��Ҫ��: " << endl;
					 cout << "2.����Ҫ��" << endl;
					 int select = 0;
					 cin >> select;

					 if (select == 1)  //��Ҫ����
					 {
						 //��������ϱ��
						 (*it).m_BookState = 1; //�ѱ����
						 cout << "��������ɣ���ӭ�´�ʹ�ã�" << endl;
					 }
				 }
			
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
			<< (*it).m_BookName << " " 
			<<(*it).m_BookState << endl;
	}

	ofs1.close();

	//����
	this->initVector1();

	system("pause");
	system("cls");
}



//��ʾͼ��
void Student::showBook()
{
	string tip;  //�鱾״̬��ʾ
	
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
			<<"  �鱾״̬Ϊ: "<<tip << endl;

	}

	system("pause");
	system("cls");
}