#include "bullet.h"

Bullet::Bullet()
{
    //加载子弹图片资源
    m_Bullet.load(BULLET_PATH);
    //子弹坐标 初始坐标可随意设置，后期会重置
    m_X = GAME_WIDTH*0.5 - m_Bullet.width()*0.5;  //先从中间发射
    m_Y = GAME_HEIGHT;
    //子弹状态  默认空闲
    this->m_Free=true;
    //子弹速度
    this->m_Speed=BULLET_SPEED;

    //子弹矩形框
    m_Rect.setWidth(m_Bullet.width());
    m_Rect.setHeight(m_Bullet.height());
    m_Rect.moveTo(m_X,m_Y);



}

//更新子弹坐标
void Bullet::updatePosition()
{
    //如果子弹是空闲状态 不需要计算坐标
    //玩家飞机可以控制子弹的状态为false  ,也就是非空闲状态
    if(m_Free==true)//空闲
    {
        return;
    }
    else
    {
        //非空闲
        //子弹要向上移动
        m_Y-=m_Speed;
        //更新矩形框位置
        m_Rect.moveTo(m_X,m_Y);
        //子弹出界面外后 要重置为空闲状态
        if(m_Y<=-m_Rect.height())
        {
            m_Free=true;
        }

    }


}
