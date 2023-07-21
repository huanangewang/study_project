#include <iostream>
using namespace std;
#include"manager.h"
#include"student.h"
#include <fstream>
#include <string>

//����ѧ���Ӳ˵�1����(��ʾͼ��Ͳ���ͼ��Ľ���)
void studentMenu(Identity*  &student)
{
	while (true)
	{
		//����ѧ���Ӳ˵�
		student->operMenu1();

		//������ָ�� תΪ����ָ�룬���������������ӿ�
		Student* stu = (Student*)student;

		int select = 0;
		cin >> select; //�����û�ѡ��

		if (select == 1)      //��ʾ����ͼ��
		{
			stu->showBook();
		}
		else if (select == 2)  //�鿴��Ӧ���ͼ��,������
		{
			stu->findBook();
		}
		else if (select == 3)   //����
		{
			stu->returnBook();
		}
		else
		{
			//ע����¼
			delete student;
			cout << "ע���ɹ�" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}


//�������Ա �û�������� �Ӳ˵�
void managerMenu(Identity*& manager)
{
	while (true)
	{
		//���ù���Ա�Ӳ˵�
		manager->operMenu();

		//������ָ�� תΪ����ָ�룬���������������ӿ�
		Manager* man = (Manager*)manager;

		int select = 0;
		//�����û�ѡ��
		cin >> select;

		if (select == 1) //����û��˺�
		{
			man->addPerson();
		}
		else if (select == 2) //ɾ���û��˺�
		{
			man->delectPerson();
		}
		else if (select == 3) //�޸��û��˺�
		{
			man->modPerson();
		}
		else if (select == 4) //�����û��˺�
		{
			man->findPerson();
		}
		else if (select == 5)  //��ʾ�û��˺�
		{
			man->showPerson();
		}
		else
		{
			//ע��
			delete manager; //���ٵ���������
			cout << "ע���ɹ�" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}

//�������Ա ͼ�������� �Ӳ˵�
void managerMenu1(Identity* &tushu)
{
	while (true)
	{
		//���ù���Ա�Ӳ˵�
		tushu->operMenu1();

		//������ָ�� תΪ����ָ�룬���������������ӿ�
		Manager* book = (Manager*)tushu;

		int select = 0;
		//�����û�ѡ��
		cin >> select;

		if (select == 1)    //���ͼ��
		{
			book->addBook();
		}
		else if (select == 2) //ɾ��ͼ��
		{
			book->delectBook();
		}
		else if (select == 3) //�޸�ͼ��
		{
			book->modBook();
		}
		else if (select == 4) //����ͼ��
		{
			book->findBook();
		}
		else if (select == 5)  //��ʾͼ��
		{
			book->showBook();
		}
		else
		{
			//ע��
			delete tushu; //���ٵ���������
			cout << "ע���ɹ�" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}


//��¼����   ����1  �����ļ���   ����2  �����������
void LoginIn(string fileName, int type)
{
	//����ָ�� ������ָ���������
	Identity*  person = NULL;

	//���ļ�
	ifstream ifs;
	ifs.open(fileName, ios::in);

	//�ж��ļ��Ƿ����
	if (!ifs.is_open())
	{
		cout << "�ļ�������" << endl;
		ifs.close();
		return;
	}

	//׼�������û�����Ϣ
	int id = 0;
	string name;
	string pwd;


	//�ж����
	if (type == 1) //ѧ�����
	{
		cout << "���������ѧ�ţ�" << endl;
		cin >> id;
	}

	cout << "�������û�����" << endl;
	cin >> name;

	cout << "���������룺" << endl;
	cin >> pwd;

	if (type == 1)
	{
		//ѧ�������֤
		int fId; //���ļ��ж�ȡ��id��
		string fName; //���ļ��л�ȡ������
		string fPwd; //���ļ��л�ȡ����
		while (ifs >> fId && ifs >> fName && ifs >> fPwd)
		{
			//���û��������Ϣ���Ա�
			if (fId == id && fName == name && fPwd == pwd)
			{
				cout << "ѧ����֤��¼�ɹ���" << endl;
				system("pause");
				system("cls");
				person = new Student(id, name, pwd);


				//����ѧ����ݵ��Ӳ˵�
				studentMenu(person);
				return;
			}
		}
	}
	
	else if (type == 2)
	{
		//����Ա�����֤
		string fName; //���ļ��л�ȡ����
		string fPwd; //���ļ��л�ȡ����

		while (ifs >> fName && ifs >> fPwd)
		{
			if (name == fName && pwd == fPwd)
			{
				cout << "����Ա��֤��¼�ɹ�!" << endl;
				system("pause");
				system("cls");

				person = new Manager(name, pwd);

				//�������Ա�Ӳ˵�����
				//һ�ǽ����û��������
				//���ǽ���ͼ��������
				cout << "��ѡ����Ҫ����Ľ��棺" << endl;
				cout << "------------------------------------------ " << endl;
				cout << "--------------1.�û��������-------------- " << endl;
				cout << "------------------------------------------ " << endl;
				cout << "--------------2.ͼ��������-------------- " << endl;
				cout << "------------------------------------------ " << endl;
				int choice = 0;
				cin >> choice;
				if(choice==1)  //�û��������
				{
					managerMenu(person);
				}
				else   //ͼ��������
				{
					managerMenu1(person);
				}
				
				return;
			}
		}

	}

	cout << "��֤��¼ʧ�ܣ�" << endl;
	system("pause");
	system("cls");

	return;
}


int main()
{
	int select = 0; //���ڽ����û���ѡ��

	while (true)
	{
		cout << "======================  ��ӭʹ��ͼ�����ϵͳ  =====================" << endl;
		cout << endl << "�������������" << endl;
		cout << "\t\t -------------------------------\n";
		cout << "\t\t|                               |\n";
		cout << "\t\t|          1.ѧ    ��           |\n";
		cout << "\t\t|                               |\n";
		cout << "\t\t|          2.�� �� Ա           |\n";
		cout << "\t\t|                               |\n";
		cout << "\t\t|          0.��    ��           |\n";
		cout << "\t\t|                               |\n";
		cout << "\t\t -------------------------------\n";
		cout << "��������ѡ��: ";

		cin >> select; //�����û�ѡ��

		switch (select) //�����û�ѡ�� ʵ�ֲ�ͬ�ӿ�
		{
		case 1: //ѧ�����
			LoginIn(STUDENT_FILE, 1);
			break;
		case 2: //����Ա���
			LoginIn(ADMIN_FILE, 2);
			break;
		case 0:  //�˳�ϵͳ
			cout << "��ӭ��һ��ʹ��" << endl;
			system("pause");
			return 0;
			break;
		default:
			cout << "��������������ѡ��" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
	system("pause");
	return 0;
}