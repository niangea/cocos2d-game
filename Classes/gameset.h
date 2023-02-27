#pragma once
#include "cocos2d.h"					//包含头文件
#include"config.h"

#include"cocos-ext.h"
USING_NS_CC_EXT;

USING_NS_CC;

using namespace cocos2d;				//使用cocos2d命名空间

class Gameset : public Layer		//新建一个类，继承层（Layer）
{
public:
	static Scene* createScene();		

	virtual bool init();				

	CREATE_FUNC(Gameset);	

	
	
	void valueChanged(Ref* pSender, Control::EventType controlEvent);

	void EnterSecondScene(Ref* pSender);	//创建一个菜单回调函数，这里实现了场景的跳转
};