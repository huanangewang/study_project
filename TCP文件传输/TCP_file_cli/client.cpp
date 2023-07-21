#include "client.h"



Client::Client(QObject *parent) : QObject(parent)
{

}

//连接服务器
void Client::connect_server(QString ip, unsigned short port)
{
    //需要Socker套接字
    m_tcp=new QTcpSocket;
    m_tcp->connectToHost(QHostAddress(ip),port);
    //检测一下什么时候连接成功了
    connect(m_tcp,&QTcpSocket::connected,this,&Client::connectOK);
    //什么时候断开连接
    connect(m_tcp,&QTcpSocket::disconnected,this,[=]()
    {
        //释放内存
        m_tcp->close();
        m_tcp->deleteLater();
        emit gameover();
    });
}

//发送文件
void Client::send_file(QString path)
{
    //为了让服务器知道什么时候收完，要给他发一下文件的大小
    QFileInfo info(path);
    int filesize = info.size();
    QFile file(path);
    file.open(QFile::ReadOnly);
    while(!file.atEnd())
    {
        //如果是第一次循环，就发送文件大小
        static int num=0;
        if(num==0)
        {
            //为什么一共就四个字节？
            m_tcp->write((char*) &filesize,4);
        }
        //读文件
       QByteArray data =  file.readLine();
       num+=data.size();
       int precent=(num*100)/filesize; //计算百分比
       emit curPrecent(precent); //发送百分比给主线程
       m_tcp->write(data);//发文件
    }


}
