#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>
#include <QFileInfo>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    //连接服务器的工作函数
    void connect_server(QString ip, unsigned short port );
    //发送文件的工作函数
    void send_file(QString path);

signals:
    void connectOK();
    void gameover();
    void curPrecent(int num);//当前百分比
private:
    QTcpSocket *m_tcp;

};

#endif // CLIENT_H
