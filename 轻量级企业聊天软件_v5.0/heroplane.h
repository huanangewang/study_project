#ifndef HEROPLANE_H
#define HEROPLANE_H

#include <QPixmap>
#include <QRect>
#include "config.h"
#include "bullet.h"

class HeroPlane
{
public:
    HeroPlane();
    //发射子弹
    void shoot();
    //设置飞机位置
    void setPosition(int x, int y);

public:
/////////////*********玩家飞机*********//////////////
    //飞机资源 对象
    QPixmap m_Plane;
    //飞机坐标
    int m_X;
    int m_Y;
    //飞机的矩形边框
    QRect m_Rect;

/////////////*********子弹*********//////////////
    //弹夹  //设置成数组 ，容量为 BULLET_NUM （30）
    Bullet m_bullets[BULLET_NUM];
    //发射间隔记录
    int m_recorder;

};

#endif // HEROPLANE_H
