#pragma once
#include "cocos2d.h"					//包含头文件

using namespace cocos2d;				//使用cocos2d命名空间

class Message : public Layer		//新建一个类，继承层（Layer）
{
public:
    static Scene* createScene();		//Message场景创建实现

    virtual bool init();				//Message层的初始化

    CREATE_FUNC(Message);			//创建Message的Create方法

    void EnterSecondScene(Ref* pSender);	//创建一个菜单回调函数，这里实现了场景的跳转
};