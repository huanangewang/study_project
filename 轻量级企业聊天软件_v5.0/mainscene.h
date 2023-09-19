#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include "config.h"
#include <QIcon>
#include <QTimer> //定时器
#include "map.h"
#include<QPainter>  //画家类
#include "heroplane.h"
#include <QMouseEvent>  //鼠标事件
#include "bullet.h"
#include"enemyplane.h"
#include <ctime>
#include"bomb.h"



class MainScene : public QWidget
{
    Q_OBJECT

 //初始化游戏场景
void initScene();

public:
    MainScene(QWidget *parent = 0);

    //地图对象
    Map m_map;   //地图1
    //创建一个定时器对象
    QTimer m_timer;
    //创建英雄飞机对象
	HeroPlane m_hero;
//    //测试子弹代码
//    Bullet temp_bullet;


    //启动游戏 用于启动定时器对象
    void playGame();
    //更新所有游戏中的坐标
    void updatePosition();
    //绘图事件  绘制到屏幕中  函数名不可改
    void paintEvent(QPaintEvent *event);
    //重写  鼠标移动事件   函数名不可改
    void mouseMoveEvent(QMouseEvent *event);


    //敌机出场
    void enemyToScene();
    //敌机数组
    EnemyPlane m_enemys[ENEMY_NUM];
    //敌机出场间隔记录
    int m_recorder;

    //碰撞检测函数
    void collisionDetection();

    //爆炸数组
    Bomb m_bombs[BOMB_NUM];


    ~MainScene();
};

#endif // MAINSCENE_H
