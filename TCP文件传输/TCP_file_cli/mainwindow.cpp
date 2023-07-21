#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include "client.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //控件的初始化
    ui->ip->setText("127.0.0.1");
    ui->port->setText("8989");
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);

    //创建线程对象
    QThread *t1=new QThread();
    //c创建任务类对象
    Client * worker= new Client();

    //让子线程知道我们的文件地址  ,两个send_file,的参数一样的
    connect(this,&MainWindow::send_file,worker,&Client::send_file);

    //检测点击连接服务器
    connect(this,&MainWindow::start_connect,worker,&Client::connect_server);

    //处理子线程发来的信号
    connect(worker,&Client::connectOK,this,[=]()
    {
        QMessageBox::information(this,"服务器","服务器连接成功");
    });
    //接收子线程发来的百分比
    connect(worker,&Client::curPrecent,ui->progressBar,&QProgressBar::setValue);

    connect(worker,&Client::gameover,this,[=](){
        //断开，要释放内存
        t1->quit();
        t1->wait();
        worker->deleteLater();
    });
    t1->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//点击连接服务器
void MainWindow::on_connect_server_clicked()
{
    QString ip= ui->ip->text();
    unsigned short port=ui->port->text().toUShort();//转成短无符号整型
    emit start_connect(ip, port);

}

//点击选择文件
void MainWindow::on_checkfile_clicked()
{
    QString path=QFileDialog::getOpenFileName();//拿到文件的路径
    if(path.isEmpty())
    {
       return;
    }
    ui->checkfileline->setText(path);

}

//点击发送文件
void MainWindow::on_sendfile_clicked()
{
    //获取文件路径
    emit send_file(ui->checkfileline->text());
}
