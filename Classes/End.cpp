#include "End.h"
#include"HelloWorldScene.h"
#include<string>
#include "SimpleAudioEngine.h"
#include"MazeScene.h"
#include"Message.h"
#include"config.h"
#include"gameset.h"
#include"AnimationScene.h"
#include"StartGUI.h"

using namespace std;


/*
游戏的进入画面
（1）开始游戏
（2）游戏设置
（3）背景音乐快捷按钮
（4）游戏版本信息
*/


// GBK转UTF-8
std::string GBK2UTF84(const std::string& gbk) {
	std::string utf_8 = "";

	int n = MultiByteToWideChar(CP_ACP, 0, gbk.c_str(), -1, NULL, 0);
	WCHAR* str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, gbk.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char* str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	utf_8 = str2;
	delete[]str1;
	str1 = nullptr;
	delete[]str2;
	str2 = nullptr;
	return utf_8;
}




//StartGUI场景创建实现
Scene* End::createScene()
{
	auto scene = Scene::create();
	auto layer = End::create();
	scene->addChild(layer);
	return scene;
}

bool End::init()
{
	if (!Layer::init())
	{
		return false;
	}

	if (musicoff == 0) {


		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("end.mp3", true); //播放背景音乐

	}




	Size VisibleSize = Director::getInstance()->getVisibleSize();

	// 开始界面演示动漫
	Vector<SpriteFrame* >frameVector;
	for (int i = 2; i <= 344; i += 2)
	{
		//这里的i对应的是切割出来的视频帧号
		auto spriteFrame = SpriteFrame::create("endvideo/" + to_string(i) + ".jpg", Rect(0, 0, 1424, 768));
		frameVector.pushBack(spriteFrame);
	}
	auto animation = Animation::createWithSpriteFrames(frameVector);
	animation->setDelayPerUnit(0.07f);
	auto animate = Animate::create(animation);

	// 添加画面
	auto pic = Sprite::create();
	//pic->setTag(PLAYER_TAG);
	pic->runAction(RepeatForever::create(animate));
	this->addChild(pic, 1);
	pic->setPosition(Vec2(700, 350));




	//开始按钮
	auto label = Label::createWithSystemFont(GBK2UTF84("返回游戏开始!"), "fonts/HGWT_CNKI.TTF", 40);

	//label->setPosition(0, -150);

	//按下确认，跳进主界面
	auto menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(End::StartGame, this));


	menuitem->setPosition(0, -150);


	menuitem->setTag(10001);


	auto menu = Menu::create(menuitem, NULL);
	menu->alignItemsVertically();


	//音乐设置按钮
	auto label3 = Sprite::create("BOSSSHOW.png");

	label3->setPosition(500, 400);

	//label3->setScale(0.9);

	//this->addChild(label3, 12);

	//音乐设置按钮

	auto label4 = MenuItemImage::create(
		"mess.png",
		"mess.png",
		CC_CALLBACK_1(End::Message, this));

	//auto label4 = Label::createWithSystemFont(GBK2UTF82("游戏信息"), "fonts/HGWT_CNKI.TTF", 40);

	//按下确认，跳进主界面
	auto menuitem4 = MenuItemLabel::create(label4, CC_CALLBACK_1(End::Message, this));
	menuitem4->setPosition(Vec2(400, 300));

	//auto m2 = Menu::create(menuitem4, NULL);
	//this->addChild(m2, 12);


	auto label5 = MenuItemImage::create(
		"setbutton.png",
		"setbutton.png",
		CC_CALLBACK_1(End::Message, this));

	label5->setPosition(Vec2(600, 100));
	//this->addChild(label5, 120);


	/*
	Vector<SpriteFrame* >frameVector;
	for (int i = 2; i <= 400; i += 2)
	{
		//这里的i对应的是切割出来的视频帧号
		auto spriteFrame = SpriteFrame::create("video/" + to_string(i) + ".jpg", Rect(0, 0, 1024, 768));
		frameVector.pushBack(spriteFrame);
	}
	auto animation = Animation::createWithSpriteFrames(frameVector);
	animation->setDelayPerUnit(0.07f);
	auto animate = Animate::create(animation);

	// 添加画面
	auto pic = Sprite::create();
	//pic->setTag(PLAYER_TAG);
	pic->runAction(RepeatForever::create(animate));
	this->addChild(pic, 1);
	pic->setPosition(Vec2(1024 / 2, 768 / 2));
	*/





	//this->addChild(menu, 100);



	return true;
}




void  End::StartGame2(Ref* pSender) {
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

//进入游戏
void End::StartGame(Ref* pSender)
{
	
	Director::getInstance()->replaceScene(StartGUI::createScene());
		








}

//进入游戏设置
void End::Gameset(Ref* pSender)
{




	Director::getInstance()->replaceScene(Gameset::createScene());
	//Director::getInstance()->replaceScene(AnimationScene::createScene());
}


//进入音乐设置
void End::MusicSet(Ref* pSender)
{






	//Director::getInstance()->replaceScene(HelloWorld::createScene());
}

//进入版权信息
void End::Message(Ref* pSender)
{

	Director::getInstance()->replaceScene(Message::createScene());
}