#include "enemyplane.h"

EnemyPlane::EnemyPlane()
{
    //敌机图片资源加载
    m_enemy.load(ENEMY_PATH);
    //敌机位置
    m_X=0;
    m_Y=0;
    //敌机状态
    this->m_Free=true;
    //敌机速度
    this->m_Speed=ENEMY_SPEED;
    //敌机矩形框
    m_Rect.setWidth(m_enemy.width());
    m_Rect.setHeight(m_enemy.height());

}

//敌机位置更新
void EnemyPlane::updatePosition()
{
    //空闲状态 不计算坐标 return
    if(this->m_Free==true)
    {
        return;
    }
    else
    {
        //沿着Y轴走
        m_Y+=this->m_Speed;
        //矩形框位置也要随着更新
        m_Rect.moveTo(m_X,m_Y);
        //飞机飞出去后，要更新状态
        if(m_Y>=GAME_HEIGHT)
        {
            this->m_Free=true;
        }
    }
}
