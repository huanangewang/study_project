#include "login.h"

//声明
Login Login:: m_login;

//静态成员方法
Login *Login::getInstance()
{
    return &m_login;//把这个单例对象的地址给到函数的调用者
}

void Login::setUserName(QString name)
{
    m_user=name;
}

QString Login::getUserName()
{
    return m_user;
}

Login::Login()
{

}

Login::Login(const Login &t)
{

}
