#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QObject>
#include <QWidget>

class Today //当日天气预报的数据
{
public:
    Today();
    //重载=号
    Today & operator=(const QJsonObject & jsObj);
    QString date;
    QString wendu;
    QString city;
    QString shidu;
    QString pm25;
    QString quality;
    QString ganmao;//感冒指数
    QString fx;//风向
    QString fl;//风力
    QString type;//天气
    QString sunrise;
    QString sunset;
    QString notice;//注意信息
};

class Forecast//预报数据
{
public:
    Forecast();
    //重载=号操作符
    Forecast & operator=(const QJsonObject & obj);
    QString date;
    QString week;
    QString high;
    QString low;
    QString aqi;//质量
    QString type;//天气
};

#endif // WEATHERDATA_H
