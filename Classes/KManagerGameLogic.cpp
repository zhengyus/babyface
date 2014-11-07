/*
 * KManagerGameLogic.cpp
 *
 *  Created on: 2014-10-30
 *      Author: kangchao
 */

#include "KMainLogicLayer.h"
#include "KManagerGameLogic.h"
#include "KMainLogicLayer4Slide.h"

static KManagerGameLogic* g_kManagerGameLogic = NULL;

KManagerGameLogic::KManagerGameLogic()
{
	// TODO Auto-generated constructor stub

}

KManagerGameLogic::~KManagerGameLogic()
{
	// TODO Auto-generated destructor stub
}

//连个参数都是从0计数开始
CCScene* KManagerGameLogic::gameScene4Type(int cardID, int Lv)
{
	CCScene *s = NULL;

	//获取关卡游戏类型
	int type;
	char tmpStr[50];
	sprintf(tmpStr, "card%dLv%dType", cardID, Lv);
	type = CCUserDefault::sharedUserDefault()->getIntegerForKey(tmpStr, TYPE_GAME_LOGIC_SLIDER);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(tmpStr, type);
	CCUserDefault::sharedUserDefault()->flush();

	switch(type)
	{
		case TYPE_GAME_LOGIC_MOVE_POINT:
		{
			s = KMainLogicLayer::scene(cardID, Lv);
			break;
		}
		case TYPE_GAME_LOGIC_SLIDER:
		{
			s = KMainLogicLayer4Slide::scene(cardID, Lv);
			break;
		}
		default:
		{
			CCLog("没有s");
			break;
		}
	}

	return s;
}

KManagerGameLogic* KManagerGameLogic::shareKManagerGameLogic()
{
	if(g_kManagerGameLogic == NULL)
	{
		g_kManagerGameLogic = new KManagerGameLogic;
	}

	return g_kManagerGameLogic;
}
