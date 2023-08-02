#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>
#include <QList>
#include <QLabel>
#include <QPoint>
#include <QRect>
#include <QTimer>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>

//加载本地数据
#include "WeatherTool.h"
#include "weatherdata.h"

namespace Ui {
class Weather;
}


class Weather : public QWidget
{
    Q_OBJECT

public:
    explicit Weather(QWidget *parent = 0);
    ~Weather();
    //重写虚函数
    virtual bool eventFilter(QObject *watched, QEvent *event);//1.触发事件的对象 2.触发什么事件
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *menuEvent); //菜单相关事件
    void getWeatherInfo(QNetworkAccessManager *manager); /* 请求数据 */
private slots:
    void replayFinished(QNetworkReply *reply);//用于接收服务器响应数据
    void on_searchBt_clicked();//搜索
    void on_refreshBt_clicked();//刷新
    void slot_exitApp();//退出
private:
    Ui::Weather *ui;

    QPoint mPos; // 窗口移动

    //UI,设置list方便添加元素,当成数组处理
    QList<QLabel *> forecast_week_list;
    QList<QLabel *> forecast_date_list;
    QList<QLabel *> forecast_aqi_list;
    QList<QLabel *> forecast_type_list;
    QList<QLabel *> forecast_typeIco_list;
    QList<QLabel *> forecast_high_list;
    QList<QLabel *> forecast_low_list;

    static const QPoint sun[2]; // 日出日落底线
    static const QRect sunRizeSet[2];// 日出日落时间
    static const QRect rect[2];// 日出日落圆弧

    //定时更新绘画
    QTimer *sunTimer;

    //网络请求
    QNetworkAccessManager *manager;
    QString url; //接口链接
    QString city; //访问城市
    QString cityTmp; //临时存放的城市变量，防止输入错误城市，原来城市名称还在
    WeatherTool tool;   //天气工具
    //本地文件
    Today today;
    Forecast forecast[6];
    /* 右键菜单退出程序 */
    QMenu *m_pMenu;
    QAction *m_pExitAct;

    void parseJson(QByteArray bytes);/* 解析Json数据 */
    void setLabelContent();//UI更新
    void paintSunRiseSet();/* 日出日落图形绘制 */
    void paintCurve();/* 温度曲线绘制 */
//    void callKeyBoard();
};

#endif // WEATHER_H
