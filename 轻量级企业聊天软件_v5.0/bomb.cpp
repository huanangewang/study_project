#include "bomb.h"

Bomb::Bomb()
{
     //初始化爆炸图片数组
     for(int i = 1 ;i <= BOMB_MAX ;i++)
     {
         //字符串拼接，类似  ":/res/bomb-1.png"
         QString str = QString(BOMB_PATH).arg(i);
         m_pixArr.push_back(QPixmap(str));
     }
    //爆炸坐标
    m_X=0;
    m_Y=0;
    //爆炸状态
    m_Free=true;
    //爆炸时间间隔
    m_Recoder=0;
    //爆炸图片的下标
    m_index=0;
}



//更新信息（播放图片下标、播放间隔）
void Bomb::updateInfo()
{
    //空闲状态
    if(m_Free)
    {
        return;
    }
    //非空闲
    m_Recoder++;
    if(m_Recoder<BOMB_INTERVAL)
    {
        //记录爆炸间隔未到，直接return，不需要切图
        return;
    }
    //记录爆炸间隔到了
    m_Recoder=0;
    //切图
    m_index++;
    //判断是否切完
    //数组是从0开始的 ，也就是 这里是0-6  （7张图）
    if(m_index>BOMB_MAX-1)
    {
        m_index=0;
        m_Free=true;
    }

}

