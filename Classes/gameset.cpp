#include "gameset.h"
#include"HelloWorldScene.h"
#include"StartGUI.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

#include"cocos-ext.h"
USING_NS_CC_EXT;

USING_NS_CC;



//StartGUI场景创建实现
Scene* Gameset::createScene()
{
	auto scene = Scene::create();	//创建一个场景
	auto layer = Gameset::create();//创建一个StartGUI层
	scene->addChild(layer);		//把StartGUI层加入刚刚创建的场景中，
	return scene;			//返回这个场景
}

bool Gameset::init()
{
	if (!Layer::init())	//先初始化父类的init方法，如何初始化失败，则创建StartGUI层失败
	{
		return false;
	}
	Size VisibleSize = Director::getInstance()->getVisibleSize();	//获得屏幕大小

	//音量设置
	
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("4.mp3", true); //开始界面音乐
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
	
	auto slide_control = ControlSlider::create("ba1.png", "ba2.png", "bb1.png");

	slide_control->setPosition(Vec2(550, 500));

	slide_control->setMinimumValue(0);
	slide_control->setMaximumValue(100);
	slide_control->setValue(0);
	slide_control->setTag(20);
	slide_control->addTargetWithActionForControlEvents(this, cccontrol_selector(Gameset::valueChanged), Control::EventType::VALUE_CHANGED);
	this->addChild(slide_control,1001);
		


	auto cloud_1 = Sprite::create("set.png");
	cloud_1->setPosition(Vec2(1024 / 2, 768 / 2));

	this->addChild(cloud_1);
	//菜单条目的创建有多种，之前的HelloWorld.cpp中实现的是图片菜单条目(MenuItemImage)，即根据图片来创建菜单条目
	//这里是根据标签来创建菜单条目,然后设置回调函数
	auto menuitem = MenuItemImage::create(
		"goback.png",
		"goback.png",
		CC_CALLBACK_1(Gameset::EnterSecondScene, this));
	//创建好了菜单条目，就需要加入菜单中，所以下面就是创建菜单
	auto menu = Menu::create(menuitem, NULL);
	menu->setPosition(Vec2(50, 700));
	//把菜单添加到StartGUI层中
	this->addChild(menu, 100);

	

	return true;
}





//菜单回调函数的实现
void Gameset::EnterSecondScene(Ref* pSender)
{
	//我们知道，如果需要跳转场景，就会想到一个类，它就是Director类，它就是管理场景的
	//这里跳转场景调用到的是导演类的这个接口：replaceScene(Scene *scene),里面传进去的是就是一个场景，这里需要注意的是场景二的头文件这是要加上的
	Director::getInstance()->replaceScene(StartGUI::createScene());
}


void Gameset::valueChanged(Ref* pSender, Control::EventType controlEvent) {
	double value = dynamic_cast<ControlSlider*>(pSender)->getValue();
	
	musicpos = value/(double)100;

	double v = CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();

	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(musicpos);


	
}

