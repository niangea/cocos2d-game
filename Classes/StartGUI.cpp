#include "StartGUI.h"
#include"HelloWorldScene.h"
#include<string>
#include "SimpleAudioEngine.h"
#include"MazeScene.h"
#include"Message.h"
#include"config.h"
#include"gameset.h"
#include"AnimationScene.h"
#include"End.h"

using namespace std;


/*
游戏的进入画面
（1）开始游戏
（2）游戏设置
（3）背景音乐快捷按钮
（4）游戏版本信息
*/


// GBK转UTF-8
std::string GBK2UTF82(const std::string& gbk) {
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


void StartGUI::loadvideo() {
	
}

//StartGUI场景创建实现
Scene* StartGUI::createScene()
{
	auto scene = Scene::create();	
	auto layer = StartGUI::create();
	scene->addChild(layer);		
	return scene;			
}

bool StartGUI::init()
{
	if (!Layer::init())
	{
		return false;
	}

	if (musicoff == 0) {
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("4.mp3", true); //开始界面音乐
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(musicpos);
	}
	
	Size VisibleSize = Director::getInstance()->getVisibleSize();
	// 开始界面演示动漫
	Vector<SpriteFrame* >frameVector;
	for (int i = 2; i <= 344; i += 2)
	{
		//这里的i对应的是切割出来的视频帧号
		auto spriteFrame = SpriteFrame::create("start/" + to_string(i) + ".jpg", Rect(0, 0, 1424, 768));
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
	auto label = Label::createWithSystemFont(GBK2UTF82("开始游戏!"), "fonts/HGWT_CNKI.TTF", 40);
	
	auto menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(StartGUI::StartGame, this));
	
	//音量调节按钮
	auto label2 = Label::createWithSystemFont(GBK2UTF82("音量调节"), "fonts/HGWT_CNKI.TTF", 40);

	auto menuitem2 = MenuItemLabel::create(label2, CC_CALLBACK_1(StartGUI::Gameset, this));
	menuitem->setTag(10001);
	menuitem2->setTag(10002);

	auto menu = Menu::create(menuitem, menuitem2, NULL);
	menu->alignItemsVertically();

	//音乐开关按钮
	auto label3 = Sprite::create("music.png");

	label3->setTag(1111);
	auto menuitem3 = MenuItemLabel::create(label3, CC_CALLBACK_1(StartGUI::MusicSet, this));
	menuitem3->setPosition(Vec2(350,300));

	menuitem3->setTag(2222);
	auto m = Menu::create(menuitem3, NULL);
	m->setTag(3333);
	this->addChild(m, 12);

	//游戏信息按钮
	auto label4 = MenuItemImage::create(
		"mess.png",
		"mess.png",
		CC_CALLBACK_1(StartGUI::Messages, this));
	auto menuitem4 = MenuItemLabel::create(label4, CC_CALLBACK_1(StartGUI::Messages, this));
	menuitem4->setPosition(Vec2(400, 300));
	auto m2 = Menu::create(menuitem4, NULL);
	this->addChild(m2, 12);	
	this->addChild(menu,10);



	return true;
}


void  StartGUI::StartGame2(Ref* pSender) {
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

//进入游戏
void StartGUI::StartGame(Ref* pSender)
{
	auto go = CallFuncN::create([&](Ref* psender) {
		Director::getInstance()->replaceScene(MazeScene::createScene());
		});
	auto loadvideos = CallFuncN::create([&](Ref* psender) {
		Vector<SpriteFrame* >frameVector;
		for (int i = 2; i <= 606; i += 2)
		{
		
			auto spriteFrame = SpriteFrame::create("show/" + to_string(i) + ".jpg", Rect(0, 0, 1424, 768));
			frameVector.pushBack(spriteFrame);
		}
		auto animation = Animation::createWithSpriteFrames(frameVector);
		animation->setDelayPerUnit(0.07f);
		auto animate = Animate::create(animation);


		auto pic = Sprite::create();
		pic->runAction(RepeatForever::create(animate));
		this->addChild(pic, 10000);
		pic->setPosition(Vec2(700, 350));
		
		});
	auto opt = Sequence::create(
		loadvideos,
		DelayTime::create(7.5),
		go,
		nullptr		
	);
	auto act = RepeatForever::create(opt);
	this->runAction(act);

}

//进入游戏设置
void StartGUI::Gameset(Ref* pSender)
{
	Director::getInstance()->replaceScene(Gameset::createScene());

}

int tag = 0;
//进入音乐设置
void StartGUI::MusicSet(Ref* pSender)
{
	auto b = (Sprite*)this->getChildByTag(3333)->getChildByTag(2222)->getChildByTag(1111);
	if (tag == 0) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();//暂停背景音乐

		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("music2.png");
		b->setTexture(texture);
		tag = 1;

		musicoff = 1;
	}
	else {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->rewindBackgroundMusic();//重头调用背景音乐

		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("music.png");
		b->setTexture(texture);
		tag = 0;

		musicoff = 0;
	}

}

//进入版权信息
void StartGUI::Messages(Ref* pSender)
{

	Director::getInstance()->replaceScene(Message::createScene());
}