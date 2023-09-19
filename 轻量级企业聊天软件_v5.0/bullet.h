#ifndef BULLET_H
#define BULLET_H
#include "config.h"
#include <QPixmap>
//7.3 子弹类成员函数实现
//在config.h中追加子弹配置信息
//在bullet.cpp中实现成员函数，代码如下：


class Bullet
{
public:
    Bullet();
    //更新子弹坐标
    void updatePosition();

public:
    //子弹资源对象
    QPixmap m_Bullet;
    //子弹的矩形边框（用于碰撞检测）
    QRect m_Rect;

    //子弹坐标
    int m_X;
    int m_Y;
    //子弹移动速度
    int m_Speed;
    //子弹是否闲置  //子弹的状态
    bool m_Free;

};


#endif // BULLET_H



