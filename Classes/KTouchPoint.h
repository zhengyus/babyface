/*
 * KTouchPoint.h
 *
 *  Created on: 2014-10-15
 *      Author: kangchao
 *      触摸的点
 */

#ifndef KTOUCHPOINT_H_
#define KTOUCHPOINT_H_

#include "cocos2d.h"
#include "Define.h"

#define ACTION_TIME 1.6f
#define ACTION_SCALE 10.0f

USING_NS_CC;

class KTouchPoint : public CCNode
{
public:
	KTouchPoint();
	virtual ~KTouchPoint();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	bool init(int type, CCPoint pos);

	void runCircle0();
	void runCircle1();
	void runBigCircle0();
	void runBigCircle1();
	void resetCircle(CCNode *np);
	// implement the "static node()" method manually
	static KTouchPoint* create(int type, CCPoint pos)
	{
		KTouchPoint *pRet = new KTouchPoint();

		if (pRet && pRet->init(type, pos))
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
	void setCircleBig(bool isCanBig);

	CCRect m_rect;

private:

	CCSprite* m_spBg;

	CCSprite* m_circle0;
	CCSprite* m_circle1;

	CCSprite* m_bigCircle0;
	CCSprite* m_bigCircle1;
	int m_type;//拓展用，以后可能会出现多种点逻辑0-默认点 1-还没有 2-还没有
};

#endif /* KTOUCHPOINT_H_ */
