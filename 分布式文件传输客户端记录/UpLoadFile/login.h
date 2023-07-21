#ifndef LOGIN_H
#define LOGIN_H

#include <QString>
//饿汉模式单例
class Login
{
public:
//    login();

    //静态成员方法
    static Login *getInstance();

    //方法
    void setUserName(QString name);
    QString getUserName();

private:
    Login(); //构造私有化
    Login(const Login &t);  //拷贝构造私有化
    static Login m_login;  //单例对象创建

    // 存储的数据
    QString m_user;
    QString m_passwd;
    QString m_ip;
    QString m_port;
    QString m_token;

};

#endif // LOGIN_H
