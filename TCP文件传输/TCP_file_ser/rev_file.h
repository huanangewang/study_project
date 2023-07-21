#ifndef REV_FILE_H
#define REV_FILE_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QFile>

class Rev_file : public QThread
{
    Q_OBJECT
public:
    explicit Rev_file(QTcpSocket* socket,QObject *parent = nullptr);
    //重写基类继承的run()方法
    void run() override;

signals:
    //读完文件的信号
    void overfile();

private:
    QTcpSocket *m_socket;
};

#endif // REV_FILE_H
