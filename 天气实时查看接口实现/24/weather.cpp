#include "weather.h"
#include "ui_weather.h"
#include "weatherdata.h"

#include <QPainter>
#include <QDateTime>
#include <QMessageBox>
#include<QDebug>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#define SPAN_INDEX 4   // 温度曲线间隔指数
#define ORIGIN_SIZE 3 // 温度曲线原点大小
#define TEMPERATURE_STARTING_COORDINATE 45 // 高温平均值起始坐标

// 日出日落底线
const QPoint Weather::sun[2] = {
    QPoint(20, 75),//起点
    QPoint(130, 75)//终点
};

// 日出日落时间
const QRect Weather::sunRizeSet[2] = {
    QRect(0, 80, 50, 20),//x,y,宽，高
    QRect(100, 80, 50, 20)
};

// 日出日落圆弧
const QRect Weather::rect[2] = {
    QRect(25, 25, 100, 100), // 虚线圆弧
    QRect(50, 80, 50, 20) // “日出日落”文本
};

Weather::Weather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Weather)
{
    ui->setupUi(this);
    //设置窗口属性为没用边框的窗口
    this->setWindowFlag(Qt::FramelessWindowHint);
    //设置输入框
    ui->cityLineEdit->setStyleSheet("QLineEdit{border: 1px solid gray; "
                                    "border-radius: 4px; background:argb(47, 47, 47, 130); "
                                    "color:rgb(255, 255, 255);} "
                                    "QLineEdit:hover{border-color:rgb(101, 255, 106); }");

    //UI初始化
    forecast_week_list << ui->week0Lb << ui->week1Lb << ui->week2Lb << ui->week3Lb << ui->week4Lb << ui->week5Lb;
    forecast_date_list << ui->date0Lb << ui->date1Lb << ui->date2Lb << ui->date3Lb << ui->date4Lb << ui->date5Lb;
    forecast_aqi_list << ui->quality0Lb << ui->quality1Lb << ui->quality2Lb << ui->quality3Lb << ui->quality4Lb << ui->quality5Lb;
    forecast_type_list << ui->type0Lb << ui->type1Lb << ui->type2Lb << ui->type3Lb << ui->type4Lb << ui->type5Lb;
    forecast_typeIco_list << ui->typeIco0Lb << ui->typeIco1Lb << ui->typeIco2Lb << ui->typeIco3Lb << ui->typeIco4Lb << ui->typeIco5Lb;
    forecast_high_list << ui->high0Lb << ui->high1Lb << ui->high2Lb << ui->high3Lb << ui->high4Lb << ui->high5Lb;
    forecast_low_list << ui->low0Lb << ui->low1Lb << ui->low2Lb << ui->low3Lb << ui->low4Lb << ui->low5Lb;

    // 右键菜单
    m_pMenu = new QMenu(this);
    m_pExitAct = new QAction;
    m_pExitAct->setText( u8"退出" );
    m_pExitAct->setIcon(QIcon(":/weatherIco/close.ico")); //设置退出图标
    m_pMenu->addAction(m_pExitAct); //关联
    connect( m_pExitAct, SIGNAL(triggered(bool)),
             this, SLOT(slot_exitApp()) ); //这样写需要实现槽函数

    // dateLb和WeekLb样式表设置
    for (int i = 0; i < 6; i++)
    {
        forecast_date_list[i]->setStyleSheet("background-color: rgba(0, 255, 255, 100);");
        forecast_week_list[i]->setStyleSheet("background-color: rgba(0, 255, 255, 100);");
    }

    // 请求天气API信息
    url = "http://t.weather.itboy.net/api/weather/city/";
    city = u8"株洲";
    cityTmp = city;
    manager = new QNetworkAccessManager(this); //
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replayFinished(QNetworkReply*)));
    getWeatherInfo(manager);

    /* 事件过滤 */
    ui->sunRiseSetLb->installEventFilter(this); // 启用事件过滤器
    ui->curveLb->installEventFilter(this);
    ui->cityLineEdit->installEventFilter(this);

    sunTimer = new QTimer(ui->sunRiseSetLb);
    connect(sunTimer, SIGNAL(timeout()), ui->sunRiseSetLb, SLOT(update()));
    sunTimer->start(1000);//一秒更新一次

    //    setLabelContent();
}

Weather::~Weather()
{
    delete ui;
}

/* 请求数据 */
void Weather::getWeatherInfo(QNetworkAccessManager *manager)
{
    QString citycode = tool[city];
    qDebug()<<citycode;
    if(citycode=="000000000"){
        QMessageBox::warning(this, u8"错误", u8"天气：指定城市不存在！", QMessageBox::Ok);
        return;
    }
    QUrl jsonUrl(url + citycode);
    manager->get( QNetworkRequest(jsonUrl) );
}

//自定义信号槽,用于接收服务器响应数据
void Weather::replayFinished(QNetworkReply *reply)
{
    /* 获取响应的信息，状态码为200表示正常 */
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if(reply->error() != QNetworkReply::NoError || status_code != 200)
    {
        QMessageBox::warning(this, u8"错误", u8"天气：请求数据错误，检查网络连接！", QMessageBox::Ok);
        return;
    }

    QByteArray bytes = reply->readAll();
//    QString result = QString::fromLocal8Bit(bytes);
    qDebug()<<bytes;
    parseJson(bytes);//解析json
}

/* 解析Json数据 */
void Weather::parseJson(QByteArray bytes)
{
    QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes, &err); // 检测json格式
    if (err.error != QJsonParseError::NoError) // Json格式错误
    {
        return;
    }

    QJsonObject jsObj = jsonDoc.object();
    //测试
//    qDebug()<<jsObj;
//    qDebug()<<"==================\r\n";
//    qDebug()<<jsObj.value("message").toString();
    QString message = jsObj.value("message").toString();
    if (message.contains("success")==false)
    {
        QMessageBox::information(this, tr("The information of Json_desc"),
                                 u8"天气：城市错误！", QMessageBox::Ok );
        city = cityTmp;//备份城市
        return;
    }
    //在weatherdata中重载过了
    today=jsObj;

    // 解析data中的yesterday
    QJsonObject dataObj = jsObj.value("data").toObject();//先取大块数据---再找里面的数据，比如yesterday
    forecast[0] = dataObj.value("yesterday").toObject();//[0]代表昨天数据

    // 解析data中的forecast
    QJsonArray forecastArr = dataObj.value("forecast").toArray();
    int j = 0;//再找五天,预报数据今天开始
    for (int i = 1; i < 6; i++)
    {
        //从1开始     ，取的时候从0开始
        forecast[i]=forecastArr.at(j).toObject();
        j++;
    }

    // 取得今日数据
    QJsonObject todayObj = forecastArr.at(0).toObject();
    today.fx = todayObj.value("fx").toString();
    today.fl = todayObj.value("fl").toString();
    today.type = todayObj.value("type").toString();
    today.sunrise = todayObj.value("sunrise").toString();
    today.sunset = todayObj.value("sunset").toString();
    today.notice = todayObj.value("notice").toString();

    setLabelContent();
}

//UI更新
void Weather::setLabelContent()
{
    /////////////////今日数据/////////////////
    ui->dateLb->setText(today.date);
    ui->temLb->setText(today.wendu);
    ui->cityLb->setText(today.city);
    ui->typeLb->setText(today.type);
    ui->noticeLb->setText(today.notice+u8"。  --作者：xmb");//提示更新
    ui->shiduLb->setText(today.shidu);
    ui->pm25Lb->setText(today.pm25);
    ui->fxLb->setText(today.fx);
    ui->flLb->setText(today.fl);
    ui->ganmaoBrowser->setText(today.ganmao);

    // 判断白天还是夜晚图标
//    QString sunsetTime = today.date + " " + today.sunset;
    if (QDateTime::currentDateTime() <= QDateTime::fromString(today.sunset, "yyyy-MM-dd hh:mm"))
    {
        //还没日落---白天
         ui->typeIcoLb->setStyleSheet( tr("border-image: url(:/day/day/晴.png); background-color: argb(60, 60, 60, 0);"));
//        ui->typeIcoLb->setStyleSheet( tr("border-image: url(:/day/day/%1.png); background-color: argb(60, 60, 60, 0);").arg(today.type) );
    }
    else
    {
        ui->typeIcoLb->setStyleSheet( tr("border-image: url(:/night/night/晴.png); background-color: argb(60, 60, 60, 0);"));
    }

    //空气质量设置
    ui->qualityLb->setText(today.quality == "" ? "无" : today.quality);
    if (today.quality == "优")
    {
        ui->qualityLb->setStyleSheet("color: rgb(0, 255, 0); background-color: argb(255, 255, 255, 0);");
    }
    else if (today.quality == "良")
    {
        ui->qualityLb->setStyleSheet("color: rgb(255, 255, 0); background-color: argb(255, 255, 255, 0);");
    }
    else if (today.quality == "轻度污染")
    {
        ui->qualityLb->setStyleSheet("color: rgb(255, 170, 0); background-color: argb(255, 255, 255, 0);");
    }
    else if (today.quality == "重度污染")
    {
        ui->qualityLb->setStyleSheet("color: rgb(255, 0, 0); background-color: argb(255, 255, 255, 0);");
    }
    else if (today.quality == "严重污染")
    {
        ui->qualityLb->setStyleSheet("color: rgb(170, 0, 0); background-color: argb(255, 255, 255, 0);");
    }
    else
    {
        ui->qualityLb->setStyleSheet("color: rgb(255, 255, 255); background-color: argb(255, 255, 255, 0);");
    }

    /////////////////更新六天数据--实际上从昨天开始////////////////
    for (int i = 0; i < 6; i++)
    {
        //可以通过下标的方式访问，拿到QLabel的指针,也就是QLabel *
        forecast_week_list[i]->setText(forecast[i].week.right(3));
        forecast_date_list[i]->setText(forecast[i].date.left(3));
        forecast_type_list[i]->setText(forecast[i].type);
        forecast_high_list[i]->setText(forecast[i].high.split(" ").at(1));
        forecast_low_list[i]->setText(forecast[i].low.split(" ").at(1));
        forecast_typeIco_list[i]->setStyleSheet
                ( tr("image: url(:/day/day/%1.png);")
                  .arg(forecast[i].type) );//设置图标
        //设置空气质量
        if (forecast[i].aqi.toInt() >= 0 && forecast[i].aqi.toInt() <= 50)
        {
            forecast_aqi_list[i]->setText(u8"优质");
            forecast_aqi_list[i]->setStyleSheet("color: rgb(0, 255, 0);");
        }
        else if (forecast[i].aqi.toInt() > 50 && forecast[i].aqi.toInt() <= 100)
        {
            forecast_aqi_list[i]->setText(u8"良好");
            forecast_aqi_list[i]->setStyleSheet("color: rgb(255, 255, 0);");
        }
        else if (forecast[i].aqi.toInt() > 100 && forecast[i].aqi.toInt() <= 150)
        {
            forecast_aqi_list[i]->setText(u8"轻度污染");
            forecast_aqi_list[i]->setStyleSheet("color: rgb(255, 170, 0);");
        }
        else if (forecast[i].aqi.toInt() > 150 && forecast[i].aqi.toInt() <= 200)
        {
            forecast_aqi_list[i]->setText(u8"重度污染");
            forecast_aqi_list[i]->setStyleSheet("color: rgb(255, 0, 0);");
        }
        else
        {
            forecast_aqi_list[i]->setText(u8"严重污染");
            forecast_aqi_list[i]->setStyleSheet("color: rgb(170, 0, 0);");
        }
    }//for
    //设置昨天和今天的星期数,细节处理一下，可观性高
    ui->week0Lb->setText( u8"昨天" );
    ui->week1Lb->setText( u8"今天" );
    //温度曲线绘制  (日出日落已经用定时器管理了)
    ui->curveLb->update();
}

/* 搜索按钮 */
void Weather::on_searchBt_clicked()
{
    cityTmp = city;
    city = ui->cityLineEdit->text();
    getWeatherInfo(manager);
}

/* 刷新按钮 */
void Weather::on_refreshBt_clicked()
{
    getWeatherInfo(manager);
    ui->curveLb->update();
}

/* 事件触发 */
bool Weather::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->sunRiseSetLb && event->type() == QEvent::Paint)
    {
        paintSunRiseSet();//绘制
    }
    else if (watched == ui->curveLb && event->type() == QEvent::Paint)
    {
        paintCurve();
    }
    //否则调默认处理,丢给父类
    return QWidget::eventFilter(watched,event);
}

/* 日出日落图形绘制 */
void Weather::paintSunRiseSet()
{
    QPainter painter(ui->sunRiseSetLb);
    painter.setRenderHint(QPainter::Antialiasing, true); // 反锯齿

    // 绘制日出日落线(底线)
    painter.save();//保存当前绘制器状态 restore end 成对使用
    QPen pen = painter.pen();//画笔
    pen.setWidthF(0.5);//宽度0.5
    pen.setColor(Qt::yellow);//颜色黄
    painter.setPen(pen);//把笔给画家
    painter.drawLine(sun[0], sun[1]);//画直线
    painter.restore();//重置

    //绘制文字
    painter.save();
    painter.setFont( QFont("Microsoft Yahei", 8, QFont::Normal) ); // 字体、大小、正常粗细
    painter.setPen(Qt::white);

    if (today.sunrise != "" && today.sunset != "")
    {
        //在指定区域绘制文字
        painter.drawText(sunRizeSet[0], Qt::AlignHCenter, today.sunrise);
        painter.drawText(sunRizeSet[1], Qt::AlignHCenter, today.sunset);
    }
    painter.drawText(rect[1], Qt::AlignHCenter, u8"日出日落");//在指定区域绘制文字
    painter.restore();

    // 绘制圆弧
    painter.save();
    pen.setWidthF(0.5);
    pen.setStyle(Qt::DotLine); // 虚线
    pen.setColor(Qt::green);
    painter.setPen(pen);
    painter.drawArc(rect[0], 0 * 16, 180 * 16);//Qt中角度是现实生活中度数的1/16倍
    painter.restore();

    // 绘制日出日落占比（刷子）
    if (today.sunrise != "" && today.sunset != "")
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 85, 0, 100));//刷子的颜色，第四个参数是透明度

        int startAngle, spanAngle;
        QString sunsetTime = today.date + " " + today.sunset;

        if (QDateTime::currentDateTime() > QDateTime::fromString(sunsetTime, "yyyy-MM-dd hh:mm"))
        {
            //已经日落了
            startAngle = 0 * 16;
            spanAngle = 180 * 16;
        }
        else
        {
            // 计算起始角度和跨越角度
            static QStringList sunSetTime = today.sunset.split(":");
            static QStringList sunRiseTime = today.sunrise.split(":");

            static QString sunsetHour = sunSetTime.at(0);
            static QString sunsetMint = sunSetTime.at(1);
            static QString sunriseHour = sunRiseTime.at(0);
            static QString sunriseMint = sunRiseTime.at(1);

            static int sunrise = sunriseHour.toInt() * 60 + sunriseMint.toInt();
            static int sunset = sunsetHour.toInt() * 60 + sunsetMint.toInt();
            int now = QTime::currentTime().hour() * 60 + QTime::currentTime().minute();

            //算占比
            startAngle = ( (double)(sunset - now) / (sunset - sunrise) ) * 180 * 16;
            spanAngle = ( (double)(now - sunrise) / (sunset - sunrise) ) * 180 * 16;
        }

        if (startAngle >= 0 && spanAngle >= 0)
        {
            painter.drawPie(rect[0], startAngle, spanAngle); // 扇形绘制
        }
    }
}

/* 温度曲线绘制 */
void Weather::paintCurve()
{
    QPainter painter(ui->curveLb);
    painter.setRenderHint(QPainter::Antialiasing, true); // 反锯齿

    // 将温度转换为int类型，并计算平均值，平均值作为curveLb曲线的参考值，参考Y坐标为45
    int tempTotal = 0;
    int high[6] = {};
    int low[6] = {};

    QString h, l;
    for (int i = 0; i < 6; i++)
    {
        //提前知道了数据的格式，我们这里掐头去尾，取出对应的温度
        h = forecast[i].high.split(" ").at(1);
        h = h.left(h.length() - 1);
        high[i] = (int)(h.toDouble());//在json中只能先转double，再转int
        tempTotal += high[i];

        l = forecast[i].low.split(" ").at(1);
        l = l.left(h.length() - 1);
        low[i] = (int)(l.toDouble());
    }
    int tempAverage = (int)(tempTotal / 6); // 最高温平均值

    // 算出温度对应坐标
    int pointX[6] = {35, 103, 172, 241, 310, 379}; // 点的X坐标
    int pointHY[6] = {0};
    int pointLY[6] = {0};
    for (int i = 0; i < 6; i++)
    {
        pointHY[i] = TEMPERATURE_STARTING_COORDINATE - ((high[i] - tempAverage) * SPAN_INDEX);
        pointLY[i] = TEMPERATURE_STARTING_COORDINATE + ((tempAverage - low[i]) * SPAN_INDEX);
    }

    QPen pen = painter.pen();
    pen.setWidth(1);

    // 高温曲线绘制
    painter.save();
    //昨天到今天
    pen.setColor(QColor(255, 170, 0));
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.setBrush(QColor(255, 170, 0));//橙色
    painter.drawEllipse(QPoint(pointX[0], pointHY[0]), ORIGIN_SIZE, ORIGIN_SIZE);//圆
    painter.drawEllipse(QPoint(pointX[1], pointHY[1]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawLine(pointX[0], pointHY[0], pointX[1], pointHY[1]);//两点之间绘制线
    //今天到未来四天的
    pen.setStyle(Qt::SolidLine);//实线
    pen.setWidth(1);
    painter.setPen(pen);

    for (int i = 1; i < 5; i++)
    {
        painter.drawEllipse(QPoint(pointX[i+1], pointHY[i+1]), ORIGIN_SIZE, ORIGIN_SIZE);//绘制点
        painter.drawLine(pointX[i], pointHY[i], pointX[i+1], pointHY[i+1]);//两点之间绘制线
    }
    painter.restore();

//    // 低温曲线绘制
//    pen.setColor(QColor(0, 255, 255));
//    pen.setStyle(Qt::DotLine);
//    painter.setPen(pen);
//    painter.setBrush(QColor(0, 255, 255));//蓝色
//    painter.drawEllipse(QPoint(pointX[0], pointLY[0]), ORIGIN_SIZE, ORIGIN_SIZE);
//    painter.drawEllipse(QPoint(pointX[1], pointLY[1]), ORIGIN_SIZE, ORIGIN_SIZE);
//    painter.drawLine(pointX[0], pointLY[0], pointX[1], pointLY[1]);

//    pen.setColor(QColor(0, 255, 255));
//    pen.setStyle(Qt::SolidLine);
//    painter.setPen(pen);
//    for (int i = 1; i < 5; i++)
//    {
//        painter.drawEllipse(QPoint(pointX[i+1], pointLY[i+1]), ORIGIN_SIZE, ORIGIN_SIZE);
//        painter.drawLine(pointX[i], pointLY[i], pointX[i+1], pointLY[i+1]);
//    }

    // 低温曲线绘制 修正
    painter.save();
    //昨天到今天
    pen.setColor(QColor(0, 255, 255));
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.setBrush(QColor(0, 255, 255));//蓝色
    painter.drawEllipse(QPoint(pointX[0], pointLY[0]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawEllipse(QPoint(pointX[1], pointLY[1]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawLine(pointX[0], pointLY[0], pointX[1], pointLY[1]);//两点之间绘制线
    //今天到未来四天的
    pen.setStyle(Qt::SolidLine);//实线
    pen.setWidth(1);
    painter.setPen(pen);

    for (int i = 1; i < 5; i++)
    {
        painter.drawEllipse(QPoint(pointX[i+1], pointLY[i+1]), ORIGIN_SIZE, ORIGIN_SIZE);//绘制点
        painter.drawLine(pointX[i], pointLY[i], pointX[i+1], pointLY[i+1]);//两点之间绘制线
    }
    painter.restore();
}

///* 调用软键盘 */
//void Weather::callKeyBoard()
//{
//    //    KeyBoard *keyBoard = new KeyBoard(0, ui->cityLineEdit);
//    //    keyBoard->show();
//}

/* 窗口移动 */
void Weather::mousePressEvent(QMouseEvent *event)
{
    mPos = event->globalPos() - this->pos(); //屏幕 - 应用程序
}

void Weather::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - mPos);
}

/* 右键菜单 */
void Weather::contextMenuEvent(QContextMenuEvent *menuEvent)
{
    m_pMenu->exec(QCursor::pos());//获取位置
    menuEvent->accept();//表示该事件已经处理完毕
}

//退出
void Weather::slot_exitApp()
{
    qApp->exit(0);
}
