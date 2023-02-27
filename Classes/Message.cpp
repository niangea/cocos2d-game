#include "Message.h"
#include"HelloWorldScene.h"
#include"StartGUI.h"

//StartGUI场景创建实现
Scene* Message::createScene()
{
	auto scene = Scene::create();	//创建一个场景
	auto layer = Message::create();//创建一个StartGUI层
	scene->addChild(layer);		//把StartGUI层加入刚刚创建的场景中，
	return scene;			//返回这个场景
}

bool Message::init()
{
	if (!Layer::init())	//先初始化父类的init方法，如何初始化失败，则创建StartGUI层失败
	{
		return false;
	}
	Size VisibleSize = Director::getInstance()->getVisibleSize();	//获得屏幕大小

	auto cloud_1 = Sprite::create("messgui.png");
	cloud_1->setPosition(Vec2(1024 / 2, 768 / 2));

	this->addChild(cloud_1);
	//菜单条目的创建有多种，之前的HelloWorld.cpp中实现的是图片菜单条目(MenuItemImage)，即根据图片来创建菜单条目
	//这里是根据标签来创建菜单条目,然后设置回调函数
	auto menuitem = MenuItemImage::create(
		"goback.png",
		"goback.png",
		CC_CALLBACK_1(Message::EnterSecondScene, this));
	//创建好了菜单条目，就需要加入菜单中，所以下面就是创建菜单
	auto menu = Menu::create(menuitem, NULL);
	menu->setPosition(Vec2(50, 700));
	//把菜单添加到StartGUI层中
	this->addChild(menu,100);

	return true;
}
//菜单回调函数的实现
void Message::EnterSecondScene(Ref* pSender)
{
	//我们知道，如果需要跳转场景，就会想到一个类，它就是Director类，它就是管理场景的
	//这里跳转场景调用到的是导演类的这个接口：replaceScene(Scene *scene),里面传进去的是就是一个场景，这里需要注意的是场景二的头文件这是要加上的
	Director::getInstance()->replaceScene(StartGUI::createScene());
}