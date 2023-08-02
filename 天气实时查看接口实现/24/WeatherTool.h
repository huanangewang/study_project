#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <map>
#include <QFile>
#include <QCoreApplication>

///打开文件，读json数据，读到QByteArray,----放在jsondoc里面
/// jsondoc再---转成jsonarray数组，这样就有了城市数据
///


class WeatherTool
{
public:
    WeatherTool(){
        QString fileName = QCoreApplication::applicationDirPath();
        QJsonParseError err;
        fileName+="/citycode-2019-08-23.json";
        QFile file(fileName);
        bool ret = file.open(QIODevice::ReadOnly|QIODevice::Text);
        QByteArray json = file.readAll();
        file.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(json,&err);
        QJsonArray citys = jsonDoc.array();//这里面是放了一个数组,数组里面有json键值对
        for(int i=0;i<citys.size();i++)
        {
            //拿到后转成obj
            QString city = citys.at(i).toObject().value("city_name").toString();
            QString code = citys.at(i).toObject().value("city_code").toString();
            if(code.size()>0){
                m_mapCity2Code.insert(std::pair<QString,QString>(city,code));
            }
        }
    }
    //重载---写入城市名字，能得到code(邮政编码)
    //重载操作符["city"]。根据城市名称得城市代码
    QString operator[](const QString& city){
        //用迭代器，通过城市名查找邮政编码
        std::map<QString,QString>::iterator it = m_mapCity2Code.find(city);
        if(it==m_mapCity2Code.end()){//没找到--先搜城市后戴市字
            it = m_mapCity2Code.find(city+u8"市");
        }
        if(it==m_mapCity2Code.end())//再搜城市后戴县字
        {
            it=m_mapCity2Code.find(city+u8"县");
        }
        if(it!=m_mapCity2Code.end()){
            return it->second;//返回城市代码
        }
        return "000000000";
    }
private:
    //用来搜索城市用的 记录邮政编码(城市，邮政编码)
    std::map<QString,QString> m_mapCity2Code;
};

#endif // WEATHERTOOL_H


