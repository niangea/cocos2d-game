#include "HelloWorldScene.h"
#include<string>
#include "SimpleAudioEngine.h"
#include"MazeScene.h"
#include"config.h"
#include"AnimationScene.h"
#include"Boss.h"

using namespace std;

USING_NS_CC;

int state = 0;

int f;

double bloodsize = 1;
double writesize = 5;
double feixibao = 5;
double Hcl = 5;
int gansize = 5;




Sprite* b;

// GBK转UTF-8
string GBKToUTF82(const std::string& gbk) {
	string utf_8 = "";

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

Scene* HelloWorld::createScene()
{
	f = 0;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}



static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{

	flag2 = 1;

	go = CallFuncN::create([&](Ref* psender) {
		Director::getInstance()->replaceScene(AnimationScene::createScene());
		});

	loadvideos = CallFuncN::create([&](Ref* psender) {
		// 开始界面演示动漫
		Vector<SpriteFrame* >frameVector;
		for (int i = 10; i <= 190; i += 10)
		{
			//这里的i对应的是切割出来的视频帧号
			auto spriteFrame = SpriteFrame::create("BOSS/" + to_string(i) + ".jpg", Rect(0, 0, 1424, 768));
			frameVector.pushBack(spriteFrame);
		}
		auto animation = Animation::createWithSpriteFrames(frameVector);
		animation->setDelayPerUnit(0.07f);
		auto animate = Animate::create(animation);

		// 添加画面
		auto pic = Sprite::create();
		//pic->setTag(PLAYER_TAG);
		pic->runAction(RepeatForever::create(animate));
		this->addChild(pic, 10000);
		pic->setPosition(Vec2(700, 350));

		});


	if (!Layer::init())
	{
		return false;
	}

	if (musicoff == 0) {


		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("3.mp3", true); //播放背景音乐

	}
	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//添加颜色层
	auto colorLayer = LayerColor::create(Color4B(128, 125, 200, 255), 480, visibleSize.height);
	colorLayer->setPosition(Vec2(200, 0));
	//this->addChild(colorLayer);

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal2.png",
		"CloseSelected2.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	

	
	if (scoreboard == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		scoreboard=Label::createWithTTF(GBKToUTF82("计分板:  ") + std::to_string(score), "fonts/HGWT_CNKI.TTF", 48);
		scoreboard->setPosition(Vec2(origin.x + visibleSize.width / 3, origin.y + visibleSize.height - scoreboard->getContentSize().height));
		scoreboard->setName(GBKToUTF82("计分板"));
		// add the label as a child to this layer
		this->addChild(scoreboard, 1);
	}

	//移动精灵
	bean = Sprite::create("Chapter10/bool.png");
	bean->setPosition(Point(visibleSize.width / 2, 100));

	bean->setTag(10086);

	this->addChild(bean, 6);

	auto cloud_1 = Sprite::create("Chapter06/AnimationScene/animation/background/Boold1.png");
	cloud_1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	cloud_1->setTag(112);
	this->addChild(cloud_1);
	auto cloud_2 = Sprite::create("Chapter06/AnimationScene/animation/background/Boold2.png");
	cloud_2->setPosition(Vec2(cloud_1->getPositionX() + cloud_1->getContentSize().width, visibleSize.height / 2));
	cloud_2->setTag(113);
	this->addChild(cloud_2);


	// 摇杆
	m_joystick = Joystick::create(Vec2(100, 100), 50.0f, "Chapter10/j-btn.png", "Chapter10/j-bg.png");
	this->addChild(m_joystick, 4);

	addKeyBoardListener();

	this->schedule(schedule_selector(HelloWorld::addBall1), 1.0f);
	this->schedule(schedule_selector(HelloWorld::addBall2), 2.0f);
	this->schedule(schedule_selector(HelloWorld::addBall3), 3.0f);
	this->scheduleUpdate();



	return true;
}
void HelloWorld::addBall1(float dt) {
	if (guan == 3) {


		auto ball1 = Sprite::create("Chapter10/oxygen2.png");//使用图片创建小球
	
	ball1->setPosition(Point(visibleSize.width, 207 + rand() % 460));//设置小球的初始位置，这里在x方向使用了随机函数rand使得小球在随机位置出现
	this->addChild(ball1, 5);
	this->ballVector.pushBack(ball1);//将小球放进Vector数组C
	auto moveTo = MoveTo::create(rand() % 5, Point(-10, ball1->getPositionY()));//移动动作
	auto actionDone = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball1));//当小球移动到屏幕下方时回调removeBall函数，移除小球
	auto sequence = Sequence::create(moveTo, actionDone, nullptr);
	ball1->runAction(sequence);//执行动作
	}

	if (guan == 2) {
		auto ball1 = Sprite::create("jiujing.png");//使用图片创建小球

		ball1->setPosition(Point(visibleSize.width, 207 + rand() % 460));//设置小球的初始位置，这里在x方向使用了随机函数rand使得小球在随机位置出现
		this->addChild(ball1, 5);
		this->ballVector.pushBack(ball1);//将小球放进Vector数组
		auto moveTo = MoveTo::create(rand() % 5, Point(-10, ball1->getPositionY()));//移动动作
		auto actionDone = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball1));//当小球移动到屏幕下方时回调removeBall函数，移除小球
		auto sequence = Sequence::create(moveTo, actionDone, nullptr);
		ball1->runAction(sequence);//执行动作

	}
	if (guan == 1) {
		auto ball1 = Sprite::create("shiwu.png");//使用图片创建小球

		ball1->setPosition(Point(visibleSize.width, 207 + rand() % 460));//设置小球的初始位置，这里在x方向使用了随机函数rand使得小球在随机位置出现
		this->addChild(ball1, 5);
		this->ballVector.pushBack(ball1);//将小球放进Vector数组
		auto moveTo = MoveTo::create(rand() % 5, Point(-10, ball1->getPositionY()));//移动动作
		auto actionDone = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball1));//当小球移动到屏幕下方时回调removeBall函数，移除小球
		auto sequence = Sequence::create(moveTo, actionDone, nullptr);
		ball1->runAction(sequence);//执行动作

	}

	if (guan == 0) {


		auto ball1 = Sprite::create("yan.png");//使用图片创建小球

		ball1->setPosition(Point(visibleSize.width, 207 + rand() % 460));//设置小球的初始位置，这里在x方向使用了随机函数rand使得小球在随机位置出现
		this->addChild(ball1, 5);
		this->ballVector.pushBack(ball1);//将小球放进Vector数组
		auto moveTo = MoveTo::create(rand() % 5, Point(-10, ball1->getPositionY()));//移动动作
		auto actionDone = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball1));//当小球移动到屏幕下方时回调removeBall函数，移除小球
		auto sequence = Sequence::create(moveTo, actionDone, nullptr);
		ball1->runAction(sequence);//执行动作

	}


}

void HelloWorld::addBall2(float dt) {
	auto ball2 = Sprite::create("Chapter10/virus1.png");
	ball2->setPosition(Point(visibleSize.width, 207 + rand() % 460));
	this->addChild(ball2, 5);
	this->ballVector.pushBack(ball2);
	auto moveTo = MoveTo::create(rand() % 5, Point(-10, ball2->getPositionY()));
	auto actionDone = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball2));
	auto sequence = Sequence::create(moveTo, actionDone, nullptr);
	ball2->runAction(sequence);
}
void HelloWorld::addBall3(float dt) {
	/*
	auto ball3 = Sprite::create("Chapter10/ball2.png");
	ball3->setPosition(Point(visibleSize.width, 207 + rand() % 460));
	this->addChild(ball3, 5);
	this->ballVector.pushBack(ball3);
	auto moveTo = MoveTo::create(rand() % 5, Point( -10, ball3->getPositionY()));
	auto actionDone = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball3));
	auto sequence = Sequence::create(moveTo, actionDone, nullptr);
	ball3->runAction(sequence);
	*/
}
void HelloWorld::removeBall(Sprite* ball) {
	this->removeChild(ball);
	ballVector.eraseObject(ball);
}




void HelloWorld::update(float dt) {

	auto cloud_1 = (Sprite*)this->getChildByTag(112);
	auto cloud_2 = (Sprite*)this->getChildByTag(113);
	if (flag2 % 2 == 0) {
		if (flag2 > 10000) {
			flag2 = 0;
		}
		cloud_1 = (Sprite*)this->getChildByTag(113);
		cloud_2 = (Sprite*)this->getChildByTag(112);
	}
	if (cloud_1->getPositionX() > -(-200 + visibleSize.width))
	{
		cloud_1->setPositionX(cloud_1->getPositionX() - 1);

	}
	else
	{
		flag2++;

	}
	cloud_2->setPositionX(cloud_1->getPositionX() + cloud_1->getContentSize().width);
	

	for (auto ball : ballVector)
	{
		
		
		
		//碰撞检测
		if (bean->getBoundingBox().intersectsRect(ball->getBoundingBox()))
		{
			
			if (ball->getResourceName() == "Chapter10/oxygen2.png" && state == 0) {

				if (bloodsize < 3) {
					bloodsize += 0.1;
				}

				
				b = (Sprite*)this->getChildByTag(10086);

				b->setScale(bloodsize);

				auto actionDown = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball));
				ball->runAction(actionDown);

				score += 1;
			}
				
			else if (ball->getResourceName() == "Chapter10/virus1.png" && state == 2) {

				if (writesize > 0.1) {
					writesize -= 0.3;
				}

				
				b = (Sprite*)this->getChildByTag(10086);

				b->setScale(writesize);

				//得分音效
				auto actionDown = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball));
				ball->runAction(actionDown);

				//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("9266.mp3");

				score += 2;
			}
				
			else if (ball->getResourceName() == "jiujing.png" && state == 1) {
				if (gansize > 0.1) {
					gansize -= 0.3;
				}


				b = (Sprite*)this->getChildByTag(10086);

				b->setScale(gansize);

			
				auto actionDown = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball));
				ball->runAction(actionDown);

				//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("9266.mp3");

				score += 2;
			}

			else if (ball->getResourceName() == "shiwu.png" && state == 3) {
				if (Hcl > 0.1) {
					Hcl -= 0.3;
				}


				b = (Sprite*)this->getChildByTag(10086);

				b->setScale(Hcl);


				auto actionDown = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball));
				ball->runAction(actionDown);

				//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("9266.mp3");

				score += 2;
			}

			else if (ball->getResourceName() == "yan.png" && state == 3) {
				if (feixibao > 0.1) {
					feixibao -= 0.3;
				}


				b = (Sprite*)this->getChildByTag(10086);

				b->setScale(feixibao);


				auto actionDown = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball));
				ball->runAction(actionDown);

				//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("9266.mp3");

				score += 2;
			}


			//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("14633.mp3");
				
			scoreboard->setString(GBKToUTF82("计分板:  ") + std::to_string(score));
		}
	}

	
	Vec2 opt = m_joystick->getDirection();
	if (opt.x == 0 && opt.y == 0)
	{
		//不移动，那就什么都不用做
	}
	else {
		if (bean->getPositionY() - bean->getContentSize().height / 2 >= 0 && bean->getPositionY() - bean->getContentSize().height / 2 <= 666) {
			bean->setPositionY(bean->getPositionY() + m_joystick->getPower() * opt.y / sqrt(opt.x * opt.x + opt.y * opt.y));
		}
		else if (bean->getPositionY() - bean->getContentSize().height / 2 < 0) {
			bean->setPositionY(bean->getContentSize().height / 2);
		}			
		else if (bean->getPositionY() - bean->getContentSize().height / 2 > 666) {
			bean->setPositionY(666 + bean->getContentSize().height / 2);
		}
			
		if (bean->getPositionX() + bean->getContentSize().width / 2 <= 666 && bean->getPositionX() - bean->getContentSize().width / 2 >= 222) {
			bean->setPositionX(bean->getPositionX() + m_joystick->getPower() * opt.x / sqrt(opt.x * opt.x + opt.y * opt.y));
		}
		else if (bean->getPositionX() + bean->getContentSize().width / 2 > 666) {
			bean->setPositionX(666 - bean->getContentSize().width / 2);
		}			
		else if (bean->getPositionX() - bean->getContentSize().width / 2 < 222) {
			bean->setPositionX(222 + bean->getContentSize().width / 2);
		}
			
	}
	//}

	
	if (score > 10) {
		if (f == 0) {
			yin++;
			f = 1;
		}
		

		if (yin >= 4) {



			auto label4 = MenuItemImage::create(
				"contin.png",
				"contin.png",
				CC_CALLBACK_1(HelloWorld::gettoboss, this));



			//按下确认，跳进主界面
			auto menuitem4 = MenuItemLabel::create(label4, CC_CALLBACK_1(HelloWorld::gettoboss, this));
			menuitem4->setPosition(Vec2(400, 100));
			menuitem4->setScale(0.5);
			auto m2 = Menu::create(menuitem4, NULL);
			this->addChild(m2, 12);


			
			


		}
		else {

			auto replaybutton = MenuItemImage::create(
				"contin.png",
				"contin.png",
				CC_CALLBACK_1(HelloWorld::replayfuntion, this));

			replaybutton->setPosition(Vec2(400, 100));
			replaybutton->setScale(0.5);


			auto menu2 = Menu::create(replaybutton, NULL);
			menu2->setPosition(Vec2::ZERO);
			this->addChild(menu2, 10);

		}

		


		//Director::getInstance()->pause();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		Label* success = Label::createWithTTF(GBKToUTF82("游戏通关！"), "fonts/HGWT_CNKI.TTF", 77);
		if (success == nullptr)
		{
			problemLoading("'fonts/Marker Felt.ttf'");
		}
		else
		{
			
			
			success->setPosition(Vec2(origin.x + visibleSize.width / 2.5, origin.y + visibleSize.height / 2));
			success->setName("游戏通关");

			if (musicoff == 0) {
				//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("12693.mp3");
			}

			win++;
			
			
			this->addChild(success, 1);
		}
	}



}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::replayfuntion(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the replay button. Windows Store Apps do not implement a replay button.", "Alert");
	return;
#endif
	

	//this->score =0;
	Director::getInstance()->replaceScene(MazeScene::createScene());
	//Director::getInstance()->resume();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::addKeyBoardListener() {
	auto a = EventListenerKeyboard::create();
	a->onKeyPressed = CC_CALLBACK_2(HelloWorld::RestartGameBegan, this);
	a->onKeyReleased = CC_CALLBACK_2(HelloWorld::RestartGameEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(a, this);
}

bool HelloWorld::RestartGameBegan(EventKeyboard::KeyCode keycode, Event* unused_event)
{

	
	
	

	if (keycode == EventKeyboard::KeyCode::KEY_S) {
	
		
		b = (Sprite*)this->getChildByTag(10086);

		state = 1;
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("gan.png");
		b->setTexture(texture);
			
		b->setScale(gansize);
	}

	if (keycode == EventKeyboard::KeyCode::KEY_D) {

		b = (Sprite*)this->getChildByTag(10086);

		state = 2;
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("Chapter10/wr.png");
		b->setTexture(texture);

		b->setScale(writesize);

	}

	if (keycode == EventKeyboard::KeyCode::KEY_A) {

		b = (Sprite*)this->getChildByTag(10086);

		state = 0;
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("Chapter10/bool.png");
		b->setTexture(texture);
		b->setScale(bloodsize);

	}

	if (keycode == EventKeyboard::KeyCode::KEY_F) {

		b = (Sprite*)this->getChildByTag(10086);

		state = 3;
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("hcl.png");
		b->setTexture(texture);

		b->setScale(Hcl);
	}

	if (keycode == EventKeyboard::KeyCode::KEY_G) {

		b = (Sprite*)this->getChildByTag(10086);

		state = 3;
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("fei.png");
		b->setTexture(texture);

		b->setScale(feixibao);
	}


	return true;
}

void HelloWorld::RestartGameEnded(EventKeyboard::KeyCode keycode, Event* unused_event)
{
	return;
}



void HelloWorld::gettoboss(Ref* pSender) {

	
	

		//Director::getInstance()->replaceScene(Gameset::createScene());
	Director::getInstance()->replaceScene(Boss::createScene());
	
}