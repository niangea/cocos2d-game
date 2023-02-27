#pragma once

#include "cocos2d.h"					

using namespace cocos2d;

class Boss : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Boss);

	void StartGame(Ref* pSender);//转移至开始场景	


	void Gameset(Ref* pSender);//转移至游戏设置

	void MusicSet(Ref* pSender); //转移至音乐设置

	void Message(Ref* pSender); ///转移至游戏信息

	void loadvideo();

	void StartGame2(Ref* pSender);
};