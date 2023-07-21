#include"manager.h"
#include<fstream>

//默认构造
Manager::Manager()
{

}

//有参构造
Manager::Manager(string name, string pwd)
{
	//初始化管理员信息
	this->m_Name = name;
	this->m_Pwd = pwd;

	//初始化学生容器 获取到所有文件中 学生
	this->initVector();

	//初始化书本容器 获取到所有文件中 书本
	this->initVector1();
	
}

//子菜单界面
void Manager::operMenu()
{
	//用户管理界面
	cout << "欢迎管理员：" << this->m_Name << "登录！" << endl;
	cout << "\t\t -------------------------------------\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          1.添加用户账号            |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          2.删除用户账号            |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          3.修改用户账号            |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          4.查找用户账号            |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          5.显示用户账号            |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          0.注销登录                |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t -------------------------------------\n";
	cout << "请选择您的操作： " << endl;
 }

//子菜单界面2 图书管理菜单界面
void Manager::operMenu1() 
{
	//图书管理界面
	cout << "欢迎管理员：" << this->m_Name << "登录！" << endl;
	cout << "\t\t -------------------------------------\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          1.添 加 图 书             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          2.删 除 图 书             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          3.修 改 图 书             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          4.查 找 图 书             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          5.显 示 图 书             |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t|          0.注销登录                |\n";
	cout << "\t\t|                                    |\n";
	cout << "\t\t -------------------------------------\n";
	cout << "请选择您的操作： " << endl;
}

//初始化学生容器
void Manager::initVector()
{
	//确保容器清空状态
	vStu.clear();

	//读取信息   学生
	ifstream ifs;
	ifs.open(STUDENT_FILE, ios::in);
	if (!ifs.is_open())
	{
		cout << "文件读取失败" << endl;
		return;
	}

	Student s;
	//从文件中一行行读出来信息放进容器vStu内
	while (ifs >> s.m_Id && ifs >> s.m_Name && ifs >> s.m_Pwd)
	{
		vStu.push_back(s);
	}
		cout << "当前学生数量为： " << vStu.size() << endl;
	ifs.close();
}

//初始化书本容器
void Manager::initVector1()
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
	//从文件中一行行读出来信息放进容器vStu内
	while (ifs >> bd.m_BookId && ifs >> bd.m_BookName && ifs>>bd.m_BookState )
	{
		vBook.push_back(bd);
	}
	cout << "当前书本数量为： " << vBook.size() << endl;
	ifs.close();
}


//检测重复 参数1 检测学号
bool Manager::checkRepeat(int id)
{
		//检测学生
		for (vector<Student>::iterator it = vStu.begin(); it != vStu.end(); it++)
		{
			if (id == it->m_Id)
			{
				return true;
			}
		}
	
	return false;
}

//检测重复 参数1 检测学号,以防加入重复书籍编号的书本
bool Manager::checkRepeat1(int B_id)
{
	//检测学生
	for (vector<BookData>::iterator it = vBook.begin(); it != vBook.end(); it++)
	{
		if (B_id == (*it).m_BookId)
		{
			//重复，返回真
			return true;
		}
	}
	//没重复，返回假
	return false;
}

//添加用户账号
void Manager::addPerson()
{
	string fileName; //操作文件名
	ofstream ofs;//文件操作对象

	//添加的是学生
	fileName = STUDENT_FILE;

	//利用追加的方式 写文件
	ofs.open(fileName, ios::out | ios::app);

	int id; //学号 
	string name; //姓名
	string pwd; //密码

	cout << "请输入学号：" << endl;

	while (true)
	{
		cin >> id;
		bool ret = checkRepeat(id);
		if (ret) //有重复
		{
			cout << "学号重复，请重新输入学号: " << endl;
		}
		else
		{
			break;
		}

	}

	cout << "请输入姓名： " << endl;
	cin >> name;

	cout << "请输入密码： " << endl;
	cin >> pwd;

	//向文件中添加数据
	ofs <<endl<< id << " " << name << " " << pwd << " " << endl;
	cout << "添加成功!" << endl;

	system("pause");
	system("cls");

	ofs.close();

	//调用初始化容器接口，从新获取文件中的数据，更新数据
	this->initVector();
}

//删除用户账号  
void Manager::delectPerson()
{
	int id;
	cout << "请输入所要删除的学生学号:  " << endl;
	cin >> id;


	//迭代器就相当于指针 it
	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)
	{
		if ((*it).m_Id == id)
		{
			//将此处位置删除 it 
			vStu.erase(it);
			cout << "删除成功！" << endl;
			break;
		}
		else  //未实现
		{
			cout << "该学生不存在! " << endl;
		}
	}

	//以上只是在vector中修改了数据，还要将数据同步到文件txt中

	ofstream ofs(STUDENT_FILE, ios::trunc);  //如果文件存在，那么删干净，再重新创建
	//现在里面没东西，我们要加入已修改完后的容器内容进新的文件中
	ofs.close();


	ofstream ofs1;//文件操作对象
	//利用追加的方式 写文件
	ofs1.open(STUDENT_FILE, ios::out | ios::app);

	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)
	{
		ofs1 << endl << (*it).m_Id << " " << (*it).m_Name << " " << (*it).m_Pwd << " " << endl;
	}

	ofs1.close();


	system("pause");
	system("cls");
}

//修改用户账号   
void Manager::modPerson()
{
	int n = 0;
	cout << "请输入要修改的学生学号： " << endl;
	cin >> n;

	//迭代器就相当于指针 it
	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)
	{
		if ((*it).m_Id == n)
		{
			cout << "该学生原本信息如下：" << endl;
			cout << "学号为：" << (*it).m_Id << " " << "姓名为：" << (*it).m_Name << " " <<
				" 密码为：" << (*it).m_Pwd <<" " << endl;
		}
	}

	cout << "请输入该学生修改后的学号： " << endl;
	int id;
	cin >> id;
	vStu[n - 1].m_Id = id;

	cout << "请输入该学生修改后的姓名：" << endl;
	string name;
	cin >> name;
	vStu[n - 1].m_Name = name;

	cout << "请输入该学生修改后的密码： " << endl;
	string password;
	cin >> password;
	vStu[n - 1].m_Pwd = password;
	//以上只是在vector中修改了数据，还要将数据同步到文件txt中

	ofstream ofs(STUDENT_FILE, ios::trunc);  //如果文件存在，那么删干净，再重新创建
	//现在里面没东西，我们要加入已修改完后的容器内容进新的文件中
	ofs.close();


	ofstream ofs1;//文件操作对象
	//利用追加的方式 写文件
	ofs1.open(STUDENT_FILE, ios::out | ios::app);
	
	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)
	{
		//将容器内容写入文件中
		ofs1 << endl << (*it).m_Id << " " << (*it).m_Name << " " << (*it).m_Pwd << " " << endl;
	}

	ofs1.close();


	cout << "修改成功！" << endl;
	system("pause");
	system("cls");
}

//查找用户账号
void Manager::findPerson()
{
	int id = 0;
	cout << "请输入你要查找的学生id: " << endl;
	cin >> id;

	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)   //迭代器的办法
	{
		if (id == (*it).m_Id)  //查找到了该学生的id
		{
			cout << "找到了"<<(*it).m_Id << "号学生用户，该学生，其信息如下：" << endl;
			cout << "  学号为：" << (*it).m_Id
				 << "  姓名为: " << (*it).m_Name
				 << "  密码为：" << (*it).m_Pwd << endl;
		}
	}
	system("pause");
	system("cls");
}

////用算法的话，需要的函数
//void printStudent(Student& s)
//{
//	cout << "学号： " << s.m_Id << " 姓名： " << s.m_Name << " 密码：" << s.m_Pwd << endl;
//}

//显示用户账号
void Manager::showPerson()
{

	//查看学生
	cout << "所有学生信息如下：" << endl;
	////算法，第一个第二个参数是容器的范围，第三个是函数
	//for_each(vStu.begin(), vStu.end(), printStudent);
	for (vector<Student>::iterator it = vStu.begin();it != vStu.end();it++)   //迭代器的办法
	{
		cout << "学号为：" << (*it).m_Id
			<< "  姓名为：" << (*it).m_Name
			<< "  密码为：" << (*it).m_Pwd << endl;

	}

	system("pause");
	system("cls");
}



//添加图书
void Manager::addBook()
{
	string fileName; //操作文件名
	ofstream ofs;//文件操作对象

	//添加的是书本
	fileName = BOOK_FILE;

	//利用追加的方式 写文件
	ofs.open(fileName, ios::out | ios::app);

	int id; //书本编号
	string name; //书籍名字
	int state;  //书本状态

	cout << "请输入书本编号：" << endl;

	while (true)
	{
		cin >> id;
		bool ret = checkRepeat1(id);
		if (ret) //有重复
		{
			cout << "书本编号重复，请重新输入编号: " << endl;
		}
		else
		{
			break;
		}
	}

	cout << "请输入书籍名字： " << endl;
	cin >> name;

	state = 0;

	//向文件中添加数据
	ofs << endl << id << " " 
		        << name << " " 
		        <<state <<" " << endl;
	cout << "添加成功!" << endl;

	system("pause");
	system("cls");

	ofs.close();

	//调用初始化容器接口，从新获取文件中的数据，更新数据
	this->initVector1();

}

//删除图书
void Manager::delectBook()
{
	int id;
	cout << "请输入所要删除的书本编号:  " << endl;
	cin >> id;


	//迭代器就相当于指针 it
	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)
	{
		if ((*it).m_BookId == id)
		{
			//将此处位置删除 it 
			vBook.erase(it);
			cout << "删除成功！" << endl;
			break;
		}
		else  //未实现
		{
			cout << "该书本不存在! " << endl;
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
			         << (*it).m_BookName  << " " << endl;
	}

	ofs1.close();

	//调用初始化容器接口，从新获取文件中的数据，更新数据
	this->initVector1();
	system("pause");
	system("cls");
}

//修改图书
void Manager::modBook()
{
	string tip;  //提示书本的状态
	int n = 0;
	cout << "请输入要修改的书本编号： " << endl;
	cin >> n;

	//迭代器就相当于指针 it
	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)
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

		if ((*it).m_BookId == n)
		{
			cout << "该书籍原本信息如下：" << endl;
			cout << "书本编号为：" << (*it).m_BookId << " " 
				 << "书本名字为：" << (*it).m_BookName << " "  
				 <<"书本状态为： "<<tip <<" " << endl;
		}
	}

	cout << "请输入该书本修改后的编号： " << endl;
	int id;
	cin >> id;
	vBook[n - 1].m_BookId = id;

	cout << "请输入该书本修改后的书名：" << endl;
	string name;
	cin >> name;
	vBook[n - 1].m_BookName = name;

	cout << "请输入该书本修改后的状态：" << endl;
	cout << "输入1：已借出状态 " << endl;
	cout << "输入0：未借出状态 " << endl;
	int a;  // 1   0
	cin >> a;
	vBook[n - 1].m_BookState = a;

	//以上只是在vector中修改了数据，还要将数据同步到文件txt中

	ofstream ofs(BOOK_FILE, ios::trunc);  //如果文件存在，那么删干净，再重新创建
	//现在里面没东西，我们要加入已修改完后的容器内容进新的文件中
	ofs.close();


	ofstream ofs1;//文件操作对象
	//利用追加的方式 写文件
	ofs1.open(BOOK_FILE, ios::out | ios::app);

	for (vector<BookData>::iterator it = vBook.begin();it != vBook.end();it++)
	{
		//将容器内容写入文件中
		ofs1 << endl << (*it).m_BookId 
			<< " "   << (*it).m_BookName 
			<< " " <<(*it).m_BookState<<" " << endl;
	} 


	ofs1.close();

	//调用初始化容器接口，从新获取文件中的数据，更新数据
	this->initVector1();

	cout << "修改成功！" << endl;
	system("pause");
	system("cls");
}

//查找图书
void Manager::findBook()
{
	string tip;
	int id = 0;
	cout << "请输入你要查找的书本编号: " << endl;
	cin >> id;

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

		if (id == (*it).m_BookId)  //查找到了该书本的id
		{
			cout << "找到了" << (*it).m_BookId << "号书本，该书，其信息如下：" << endl;
			cout << "  书本编号为：" << (*it).m_BookId
				<< "  书本名字为: " << (*it).m_BookName
				<<"  书本状态: "<<tip << endl;
		}
	}

	system("pause");
	system("cls");
}

//显示图书
void Manager::showBook()
{
	string tip;  //提示书本的状态

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
			<<"  书本状态: "<<tip << endl;

	}

	system("pause");
	system("cls");
}
