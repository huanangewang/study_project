#include "heroplane.h"

HeroPlane::HeroPlane()
{
    //初始化 加载飞机图片的资源
    m_Plane.load(GAME_HEROPLANE);
    //初始化飞机 开始的坐标
    m_X= (GAME_WIDTH - m_Plane.width()) *0.5;  //让飞机从中间开始 出现
    m_Y=  GAME_HEIGHT-m_Plane.height();

    //初始化矩形框大小 用于检测 物体碰撞
    m_Rect.setWidth(m_Plane.width());
    m_Rect.setHeight(m_Plane.height());
    //将矩形框放对应位置
    m_Rect.moveTo(m_X,m_Y);

    //初始化发射间隔
    m_recorder=0;

}



//设置飞机位置
void HeroPlane::setPosition(int x, int y)
{
    //变更矩形框位置
    m_X=x;
    m_Y=y;
    m_Rect.moveTo(m_X,m_Y);
}

//发射子弹
void HeroPlane::shoot()
{
    //累加时间间隔变量  加到  recorder 到达 发射子弹时间间隔 才会发射子弹
    m_recorder++;
    //判断如果记录数字 未达到发射间隔，直接return
    if(m_recorder<BULLET_INTERVAL)
    {
        return;
    }
    else
    {
        //到达发射间隔
        //重置发射间隔时间
        m_recorder=0;
        //开始发射子弹
        //遍历弹夹中的子弹
        for(int i=0;i<BULLET_NUM;i++)
        {
            //如果是空闲的，就发射
            if(m_bullets[i].m_Free==true)
            {
                //改成工作模式
                m_bullets[i].m_Free=false;
                //设置发射子弹的位置坐标  ,也就是就是飞机的坐标
                //这里优化处理了 坐标
                m_bullets[i].m_X=m_X + m_Rect.width()*0.5  - 10  ;;
                m_bullets[i].m_Y=m_Y - 25;
                break;

            }
        }
    }

}
