#ifndef CONFIG_H
#define CONFIG_H

/********** 游戏配置数据 **********/
//用宏定义
//高度宽度 与图片大小有关
//素材中每个地图图片一样尺寸
#define GAME_WIDTH 512 //地图宽度
#define GAME_HEIGHT 768 //地图高度
#define GAME_TITLE "飞机大战 v1.0" //标题
#define GAME_RES_PATH "./plane.rcc"   //rcc文件路径
#define GAME_TUBIAO ":/res/app.ico"  //文件图标加载路径


/********** 地图配置数据 **********/
#define GAME_MAP ":/res/img_bg_level_1.jpg" //地图加载路径
//#define GAME_MAP2 ":/res/img_bg_level_2.jpg"  //地图2
//#define GAME_MAP3 ":/res/img_bg_level_3.jpg"  //地图3
//#define GAME_MAP4 ":/res/img_bg_level_4.jpg"  //地图4
//#define GAME_MAP5 ":/res/img_bg_level_5.jpg"  //地图5

#define GAME_MAP_SPEED 2  //地图滚动速度  单位 像素
#define GAME_TIMER_RATE 14   //刷新间隔  频率 单位：毫秒


/********** 飞机配置数据 **********/
#define GAME_HEROPLANE ":/res/hero2.png"


/********** 玩家子弹配置数据 **********/
#define BULLET_PATH ":/res/bullet_11.png" //子弹图片路径
#define BULLET_SPEED 5 //子弹移动速度
#define BULLET_NUM 30 //弹匣中子弹总数
#define BULLET_INTERVAL 20 //发射子弹时间间隔


/********** 敌机配置数据 **********/
#define ENEMY_PATH ":/res/img-plane_5.png" //敌机资源图片，地图1中的
#define ENEMY_SPEED 5 //敌机移动速度
#define ENEMY_NUM 20 //敌机总数量
#define ENEMY_INTERVAL 30 //敌机出场时间间隔


/**********  爆炸配置数据 **********/
#define BOMB_PATH  ":/res/bomb-%1.png"   //爆炸资源图片
#define BOMB_NUM  20     //爆炸数量
#define BOMB_MAX  7      //爆炸图片最大索引
#define BOMB_INTERVAL 20   //爆炸切图时间间隔

/**********  音效配置数据 **********/
#define SOUND_BACKGROUND ":/res/bg.wav"
#define SOUND_BOMB ":/res/bomb.wav"





#endif // CONFIG_H
