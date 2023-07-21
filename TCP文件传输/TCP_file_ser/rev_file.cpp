#include "rev_file.h"

Rev_file::Rev_file(QTcpSocket * socket,QObject *parent) : QThread(parent)
{
    m_socket=socket;
}


void Rev_file::run()
{
    QFile * file=new QFile("revfile.txt");
    file->open(QFile::WriteOnly);

    //接收数据
    connect(m_socket,&QTcpSocket::readyRead,this,[=](){
        static int count =0;
        static int filesize =0;
        //第一次读文件，只需要读4个字节,读文件大小
        if(count==0)
        {
            m_socket->read((char* ) &filesize,4);
        }
        //读出剩余数据
        QByteArray data=  m_socket->readAll();
        //每次读一部分文件，就对大小做一次记录
        count+=data.size();
        file->write(data);
        //判断数据是否收完
        if(count==filesize)
        {
            //关闭套接字和文件，回收资源
            m_socket->close();
            m_socket->deleteLater();
            file->close();
            file->deleteLater();
            emit overfile();
        }
    });

    //子线程需要一直检测是否有事件，不能退出的
    //进入事件循环
    exec();
}
