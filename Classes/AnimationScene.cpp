#include "AnimationScene.h"
#include"MazeScene.h"
#include"End.h"
#include"config.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

int gametime = 0;
int wintime = 0;


void sleep() {
	for (int i = 0; i < 100000; i++) {

	}

}

AnimationScene::AnimationScene()
{

}

AnimationScene::~AnimationScene()
{

}

bool AnimationScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();


	if (musicoff == 0) {


		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bosspk.mp3", true); //播放背景音乐

	}

	// 背景
	auto background = LayerColor::create(Color4B(128, 128, 128, 255), 960, 640);
	background->setPosition(Vec2(0, 0));
	background->setTag(110);
	this->addChild(background);
	auto cloud_1 = Sprite::create("Chapter06/AnimationScene/animation/background/Boold1.png");
	cloud_1->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2));
	cloud_1->setTag(112);
	this->addChild(cloud_1);
	auto cloud_2 = Sprite::create("Chapter06/AnimationScene/animation/background/Boold2.png");
	cloud_2->setPosition(Vec2(cloud_1->getPositionX() + cloud_1->getContentSize().width, visibleSize.height / 2));
	cloud_2->setTag(113);
	this->addChild(cloud_2);

	// 背景文字VS
	auto tips_vs = Label::createWithBMFont("fonts/futura-48.fnt", "VS");
	tips_vs->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 80));
	this->addChild(tips_vs, 5);

	// 摇杆
	m_joystick = Joystick::create(Vec2(100, 100), 50.0f, "Chapter06/AnimationScene/animation/control/j-btn.png", "Chapter06/AnimationScene/animation/control/j-bg.png");
	this->addChild(m_joystick, 4);

	// 攻击
	auto attackItem = MenuItemImage::create(
		"Chapter06/AnimationScene/animation/control/j-btn.png",
		"Chapter06/AnimationScene/animation/control/j-btn.png",
		CC_CALLBACK_1(AnimationScene::attackCallback, this));

	attackItem->setPosition(Vec2(visibleSize.width - 80, 200));

	auto defendItem = MenuItemImage::create(
		"Chapter06/AnimationScene/animation/control/j-btn.png",
		"Chapter06/AnimationScene/animation/control/j-btn.png",
		CC_CALLBACK_1(AnimationScene::defendCallback, this));

	defendItem->setPosition(Vec2(visibleSize.width - 200, 200));

	auto act1 = FadeOut::create(1);
	auto act2 = FadeIn::create(1);
	auto act3 = Sequence::create(act1, act2, NULL);
	auto act = RepeatForever::create(act3);  // 按钮闪烁
	attackItem->runAction(act);
	//defendItem->runAction(act);
	auto menu = Menu::create(attackItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 5);
	auto menu2 = Menu::create(defendItem, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 6);

	auto replayItem = MenuItemImage::create(
		"contin.png",
		"contin.png",
		CC_CALLBACK_1(AnimationScene::menuReplayCallback, this));
	replayItem->setPosition(Vec2(visibleSize.width - 50, 50));
	replayItem->setVisible(0);
	replayItem->setTag(10086);
	menu2 = Menu::create(replayItem, NULL);
	menu2->setPosition(Vec2::ZERO);
	menu2->setTag(1008);
	this->addChild(menu2, 7);
	


	// position the label on the center of the screen
	scoreboard= Label::createWithTTF("ALL Round: " + std::to_string(gametime) + "  you win:"+std::to_string(wintime), "fonts/Marker Felt.ttf", 48);
	scoreboard->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - scoreboard->getContentSize().height - 200));
	// add the label as a child to this layer
	//this->addChild(scoreboard, 1);
	// 地图
	auto map = TMXTiledMap::create("Chapter06/AnimationScene/animation/background/background.tmx");
	if (map == NULL)
	{
		CCLOG("map load failed!");
		return false;
	}
	Size mapSize = map->getMapSize();
	Size tiledSize = map->getTileSize();
	map->setPosition(Vec2(0, 0));
	map->setTag(111);
	//this->addChild(map, 2);

	// enemy
	m_enemy = Enemy::create(Vec2(visibleSize.width - 200, 150));
	m_enemy->setAttack2(false);

	this->addChild(m_enemy, 3);

	// hero
	m_player = Hero::create(Vec2(visibleSize.width / 4, 100));
	this->addChild(m_player, 3);

	// AI 
	m_aimanager = AIManager::create(this);
	m_aimanager->setAI(m_enemy, m_player);

	// 碰撞检测
	m_contactListener = MyContactListener::create(this, m_player, m_enemy);

	// 血条
	m_bloodBar_h = Sprite::create("Chapter06/AnimationScene/animation/background/1-new.png");
	m_bloodBar_h->setTag(10000);
	auto blood_bg_h = Sprite::create("Chapter06/AnimationScene/animation/background/1-new-f.png");
	m_bloodBar_h->setPosition(Vec2(m_bloodBar_h->getContentSize().width / 2 + 5, visibleSize.height - 50));
	blood_bg_h->setPosition(Vec2(m_bloodBar_h->getContentSize().width / 2 + 5, visibleSize.height - 50));

	this->addChild(blood_bg_h, 5);
	this->addChild(m_bloodBar_h, 5);

	m_bloodBar_e = Sprite::create("Chapter06/AnimationScene/animation/background/2-new.png");
	m_bloodBar_e->setTag(10001);
	auto blood_bg_e = Sprite::create("Chapter06/AnimationScene/animation/background/2-new-f.png");
	m_bloodBar_e->setPosition(Vec2(visibleSize.width - m_bloodBar_e->getContentSize().width / 2 - 5, visibleSize.height - 50));
	blood_bg_e->setPosition(Vec2(visibleSize.width - m_bloodBar_e->getContentSize().width / 2 - 5, visibleSize.height - 50));

	this->addChild(blood_bg_e, 5);
	this->addChild(m_bloodBar_e, 5);

	// update
	this->scheduleUpdate();

	return true;
}

Scene* AnimationScene::createScene()
{
	auto scene = Scene::create();
	auto layer = AnimationScene::create();
	scene->addChild(layer);
	return scene;
}

int flag = 1;

void AnimationScene::update(float delta)
{
	// 背景云动画
	Size visibleSize = Director::getInstance()->getVisibleSize();	
	auto cloud_1 = (Sprite*)getChildByTag(112);
	auto cloud_2 = (Sprite*)getChildByTag(113);
	if (flag %2 ==0) {
		if (flag > 10000) {
			flag = 0;
		}
		cloud_1 = (Sprite*)getChildByTag(113);
		cloud_2 = (Sprite*)getChildByTag(112);
	}
	if (cloud_1->getPositionX() > -(-200+visibleSize.width))
	{
		cloud_1->setPositionX(cloud_1->getPositionX() - 1);
		
	}
	else
	{
		flag++;

	}
	cloud_2->setPositionX(cloud_1->getPositionX() + cloud_1->getContentSize().width);

	// 更新血条
	float temp_e = float(m_enemy->getLife()) / m_enemy->getMaxLife();
	float x = 44 + (354 - 88 - 44) * (1 - temp_e);
	m_bloodBar_e->setTextureRect(Rect(x, 0, 354 - x, m_bloodBar_e->getContentSize().height));
	m_bloodBar_e->setPositionX(visibleSize.width - m_bloodBar_e->getContentSize().width / 2 - 5);

	float temp_h = float(m_player->getLife()) / m_player->getMaxLife();
	m_bloodBar_h->setTextureRect(Rect(0, 0, 88 + (354 - 88 - 44) * temp_h, m_bloodBar_h->getContentSize().height));
	m_bloodBar_h->setPositionX(m_bloodBar_h->getContentSize().width / 2 + 5);


	Vec2 opt = m_joystick->getDirection();

	if (m_joystick->getDirection().x == 0 && m_joystick->getDirection().y == 0)
	{
		m_player->play(STAND);
	}

	else{
		m_player->setPositionX(m_player->getPositionX() + m_joystick->getPower() * opt.x / sqrt(opt.x * opt.x + opt.y * opt.y));
		m_player->setPositionY(m_player->getPositionY() + m_joystick->getPower() * opt.y / sqrt(opt.x * opt.x + opt.y * opt.y));
		m_player->play(MOVETOP);
	}
	


	if (m_enemy->isDeath()) {
		Label* success = Label::createWithTTF("SUCCESS!", "fonts/Marker Felt.ttf", 72);
		
				
				// position the label on the center of the screen
				success->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
				success->setName("game success");
				// add the label as a child to this layer				
				
				

				auto go = CallFuncN::create([&](Ref* psender) {
					m_enemy->setVisible(0);
					});

				auto opt = Sequence::create(
					
					DelayTime::create(0.5),
					go,
					nullptr

				);
				auto act = RepeatForever::create(opt);
				this->runAction(act);

				auto label4 = MenuItemImage::create(
					"contin.png",
					"contin.png",
					CC_CALLBACK_1(AnimationScene::defendCallback, this));

				//auto label4 = Label::createWithSystemFont(GBK2UTF82("游戏信息"), "fonts/HGWT_CNKI.TTF", 40);

				//按下确认，跳进主界面
				auto menuitem4 = MenuItemLabel::create(label4, CC_CALLBACK_1(AnimationScene::defendCallback, this));
				menuitem4->setPosition(Vec2(400, 200));
				auto m2 = Menu::create(menuitem4, NULL);
				this->addChild(m2, 12);
				
			
			
		
			
		
	}
	Label* success = Label::createWithTTF("you Loss!", "fonts/Marker Felt.ttf", 72);

	if (m_player->isDeath()) {

		auto go = CallFuncN::create([&](Ref* psender) {
			m_player->setVisible(0);
			});

		auto opt = Sequence::create(

			DelayTime::create(0.2),
			go,
			nullptr

		);
		auto act = RepeatForever::create(opt);
		this->runAction(act);

		success->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		success->setName("game over");
		this->addChild(success, 10);

		auto a = this->getChildByTag(1008);
		auto b = a->getChildByTag(10086);
		b->setVisible(1);
	}
	
}

void AnimationScene::attackCallback(Ref* pSender)
{
	if (m_player->isAttack() == false)
		m_player->play(ATTACK);
	// m_enemy->play(ATTACK);
}

void AnimationScene::defendCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(End::createScene());
}
void AnimationScene::menuReplayCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	flag = 1;
	if (m_enemy->isDeath()) {
		wintime++;
		
	}
	
	gametime++;

	if (gametime == 3) {
		gametime = 0;
		wintime = 0;
	}

	Director::getInstance()->replaceScene(AnimationScene::createScene());
	Director::getInstance()->resume();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}