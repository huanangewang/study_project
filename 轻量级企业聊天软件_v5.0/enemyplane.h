#ifndef ENEMYPLANE_H
#define ENEMYPLANE_H
#include <QPixmap>
#include <QRect>
#include"config.h"


class EnemyPlane
{
public:
    EnemyPlane();
public:

    //敌机位置更新坐标
    void updatePosition();
    public:
    //敌机资源对象
    QPixmap m_enemy;
    //位置
    int m_X;
    int m_Y;
    //敌机的矩形边框（碰撞检测）
    QRect m_Rect;
    //状态
    bool m_Free;
    //速度
    int m_Speed;

};

#endif // ENEMYPLANE_H
