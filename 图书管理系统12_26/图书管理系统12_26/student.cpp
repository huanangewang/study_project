#include"student.h"
#include<fstream>

//默认构造
Student::Student()
{

}

//有参构造  参数：学号、姓名、密码
Student::Student(int id, string name, string pwd)
{
	//初始化属性
	this->m_Id = id;
	this->m_Name = name;
	this->m_Pwd = pwd;

	//初始化书本容器 获取到所有文件中 书本
	this->initVector1();



}

//菜单界面
void Student::operMenu()    //舍
{
	cout << "欢迎学生代表：" << this->m_Name << "登录！" << endl;
	cout << "\t\t ----------------------------------\n";
	cout << "\t\t|                                  |\n";
	cout << "\t\t|          1.借    书              |\n";
	cout << "\t\t|                                  |\n";
	cout << "\t\t|          2.还    书              |\n";
	cout << "\t\t|                                  |\n";
	cout << "\t\t|          0.注销登录              |\n";
	cout << "\t\t|                                  |\n";
	cout << "\t\t ----------------------------------\n";
	cout << "请选择您的操作： " << endl;
 }


//子菜单界面2 图书管理菜单界面   
void Student::operMenu1()
{
	//图书管理界面
	cout << "欢迎学生：" << this->m_Name << "登录！" << endl;
	cout << "\t\t -------------------------------------\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          1.显 示 图 书             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          2.查 找 图 书             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          3.归 还 图 书             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          0.注销登录                |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t -------------------------------------\n";
	cout << "请选择您的操作： " << endl;
}


//初始化书本容器 获取到所有文件中 书本
void Student::initVector1()
{
	//确保容器清空状态
	vBook.clear();

	//读取信息   图书
	ifstream ifs;
	ifs.open(BOOK_FILE, ios::in);
	//判断文件是否打开成功
	if (!ifs.is_open())
	{
		cout << "文件读取失败" << endl;
		return;
	}
	
	BookData bd;
	//bd.m_BookState = 0;  //未借出
	//从文件中一行行读出来信息放进容器vStu内
	while (ifs >> bd.m_BookId && ifs >> bd.m_BookName && ifs>>bd.m_BookState)
	{
		vBook.push_back(bd);
	}
	cout << "当前书本数量为： " << vBook.size() << endl;
	ifs.close();
}


//还书
void Student::returnBook()
{
	//更改标记,也就是将1改成0
	//同步文件记录
	int id = 0;
	cout << "请输入你要归还的书本编号: " << endl;
	cin >> id;
	string tip;  //书本状态提示

	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)   //迭代器的办法
	{
		if (id == (*it).m_BookId)  //查找到了该书本的id
		{
			if ((*it).m_BookState == 1)
			{
				//已借出
				tip = "已借出！";
			}
			else
			{
				tip = "未借出！";
			}
			cout << "找到了" << (*it).m_BookId << "号书本，该书，其信息如下：" << endl;
			cout << "  书本编号为：" << (*it).m_BookId
				<< "  书本名字为: " << (*it).m_BookName
				<< "  书本状态为： " << tip << endl;
			system("pause");

			//还书
			//其实就是更改书本状态
			cout << "您确认归还此书吗？" << endl;
			cout << "1.确认！" << endl;
			cout << "2.返回！" << endl;
			int select = 0;
			cin >> select;
			if (select == 1)//用户确认还书
			{
				(*it).m_BookState = 0;  //变成未借出状态

			}

		}
	}
	//以上只是在vector中修改了数据，还要将数据同步到文件txt中

	ofstream ofs(BOOK_FILE, ios::trunc);  //如果文件存在，那么删干净，再重新创建
	//现在里面没东西，我们要加入已修改完后的容器内容进新的文件中
	ofs.close();


	ofstream ofs1;//文件操作对象
	//利用追加的方式 写文件
	ofs1.open(BOOK_FILE, ios::out | ios::app);

	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)
	{
		ofs1 << endl << (*it).m_BookId << " "
			<< (*it).m_BookName << " "
			<< (*it).m_BookState << endl;
	}

	ofs1.close();

	//更新
	this->initVector1();

	system("pause");
	system("cls");
}


//查找图书
void Student::findBook()
{
	int id = 0;
	cout << "请输入你要查找的书本编号: " << endl;
	cin >> id;
	string tip;  //书本状态提示

	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)   //迭代器的办法
	{
		if (id == (*it).m_BookId)  //查找到了该书本的id
		{
			if ((*it).m_BookState == 1)
			{
				//已借出
				tip = "已借出！";
			}
			else
			{
				tip = "未借出！";
			}
			cout << "找到了" << (*it).m_BookId << "号书本，该书，其信息如下：" << endl;
			cout << "  书本编号为：" << (*it).m_BookId
				<< "  书本名字为: " << (*it).m_BookName
				<<"  书本状态为： "<<tip << endl;
			     system("pause");

				 //要未借出状态才可以借出,也就是=0
				 if ((*it).m_BookState == 0)
				 {
					 //借书
					 cout << "你需要借这本书吗？" << endl;
					 cout << "1.需要借: " << endl;
					 cout << "2.不需要借" << endl;
					 int select = 0;
					 cin >> select;

					 if (select == 1)  //需要借书
					 {
						 //给该书打上标记
						 (*it).m_BookState = 1; //已被借出
						 cout << "借书已完成，欢迎下次使用！" << endl;
					 }
				 }
			
		}
	}
	//以上只是在vector中修改了数据，还要将数据同步到文件txt中

	ofstream ofs(BOOK_FILE, ios::trunc);  //如果文件存在，那么删干净，再重新创建
	//现在里面没东西，我们要加入已修改完后的容器内容进新的文件中
	ofs.close();


	ofstream ofs1;//文件操作对象
	//利用追加的方式 写文件
	ofs1.open(BOOK_FILE, ios::out | ios::app);

	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)
	{
		ofs1 << endl << (*it).m_BookId << " "
			<< (*it).m_BookName << " " 
			<<(*it).m_BookState << endl;
	}

	ofs1.close();

	//更新
	this->initVector1();

	system("pause");
	system("cls");
}



//显示图书
void Student::showBook()
{
	string tip;  //书本状态提示
	
	//显示所有书本
	cout << "所有图书信息如下：" << endl;
	////算法，第一个第二个参数是容器的范围，第三个是函数
	//for_each(vStu.begin(), vStu.end(), printStudent);
	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)   //迭代器的办法
	{
		if ((*it).m_BookState == 1)
		{
			//已借出
			tip = "已借出！";
		}
		else
		{
			tip = "未借出！";
		}

		cout << "书本编号为：" << (*it).m_BookId
			<< "  书名为：" << (*it).m_BookName 
			<<"  书本状态为: "<<tip << endl;

	}

	system("pause");
	system("cls");
}