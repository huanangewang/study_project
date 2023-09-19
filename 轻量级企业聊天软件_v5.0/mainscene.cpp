#include "mainscene.h"
#include <QSound>

MainScene::MainScene(QWidget *parent)
    : QWidget(parent)
{
    //初始化游戏场景
     this->initScene();
    //启动游戏
    this->playGame();

}

//敌机出场
void MainScene::enemyToScene()
{
    m_recorder++;
    if(m_recorder<ENEMY_INTERVAL)
    {
        //未到间隔 return , 不出敌机
        return;
    }
    else
    {
        //到达间隔了，m_recorder置0
        m_recorder=0;
        for(int i=0;i<ENEMY_NUM;i++)
        {
            //出敌机
            //判断敌机状态是否空闲
            if(m_enemys[i].m_Free==true)
            {
                //改成工作状态
                m_enemys[i].m_Free=false;
                //设置出现坐标
                m_enemys[i].m_X=rand()%(GAME_WIDTH - m_enemys[i].m_Rect.width());
                m_enemys[i].m_Y=-m_enemys[i].m_Rect.height();
                break;
            }
        }
    }
}

//初始化游戏场景
void MainScene::initScene()
{
   //初始化窗口大小 要固定住
   setFixedSize(GAME_WIDTH,GAME_HEIGHT);
   //设置窗口标题
   setWindowTitle(GAME_TITLE);
   //加载图标
   setWindowIcon(QIcon(GAME_TUBIAO));
   //定时器设置
   this->m_timer.setInterval(GAME_TIMER_RATE);  //定时器的间隔

   //初始化间隔记录属性
   m_recorder=0;
   //随机数种子
   srand((unsigned int)time(NULL));
}


//启动游戏
void MainScene::playGame()
{

    //启动背景音乐
    QSound::play(SOUND_BACKGROUND);

    //启动定时器
    m_timer.start();
    //监听定时器发送的信号
    connect(&m_timer,&QTimer::timeout,this,[=](){
        //敌机出场
        this->enemyToScene();
        //更新游戏中的坐标
        this->updatePosition();
        //重新绘制图片
        update();  //系统自带的函数 //再调用paintEvent函数
        //碰撞检测
        this->collisionDetection();
    });
}

//更新游戏中所有元素的坐标
void MainScene::updatePosition()
{
    //1.更新地图坐标
    m_map.mapPosition();

    //2.1发射子弹
    m_hero.shoot();
    //2.2计算子弹坐标
    for(int i=0;i<BULLET_NUM;i++)
    {
        //如果子弹是工作状态 就计算坐标
        if(m_hero.m_bullets[i].m_Free==false)
        {
            //是工作状态  更新子弹矩形框位置
            m_hero.m_bullets->updatePosition();
        }
    }
                        //    //测试子弹代码
                        //    temp_bullet.m_Free = false;
                        //    temp_bullet.updatePosition();
    //3.敌机坐标计算
    for(int i=0;i<ENEMY_NUM;i++)
    {
        //非空闲敌机 更新坐标
        if(m_enemys[i].m_Free==false)
        {
            m_enemys[i].updatePosition();
        }
    }

    //4.计算爆炸播放的图片
    for(int i = 0 ; i < BOMB_NUM;i++)
    {
        if(m_bombs[i].m_Free == false)
        {
            m_bombs[i].updateInfo();
        }
    }



}

//绘图事件 绘制到屏幕中
void MainScene::paintEvent(QPaintEvent *event)
{
    //创建一个画家类 对象  要画在窗口中
    QPainter painter(this);

    //1.绘制地图
    painter.drawPixmap(0,m_map.m_map1_posY,m_map.m_map1);  //参数1 x坐标  参数2 y坐标  参数3 显示什么内容
    painter.drawPixmap(0,m_map.m_map2_posY,m_map.m_map2);
    //2.绘制英雄飞机
    painter.drawPixmap(m_hero.m_X,m_hero.m_Y,m_hero.m_Plane);

                            //    //测试子弹
                            //    painter.drawPixmap(temp_bullet.m_X,temp_bullet.m_Y,temp_bullet.m_Bullet);
    //3.绘制子弹
    for(int i=0;i<BULLET_NUM;i++)
    {
        //如果子弹是工作状态 ,绘图
        if(m_hero.m_bullets[i].m_Free==false)
        {   //参数1 x坐标  参数2 y坐标  参数3 显示什么内容(图片的路径)
            painter.drawPixmap(m_hero.m_bullets->m_X,m_hero.m_bullets->m_Y,m_hero.m_bullets[i].m_Bullet);
        }
    }

    //4.绘制敌机
    for(int i=0;i<ENEMY_NUM;i++)
    {
        //非空闲敌机 更新坐标
        if(m_enemys[i].m_Free==false)
        {
           painter.drawPixmap(m_enemys[i].m_X,m_enemys[i].m_Y,m_enemys[i].m_enemy);
        }
    }

    //5.绘制爆炸效果
    //绘制爆炸图片
    for(int i = 0 ; i < BOMB_NUM;i++)
    {
    if(m_bombs[i].m_Free == false)
    {
    painter.drawPixmap(m_bombs[i].m_X,m_bombs[i].m_Y,m_bombs[i].m_pixArr[m_bombs[i].m_index]);
    }
    }

}

//鼠标移动事件  //也就是鼠标操控飞机位置的更新
void MainScene::mouseMoveEvent(QMouseEvent *event)
{
     //要让鼠标在飞机中间，还需要处理下x ,y   //不懂为什么是减法
     //记录鼠标位置
    int x = event->x()- (m_hero.m_Rect.width()  *0.5);
    int y = event->y()- (m_hero.m_Rect.height() *0.5);

    //设置 边界不能越过
    //边界检测
    if(x <= 0 )                                 //左边界
    {
         x = 0;
    }
    if(x >= GAME_WIDTH - m_hero.m_Rect.width())  //右边界
    {
         x = GAME_WIDTH - m_hero.m_Rect.width();
    }
    if(y <= 0)                                   //上边界
    {
         y = 0;
    }
    if(y >= GAME_HEIGHT - m_hero.m_Rect.height())  //下边界
    {
         y = GAME_HEIGHT - m_hero.m_Rect.height();
    }
    //设置飞机位置
    m_hero.setPosition(x,y);

}

//碰撞检测函数
void MainScene::collisionDetection()
{
    //遍历所有非空闲敌机
    for(int i=0;i<ENEMY_NUM;i++)
    {
        //如果是空闲飞机 ，那么直接执行下一次循环
        if(m_enemys[i].m_Free==true)
        {
            continue;
        }

        //如果遇到工作状态（非空闲）的敌机，那么检测所有工作状态（非空闲）的子弹
        for(int j=0;j<BULLET_NUM;j++)
        {
            //如果是空闲子弹，那么直接执行下一次循环
            if(m_hero.m_bullets[j].m_Free==true)
            {
                continue;
            }
            //如果遇到工作状态（非空闲）的子弹
            //进行碰撞检测
            if(m_enemys[i].m_Rect.intersects(m_hero.m_bullets[j].m_Rect))
            {
                //如果两个矩形框相交，会返回true
                //将敌机和子弹的状态都变为空闲，就实现了消失
                m_enemys[i].m_Free=true;
                m_hero.m_bullets[j].m_Free=true;

                //播放爆炸效果
                for(int k = 0 ; k < BOMB_NUM;k++)
                {
                    if(m_bombs[k].m_Free)
                    {
                        //爆炸状态设置为非空闲
                        m_bombs[k].m_Free = false;
                        //更新坐标
                        m_bombs[k].m_X = m_enemys[i].m_X;
                        m_bombs[k].m_Y = m_enemys[i].m_Y;
                        //播放音效
                        QSound::play(SOUND_BOMB);
                        break;
                    }
                }
            }
        }
    }
}


MainScene::~MainScene()
{

}
