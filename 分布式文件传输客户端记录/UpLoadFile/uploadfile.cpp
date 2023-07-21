#include "uploadfile.h"
#include "ui_uploadfile.h"


//QHttpPart是用来组织数据块的，，QHttpMultiPart是用来拼接数据块的(会自带分界线--append) ;
//QHttpMultiPart其实就是多个QHttpPart
UpLoadFile::UpLoadFile(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UpLoadFile)
{
    ui->setupUi(this);
//    //初始化进度条
//    ui->progress->setRange(0,100);

    //登录成功-->设置当前用户名字
   Login *ins= Login::getInstance(); //拿到了单例对象
   ins->setUserName("xmb");


}

UpLoadFile::~UpLoadFile()
{
    delete ui;
}

//计算Md5
QString UpLoadFile::get_Md5(QString path)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    //1.添加数据
    QFile file(path);
    //读权限给予
    file.open(QFile::ReadOnly);
    //用第三种方法 addData(QIODevice *device)
    hash.addData(&file);
    //2.数据运算--->结果  toHex()是为了转成16进制字符串
    QByteArray data =  hash.result().toHex();
    //3.返回结果
    return data;
}


//选择文件对话框
void UpLoadFile::on_setfile_clicked()
{
    //对话框，只要是从QDialog派生（子窗口）就用它的静态方法
    QString path= QFileDialog::getOpenFileName(this,"打开文件","D::\\");
    if(path.isEmpty())
    {
        QMessageBox::warning(this,"警告","文件路径为空！");
        return;
    }
    //将路径显示到编辑框中
    ui->filepath->setText(path);

}

//上传文件
void UpLoadFile::on_uploadfile_clicked()
{
    //1. manager request对象
    QNetworkAccessManager *  manager= new QNetworkAccessManager(this);
    QNetworkRequest request;
    //2.发送数据  --post
    //设置request-->1.Url   ---->2.header
    QString url="http://192.168.121.128:80/uploadFile";
    request.setUrl(QUrl(url));
    //带分界线的头的格式：multipart/form-data
    request.setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data");


    //post数据块multipart数据头组织
    //part :1.组织的是两个分界线中间的数据块
    QHttpPart part;
    QFileInfo info(ui->filepath->text());  //用于获取文件各种信息
    //组织disp拼接  \"是给百分号%转义
    QString disp=QString("form-data; user=\"%1\"; filename=\"%2\" md5=\"%3\";size=%4")
            .arg(Login::getInstance()->getUserName()).arg(info.fileName()).arg(get_Md5(ui->filepath->text())).arg(info.size());
    //取后缀--->为了取key值,找到value值：即type
    QString sufix=info.suffix();
    part.setHeader(QNetworkRequest::ContentDispositionHeader,disp);
    //传输的数据块的格式--根据文件后缀定的
    /* json配置文件
     * {
        "mp3":"audio/mp3",
        "avi":"video/avi",
        "html":"text/html"
        "json":"application/json"

     }*/
    //服务器是我写的
    part.setHeader(QNetworkRequest::ContentTypeHeader,"type");
    QFile *file= new QFile(ui->filepath->text());
    file->open(QFile::ReadOnly);
    part.setBodyDevice(file);

    //因为上面part 的 value 是form-data
    QHttpMultiPart *multiPart=new QHttpMultiPart(QHttpMultiPart::FormDataType,this);
    // 调用该函数会自动添加分界线 -> 使用频率高的函数
    multiPart->append(part);

    ui->record->setText("开始向服务器发送数据.....");
    //把服务器返回的数据接收 -->QHttpMultiPart *multiPart
    QNetworkReply *reply= manager->post(request , multiPart /*QHttpMultiPart *multiPart*/);

    //3.接收数据
    connect(reply,&QNetworkReply::finished,this,[=](){
        //把服务器返回的数据读出
        QByteArray all= reply->readAll();
    //4.格式解析  --->纯文本
    //5.判断是否上传成功
        ui->record->append("服务器响应数据：" + all);

        //释放内存  -- >qt中可以用一些函数更安全
        multiPart->deleteLater();
        file->close();
        file->deleteLater();
        reply->deleteLater();
    });


    //进度条处理
    connect(reply,&QNetworkReply::uploadProgress,this,[=](qint64 bytesSent, qint64 bytesTotal)
    {
       qDebug()<<"当前进度: "<<bytesSent << "总数："<<bytesTotal;
       if(bytesSent > 0)
       ui->progress->setValue(((bytesSent*100)/bytesTotal)); //int类型，x100不能放外面
    });

}
