#include "weatherdata.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include<QDateTime>

//api数据
//"date": "20230719",
//    "time": "2023-07-19 18:59:31",
//    "cityInfo": {
//        "city": "天津市",
//        "citykey": "101030100",
//        "parent": "天津",
//        "updateTime": "14:46"
//    },
//    "data": {
//        "shidu": "25%",
//        "pm25": 33,
//        "pm10": 49,
//        "quality": "轻度",
//        "wendu": "34",
//        "ganmao": "儿童、老年人及心脏、呼吸系统疾病患者人群应减少长时间或高强度户外锻炼",
//        "forecast": [
//            {
//                "date": "19",
//                "high": "高温 36℃",
//                "low": "低温 26℃",
//                "ymd": "2023-07-19",
//                "week": "星期三",
//                "sunrise": "05:00",
//                "sunset": "19:34",
//                "aqi": 118,
//                "fx": "南风",
//                "fl": "3级",
//                "type": "晴",
//                "notice": "愿你拥有比阳光明媚的心情"
//            },


Today::Today()
{
    //初始化
    date = "0000-00-00";
    city = "null";
    fl = u8"无数据";
    fx = u8"无数据";
    ganmao = u8"无数据";
    notice = u8"无数据";
    pm25 = u8"无数据";
    quality = u8"无数据";
    shidu = u8"无数据";
    sunrise = "00:00";
    sunset = "00:00";
    wendu = "null";
    type = u8"无数据";
}

//用Today=jsonobj
Today &Today::operator=(const QJsonObject &jsObj)
{
    //用json数据填满类的属性1.取日期
    QString dateStr = jsObj.value("date").toString();
    date= QDate::fromString(dateStr,"yyyyMMdd").toString("yyyy-MM-dd");
    //2.取城市
    city=jsObj.value("cityInfo").toObject().value("city").toString();

    //解析data--又是一个QJsonObject
    QJsonObject dataObj =jsObj.value("data").toObject();
    shidu=dataObj.value("shidu").toString();//取湿度
    pm25=QString::number(dataObj.value("pm25").toDouble());//取pm2.5
    quality=dataObj.value("quality").toString();//取空气质量
    wendu=dataObj.value("wendu").toString()+ u8"°";//取温度
    ganmao=dataObj.value("ganmao").toString();//取感冒提示
    //取预测数据（数组）
    QJsonArray forecastArr= dataObj.value("forecast").toArray();
    QJsonObject todayObj = forecastArr.at(0).toObject();//预测15天，.at(0)说明今日
    fx=todayObj.value("fx").toString();
    fl=todayObj.value("fl").toString();
    type=todayObj.value("type").toString();//天气情况
    sunrise=todayObj.value("sunrise").toString();
    sunset=todayObj.value("sunset").toString();
    notice=todayObj.value("notice").toString();//提示语

    return *this;
}

Forecast::Forecast()
{
    //初始化预报数据
    aqi="0"; //质量
    date=u8"00日星期0";
    week=u8"星期0";
    high=u8"高温 0.0°C";
    low=u8"低温 0.0°C";
    type = "undefined"; ///天气

}

//等号运算符重载，封装
Forecast &Forecast::operator=(const QJsonObject &obj)
{
    date = obj.value("date").toString();
    week=obj.value("week").toString();
    high=obj.value("high").toString();
    low=obj.value("low").toString();
    aqi=QString::number(obj.value("aqi").toDouble());
    type=obj.value("type").toString();
    return *this;
}
