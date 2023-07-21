#ifndef UPLOADFILE_H
#define UPLOADFILE_H

#include <QMainWindow>
#include <QFileDialog> //选择文件
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QUrl>
#include <QHttpPart>
#include <QFileInfo>
#include <QCryptographicHash> //Md5
#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UpLoadFile; }
QT_END_NAMESPACE

class UpLoadFile : public QMainWindow
{
    Q_OBJECT

public:
    UpLoadFile(QWidget *parent = nullptr);
    ~UpLoadFile();
    // 获取Md5
    QString get_Md5(QString path);

private slots:
    void on_setfile_clicked();

    void on_uploadfile_clicked();

private:
    Ui::UpLoadFile *ui;
};
#endif // UPLOADFILE_H
