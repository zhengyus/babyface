/*
 * KMovePoint.h
 *
 *  Created on: 2014-10-19
 *      Author: Administrator
 *      移动的点
 */

#ifndef KMOVEPOINT_H_
#define KMOVEPOINT_H_

#include "cocos2d.h"
#include "Define.h"

USING_NS_CC;

class KMovePoint : public CCNode
{
public:
	KMovePoint();
	virtual ~KMovePoint();
	bool init(int type, float time, CCPointArray *arrayPos, CCNode *parent);

	// implement the "static node()" method manually
	static KMovePoint* create(int type, float time, CCPointArray *arrayPos, CCNode *parent)
	{
		KMovePoint *pRet = new KMovePoint();

		if (pRet && pRet->init(type, time, arrayPos, parent))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}

	void updateRectPos();

	CCRect m_rect;
	CCNode *m_parent;
	CCMotionStreak* m_streak;

private:
	int m_type;
	CCSprite* m_spBg;
	int m_normalWidth;
	int m_normalHeight;
};

#endif /* KMOVEPOINT_H_ */
