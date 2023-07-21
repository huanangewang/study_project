#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_server=new QTcpServer(this);//对指针进行实例化

    //检测是否有客户端连接
    connect(m_server,&QTcpServer::newConnection,this,[=](){
        //可以返回套接字
        QTcpSocket * socket=m_server->nextPendingConnection();
        //创建子线程
        Rev_file *subthread=new Rev_file(socket);
        subthread->start();//run方法自动被调用
        //处理子线程发来的信号
        connect(subthread,&Rev_file::overfile,this,[=](){
            //子线程已经写完文件了，可以释放资源了
            subthread->quit();
            subthread->wait();
            subthread->deleteLater();
            socket->deleteLater();
            QMessageBox::information(this,"通知","文件发送完毕！");
        });

    });

    //用传参的方式传递socket到子线程去
}

MainWindow::~MainWindow()
{
    delete ui;
}


//点击启动监听-
void MainWindow::on_setlisten_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    m_server->listen(QHostAddress::Any,port);
}
