#include <iostream>
using namespace std;
#include"manager.h"
#include"student.h"
#include <fstream>
#include <string>

//进入学生子菜单1界面(显示图书和查找图书的界面)
void studentMenu(Identity*  &student)
{
	while (true)
	{
		//调用学生子菜单
		student->operMenu1();

		//将父类指针 转为子类指针，调用子类里其他接口
		Student* stu = (Student*)student;

		int select = 0;
		cin >> select; //接受用户选择

		if (select == 1)      //显示所有图书
		{
			stu->showBook();
		}
		else if (select == 2)  //查看对应编号图书,并借书
		{
			stu->findBook();
		}
		else if (select == 3)   //还书
		{
			stu->returnBook();
		}
		else
		{
			//注销登录
			delete student;
			cout << "注销成功" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}


//进入管理员 用户管理界面 子菜单
void managerMenu(Identity*& manager)
{
	while (true)
	{
		//调用管理员子菜单
		manager->operMenu();

		//将父类指针 转为子类指针，调用子类里其他接口
		Manager* man = (Manager*)manager;

		int select = 0;
		//接受用户选项
		cin >> select;

		if (select == 1) //添加用户账号
		{
			man->addPerson();
		}
		else if (select == 2) //删除用户账号
		{
			man->delectPerson();
		}
		else if (select == 3) //修改用户账号
		{
			man->modPerson();
		}
		else if (select == 4) //查找用户账号
		{
			man->findPerson();
		}
		else if (select == 5)  //显示用户账号
		{
			man->showPerson();
		}
		else
		{
			//注销
			delete manager; //销毁掉堆区对象
			cout << "注销成功" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}

//进入管理员 图书管理界面 子菜单
void managerMenu1(Identity* &tushu)
{
	while (true)
	{
		//调用管理员子菜单
		tushu->operMenu1();

		//将父类指针 转为子类指针，调用子类里其他接口
		Manager* book = (Manager*)tushu;

		int select = 0;
		//接受用户选项
		cin >> select;

		if (select == 1)    //添加图书
		{
			book->addBook();
		}
		else if (select == 2) //删除图书
		{
			book->delectBook();
		}
		else if (select == 3) //修改图书
		{
			book->modBook();
		}
		else if (select == 4) //查找图书
		{
			book->findBook();
		}
		else if (select == 5)  //显示图书
		{
			book->showBook();
		}
		else
		{
			//注销
			delete tushu; //销毁掉堆区对象
			cout << "注销成功" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}


//登录功能   参数1  操作文件名   参数2  操作身份类型
void LoginIn(string fileName, int type)
{
	//父类指针 ，用于指向子类对象
	Identity*  person = NULL;

	//读文件
	ifstream ifs;
	ifs.open(fileName, ios::in);

	//判断文件是否存在
	if (!ifs.is_open())
	{
		cout << "文件不存在" << endl;
		ifs.close();
		return;
	}

	//准备接受用户的信息
	int id = 0;
	string name;
	string pwd;


	//判断身份
	if (type == 1) //学生身份
	{
		cout << "请输入你的学号：" << endl;
		cin >> id;
	}

	cout << "请输入用户名：" << endl;
	cin >> name;

	cout << "请输入密码：" << endl;
	cin >> pwd;

	if (type == 1)
	{
		//学生身份验证
		int fId; //从文件中读取的id号
		string fName; //从文件中获取的姓名
		string fPwd; //从文件中获取密码
		while (ifs >> fId && ifs >> fName && ifs >> fPwd)
		{
			//与用户输入的信息做对比
			if (fId == id && fName == name && fPwd == pwd)
			{
				cout << "学生验证登录成功！" << endl;
				system("pause");
				system("cls");
				person = new Student(id, name, pwd);


				//进入学生身份的子菜单
				studentMenu(person);
				return;
			}
		}
	}
	
	else if (type == 2)
	{
		//管理员身份验证
		string fName; //从文件中获取姓名
		string fPwd; //从文件中获取密码

		while (ifs >> fName && ifs >> fPwd)
		{
			if (name == fName && pwd == fPwd)
			{
				cout << "管理员验证登录成功!" << endl;
				system("pause");
				system("cls");

				person = new Manager(name, pwd);

				//进入管理员子菜单界面
				//一是进入用户管理界面
				//二是进入图书管理界面
				cout << "请选择你要进入的界面：" << endl;
				cout << "------------------------------------------ " << endl;
				cout << "--------------1.用户管理界面-------------- " << endl;
				cout << "------------------------------------------ " << endl;
				cout << "--------------2.图书管理界面-------------- " << endl;
				cout << "------------------------------------------ " << endl;
				int choice = 0;
				cin >> choice;
				if(choice==1)  //用户管理界面
				{
					managerMenu(person);
				}
				else   //图书管理界面
				{
					managerMenu1(person);
				}
				
				return;
			}
		}

	}

	cout << "验证登录失败！" << endl;
	system("pause");
	system("cls");

	return;
}


int main()
{
	int select = 0; //用于接受用户的选择

	while (true)
	{
		cout << "======================  欢迎使用图书管理系统  =====================" << endl;
		cout << endl << "请输入您的身份" << endl;
		cout << "\t\t -------------------------------\n";
		cout << "\t\t|                               |\n";
		cout << "\t\t|          1.学    生           |\n";
		cout << "\t\t|                               |\n";
		cout << "\t\t|          2.管 理 员           |\n";
		cout << "\t\t|                               |\n";
		cout << "\t\t|          0.退    出           |\n";
		cout << "\t\t|                               |\n";
		cout << "\t\t -------------------------------\n";
		cout << "输入您的选择: ";

		cin >> select; //接受用户选择

		switch (select) //根据用户选择 实现不同接口
		{
		case 1: //学生身份
			LoginIn(STUDENT_FILE, 1);
			break;
		case 2: //管理员身份
			LoginIn(ADMIN_FILE, 2);
			break;
		case 0:  //退出系统
			cout << "欢迎下一次使用" << endl;
			system("pause");
			return 0;
			break;
		default:
			cout << "输入有误，请重新选择！" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
	system("pause");
	return 0;
}