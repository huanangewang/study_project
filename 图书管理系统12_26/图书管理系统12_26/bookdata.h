#pragma once 
#include <iostream>
using namespace std;

//图书房类
class BookData
{
public:

	int m_BookId; //书本编号

	string m_BookName; //书本名字

	int m_BookState;    //书本状态   ,一开始都是未借出状态

};