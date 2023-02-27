#include "MazeScene.h"
#include"HelloWorldScene.h"
#include"AnimationScene.h"
#include"config.h"

USING_NS_CC;
int STAGE = 1;
Scene* MazeScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MazeScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MazeScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_map = nullptr;
	//寻路起点指针
	m_origin = nullptr;
	//寻路终点指针
	m_destination = nullptr;
	//判断是否可以在地图上移动
	m_isReadyToMove = true;

	//获得屏幕尺寸
	m_visibleSize = Director::getInstance()->getVisibleSize();
	//载入地图，将放置到适当位置

	if (yin == 0) {
		auto tileMazeScene = TMXTiledMap::create("Chapter11/superMaze.tmx");
		tileMazeScene->setPosition(0, m_visibleSize.height - MAP_HEIGHT * UNIT);
		this->addChild(tileMazeScene, 1);
		m_mapLayer = tileMazeScene->getLayer("mainLayer3");
	}
	if (yin == 1) {
		auto tileMazeScene = TMXTiledMap::create("Chapter11/sup0.tmx");
		tileMazeScene->setPosition(0, m_visibleSize.height - MAP_HEIGHT * UNIT);
		this->addChild(tileMazeScene, 1);
		m_mapLayer = tileMazeScene->getLayer("mainLayer3");
	}
	if (yin == 2) {
		auto tileMazeScene = TMXTiledMap::create("Chapter11/sup1.tmx");
		tileMazeScene->setPosition(0, m_visibleSize.height - MAP_HEIGHT * UNIT);
		this->addChild(tileMazeScene, 1);
		m_mapLayer = tileMazeScene->getLayer("mainLayer3");
	}
	if (yin == 3) {
		auto tileMazeScene = TMXTiledMap::create("Chapter11/sup2.tmx");
		tileMazeScene->setPosition(0, m_visibleSize.height - MAP_HEIGHT * UNIT);
		this->addChild(tileMazeScene, 1);
		m_mapLayer = tileMazeScene->getLayer("mainLayer3");
	}
	



	//初始化地图数组
	initMazeScene();
	//在屏幕底端中央显示闪烁的操作提示
	auto playTip = LabelTTF::create("Please treat this body", "Arial", 40);
	playTip->setPosition(m_visibleSize.width / 2, 0.5 * (m_visibleSize.height - MAP_HEIGHT * UNIT));
	this->addChild(playTip, 1);
	auto blink = Blink::create(5, 10);
	playTip->runAction(blink);
	//预置找到宝箱的提示，将其设置为不可见
	auto successTip = Label::createWithBMFont("fonts/futura-48.fnt", "loading ...");
	successTip->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	this->addChild(successTip, 2, TIP_TAG);
	successTip->setVisible(false);
	//在地图起点放置寻宝精灵
	auto smileSprite = Sprite::create("Chapter11/robot2.png");
	smileSprite->setPosition(xx, yy);
	this->addChild(smileSprite, 3, SMILE_TAG);
	//预置宝箱精灵，将其设置为不可见
	auto boxSprite = Sprite::create("zuobiao.png");
	boxSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	boxSprite->setVisible(false);
	this->addChild(boxSprite, 3, BOX_TAG);
	//预置禁止放置图标，将其设置为不可见
	auto forbidSprite = Sprite::create("Chapter11/forbid.png");
	forbidSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	forbidSprite->setVisible(false);
	this->addChild(forbidSprite, 4, FORBID_TAG);
	//添加绘图节点
	m_draw = DrawNode::create();
	this->addChild(m_draw, 2, DRAW_TAG);
	//添加屏幕触摸监听
	addTouchListener();
	addKeyBoardListener();
	//调用定时器更新
	this->scheduleUpdate();
	return true;
}


void MazeScene::update(float delta)
{
	auto smile = this->getChildByTag(SMILE_TAG);
	auto box = this->getChildByTag(BOX_TAG);
	//如果笑脸精灵达到宝箱位置
	if (smile->getBoundingBox().intersectsRect(box->getBoundingBox()))
	{
		//隐藏宝箱
		box->setVisible(false);
		//将地图数组恢复初始状态
		for (int i = 0; i < MAP_WIDTH; i++)
		{
			for (int j = 0; j < MAP_HEIGHT; j++)
			{
				if (m_map[i][j].status != NOT_ACCESS)
				{
					mapNode temp = { ACCESS, i, j, 0, 0, 0, nullptr };
					m_map[i][j] = temp;
				}
			}
		}
		//将当前单元设为起点
		m_origin = m_destination;
		m_origin->status = ORIGIN;
		//清除绘制的路径
		m_draw->clear();
		//允许再次移动
		m_isReadyToMove = true;
		//显示寻路成功的标签提示
		auto tip = this->getChildByTag(TIP_TAG);
		tip->setVisible(true);
	}
}

void MazeScene::initMazeScene()
{
	srand((unsigned)time(NULL));
	//根据地图宽、高分配数组空间
	m_map = new mapNode * [MAP_WIDTH];
	for (int n = 0; n < MAP_WIDTH; n++)
		m_map[n] = new mapNode[MAP_HEIGHT];
	//依次扫描地图数组每一个单元
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{

			//若当前位置为墙体瓦片设置为不可通过

			auto a = m_mapLayer->getTileGIDAt(Vec2(i, j));
			if (m_mapLayer->getTileGIDAt(Vec2(i, j)) == NOT_ACCESS_TILE)
			{
				mapNode temp = { NOT_ACCESS, i, j, 0, 0, 0, nullptr };
				m_map[i][j] = temp;
			}

			//否则设置为可以通过
			else
			{
				
				mapNode temp = { ACCESS, i, j, 0, 0, 0, nullptr };
				m_map[i][j] = temp;

				

			}
			

		}
		
	}
	
	m_map[xpos][ypos].status = ORIGIN;
	m_origin = &m_map[xpos][ypos];
}

int MazeScene::aStar(mapNode** map, mapNode* origin, mapNode* destination)
{
	openList* open = new openList;
	open->next = nullptr;
	open->openNode = origin;
	closedList* close = new closedList;
	close->next = nullptr;
	close->closedNode = nullptr;
	//循环检验8个方向的相邻节点
	while (checkNeighboringNodes(map, open, open->openNode, destination))
	{
		//从OPEN表中选取节点插入CLOSED表
		insertNodeToClosedList(close, open);
		//若OPEN表为空，表明寻路失败
		if (open == nullptr)
		{
			//屏幕中央显示寻路失败的提示
			auto failedTip = Label::createWithBMFont("fonts/futura-48.fnt", "No Way!");
			failedTip->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
			this->addChild(failedTip, 2, TIP_TAG);
			break;
		}
		//若终点在OPEN表中，表明寻路成功
		if (open->openNode->status == DESTINATION)
		{
			mapNode* tempNode = open->openNode;
			//调用moveOnPath（）函数控制精灵在路径上移动
			moveOnPath(tempNode);
			break;
		}
	}
	return 0;
}

void MazeScene::moveOnPath(mapNode* tempNode)
{
	string next;
	//声明存储路径坐标的结构体
	struct pathCoordinate { int x; int y; bool isDangerous; };
	//分配路径坐标结构体数组
	pathCoordinate* path = new pathCoordinate[MAP_WIDTH * MAP_HEIGHT];
	//利用父节点信息逆序存储路径坐标
	int loopNum = 0;
	while (tempNode != nullptr)
	{
		path[loopNum].x = tempNode->xCoordinate;
		path[loopNum].y = tempNode->yCoordinate;
		path[loopNum].isDangerous = tempNode->isDangerous;
		loopNum++;
		tempNode = tempNode->parent;
	}
	//将笑脸精灵的坐标存为绘制线段起点
	auto smile = this->getChildByTag(SMILE_TAG);
	int fromX = smile->getPositionX();
	int fromY = smile->getPositionY();
	//声明动作向量存储动作序列
	Vector<FiniteTimeAction*> actionVector;
	//从结构体数组尾部开始扫描
	for (int j = loopNum - 2; j >= 0; j--)
	{
		//将地图数组坐标转化为屏幕实际坐标
		int realX = (path[j].x + 0.5) * UNIT;
		int realY = m_visibleSize.height - (path[j].y + 0.5) * UNIT;
		//绘制从起点到下一个地图单元的线段
		m_draw->drawLine(Vec2(fromX, fromY), Vec2(realX, realY), Color4F(1.0, 1.0, 1.0, 1.0));
		if (path[j].isDangerous) {
			next = "dangerous";
			break;
		}
		if (path[j].x >= 12 && path[j].x <= 14 && path[j].y >= 11 && path[j].y <= 13&& f1 == 0) {
			next = "next_stage";
			f1 = 1;
			xx = (13 + 0.5) * UNIT;
			yy = m_visibleSize.height - (12 + 0.5) * UNIT;

			break;
		}
		else if (path[j].x >= 3 && path[j].x <= 5 && path[j].y >= 9&& path[j].y <=11 && f2 == 0) {
			next = "front_stage";
			xx = (4 + 0.5) * UNIT;
			yy = m_visibleSize.height - (10 + 0.5) * UNIT;
			f2 = 1;
			break;
		}
		else if (path[j].x >= 22 && path[j].x <= 24 && path[j].y >= 15&& path[j].y <= 17 && f3 == 0) {
			next = "last_stage";
			f3 = 1;
			xx = (23 + 0.5) * UNIT;
			yy = m_visibleSize.height - (16 + 0.5) * UNIT;

			break;
		}
		else if (path[j].x >= 13&& path[j].x <= 15 && path[j].y >= 1&& path[j].y<=3 && f4 == 0) {
			next = "fei_stage";
			f4 = 1;
			xx = (14 + 0.5) * UNIT;
			yy = m_visibleSize.height - (2 + 0.5) * UNIT;
			break;
		}
		//创建移动动作并存入动作向量
		auto moveAction = MoveTo::create(0.2, Vec2(realX, realY));
		actionVector.pushBack(moveAction);
		//将当前坐标保存为下一次绘制的起点
		fromX = realX;
		fromY = realY;
	}
	//创建动作序列
	if (actionVector.size()>0) {
		auto actionSequence = Sequence::create(actionVector);
		//笑脸精灵执行移动动作序列
		smile->runAction(Sequence::create(actionSequence, CallFunc::create(CC_CALLBACK_0(MazeScene::Next, this, next)), NULL));
	}
	
}

void MazeScene::Next(string s) {
	if (s == "dangerous") {
		//屏幕中央显示失败的提示
		auto failedTip = Label::createWithBMFont("fonts/futura-48.fnt", "Dangerous!You're Dead!");
		failedTip->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
		this->addChild(failedTip, 2, TIP_TAG);
	}
	else if (s == "next_stage") {

		guan = 3;

		xpos = 13;
		ypos = 12;

		auto smile = this->getChildByTag(SMILE_TAG);
		

		auto go = CallFuncN::create([&](Ref* psender) {
			Director::getInstance()->replaceScene(HelloWorld::createScene());
			});

		auto loadvideos = CallFuncN::create([&](Ref* psender) {
			// 开始界面演示动漫
			Vector<SpriteFrame* >frameVector;
			for (int i = 2; i <= 606; i += 2)
			{
				//这里的i对应的是切割出来的视频帧号
				auto spriteFrame = SpriteFrame::create("end/" + to_string(i) + ".jpg", Rect(0, 0, 1424, 768));
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
			pic->setPosition(Vec2(700, 390));
			});
		auto opt = Sequence::create(
			loadvideos,
			DelayTime::create(9.5),
			go,
			nullptr

		);
		auto act = RepeatForever::create(opt);
		this->runAction(act);

		//STAGE = 2;
		
	}
	else if (s == "front_stage") {
		
		guan = 2;
		xpos = 4;
		ypos = 10;

		auto smile = this->getChildByTag(SMILE_TAG);


		auto go = CallFuncN::create([&](Ref* psender) {
			Director::getInstance()->replaceScene(HelloWorld::createScene());
			});

		auto loadvideos = CallFuncN::create([&](Ref* psender) {
			// 开始界面演示动漫
			Vector<SpriteFrame* >frameVector;
			for (int i = 2; i <= 606; i += 2)
			{
				//这里的i对应的是切割出来的视频帧号
				auto spriteFrame = SpriteFrame::create("end/" + to_string(i) + ".jpg", Rect(0, 0, 1424, 768));
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
			pic->setPosition(Vec2(700, 390));
			});
		auto opt = Sequence::create(
			loadvideos,
			DelayTime::create(9.5),
			go,
			nullptr

		);
		auto act = RepeatForever::create(opt);
		this->runAction(act);

	}
	else if (s == "last_stage") {
		
		guan = 1;
		xpos = 23;
		ypos = 16;

		auto smile = this->getChildByTag(SMILE_TAG);


		auto go = CallFuncN::create([&](Ref* psender) {
			Director::getInstance()->replaceScene(HelloWorld::createScene());
			});

		auto loadvideos = CallFuncN::create([&](Ref* psender) {
			// 开始界面演示动漫
			Vector<SpriteFrame* >frameVector;
			for (int i = 2; i <= 606; i += 2)
			{
				//这里的i对应的是切割出来的视频帧号
				auto spriteFrame = SpriteFrame::create("end/" + to_string(i) + ".jpg", Rect(0, 0, 1424, 768));
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
			pic->setPosition(Vec2(700, 390));
			});
		auto opt = Sequence::create(
			loadvideos,
			DelayTime::create(9.5),
			go,
			nullptr

		);
		auto act = RepeatForever::create(opt);
		this->runAction(act);

	}
	else if (s == "fei_stage") {

	guan = 0;
	xpos = 14;
	ypos = 2;

	auto smile = this->getChildByTag(SMILE_TAG);


	auto go = CallFuncN::create([&](Ref* psender) {
		Director::getInstance()->replaceScene(HelloWorld::createScene());
		});

	auto loadvideos = CallFuncN::create([&](Ref* psender) {
		// 开始界面演示动漫
		Vector<SpriteFrame* >frameVector;
		for (int i = 2; i <= 606; i += 2)
		{
			//这里的i对应的是切割出来的视频帧号
			auto spriteFrame = SpriteFrame::create("end/" + to_string(i) + ".jpg", Rect(0, 0, 1424, 768));
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
		pic->setPosition(Vec2(700, 390));
		});
	auto opt = Sequence::create(
		loadvideos,
		DelayTime::create(9.5),
		go,
		nullptr

	);
	auto act = RepeatForever::create(opt);
	this->runAction(act);

	}


}

void MazeScene::addTouchListener()
{
	//获得监听器
	auto listener = EventListenerTouchOneByOne::create();
	//为触摸开始和触摸结束设置回调函数
	listener->onTouchBegan = CC_CALLBACK_2(MazeScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(MazeScene::onTouchEnded, this);
	//设置触摸监听优先级
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void MazeScene::addKeyBoardListener()
{

	auto a = EventListenerKeyboard::create();
	a->onKeyPressed = CC_CALLBACK_2(MazeScene::RestartGameBegan, this);
	a->onKeyReleased = CC_CALLBACK_2(MazeScene::RestartGameEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(a, this);

}

bool MazeScene::onTouchBegan(Touch* touch, Event* unused_event)
{

	int x = (int)(touch->getLocation().x / UNIT);
	int y = (int)((m_visibleSize.height - touch->getLocation().y) / UNIT);

	if ((m_destination && x == m_destination->xCoordinate && y == m_destination->yCoordinate) || m_map[x][y].status == NOT_ACCESS || y >= MAP_HEIGHT)
	{
		auto f = this->getChildByTag(FORBID_TAG);

		f->setPosition(0.5 * UNIT + x * UNIT, m_visibleSize.height - 0.5 * UNIT - y * UNIT);

		f->setVisible(true);
		return true;
	}
	else
	{
		//若该地图单元可到达且允许移动
		if (m_isReadyToMove)
		{
			//为到达终点前不再允许移动
			m_isReadyToMove = false;
			//将当前地图单元设置为终点
			m_map[x][y].status = DESTINATION;
			m_destination = &m_map[x][y];
			if (x != 3 && y != 0) {
				//在当前地图单元添加宝箱精灵
				auto box = this->getChildByTag(BOX_TAG);
				box->setPosition(0.5 * UNIT + x * UNIT, m_visibleSize.height - 0.5 * UNIT - y * UNIT);
				box->setVisible(true);
			}
			//调用A*算法开始寻路
			aStar(m_map, m_origin, m_destination);
			//将上一次寻路成功的提示隐藏
			auto tip = this->getChildByTag(TIP_TAG);
			tip->setVisible(false);
			return false;
		}
		//若不允许移动，在该地图单元设置禁止图标
		else
		{
			auto forbid = this->getChildByTag(FORBID_TAG);
			forbid->setPosition(0.5 * UNIT + x * UNIT, m_visibleSize.height - 0.5 * UNIT - y * UNIT);
			forbid->setVisible(true);
			return true;
		}
	}
}

void MazeScene::onTouchEnded(Touch* touch, Event* unused_event)
{
	//隐藏禁止图标
	auto forbid = this->getChildByTag(FORBID_TAG);
	forbid->setVisible(false);
	//隐藏寻路成功提示
	auto tip = this->getChildByTag(TIP_TAG);
	tip->setVisible(false);
}

bool MazeScene::RestartGameBegan(EventKeyboard::KeyCode keycode, Event* unused_event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_R) Director::getInstance()->replaceScene(MazeScene::createScene());
	
	return true;
}

void MazeScene::RestartGameEnded(EventKeyboard::KeyCode keycode, Event* unused_event)
{
	return;
}
