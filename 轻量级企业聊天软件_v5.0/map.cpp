#include "map.h"
#include "config.h"

Map::Map()
{
    //初始化 加载地图的 对象
    m_map1.load(GAME_MAP);
    m_map2.load(GAME_MAP);

    //初始化Y轴坐标
    //第一张图在-GAME_HEIGHT处 ，第二张图在0处 （Qt中的坐标轴注意 是Y朝下 X朝右 原点在左上角）
    m_map1_posY=-GAME_HEIGHT;
    m_map2_posY=0;

    //设置地图滚动速度
    m_scroll_speed=GAME_MAP_SPEED;
}

//地图滚动坐标计算  //一直滚动要计算坐标
void Map::mapPosition()
{
    //第一张图片坐标处理
    m_map1_posY += m_scroll_speed;
    //直到图片到达尽头（此尽头是指 0 ）
    if(m_map1_posY>=0)
    {
      //到达尽头后，要重置，也就是回到原来初始位置
        m_map1_posY=-GAME_HEIGHT;
    }
    //第二张图片处理
    m_map2_posY += m_scroll_speed;
    //直到图片到达尽头 （此尽头是指 GAME_HEIGHT）
    if(m_map2_posY>=GAME_HEIGHT)
    {
        //到达尽头后要重置 ，也就是回到原来初始位置
        m_map2_posY=0;
    }
}
