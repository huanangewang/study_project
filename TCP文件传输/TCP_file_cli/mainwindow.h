#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_server_clicked();
    void on_checkfile_clicked();

    void on_sendfile_clicked();

signals:
    //自定义信号
    void start_connect(QString ip,unsigned short port);
    void send_file(QString path);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
