/*
 * KMovePoint.cpp
 *
 *  Created on: 2014-10-19
 *      Author: Administrator
 */

#include "KMovePoint.h"

KMovePoint::KMovePoint()
{
	// TODO Auto-generated constructor stub

}

KMovePoint::~KMovePoint()
{
	// TODO Auto-generated destructor stub
}

bool KMovePoint::init(int type, float time, CCPointArray *arrayPos, CCNode *parent)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCNode::init() )
    {
        return false;
    }

    m_type = type;
    m_parent = parent;

	m_normalWidth = 50;
	m_normalHeight = 50;

    this->setAnchorPoint(CCPointZero);
    this->setPosition(arrayPos->getControlPointAtIndex(0));
    m_rect.origin = this->getPosition();
	m_rect.size.width = m_normalWidth;
	m_rect.size.height = m_normalHeight;

    m_spBg = CCSprite::createWithSpriteFrameName(IMG_MOVE_POINT);
    m_spBg->setPosition(ccp(m_rect.size.width/2, m_rect.size.height/2));

    //-渐隐时间 -渐隐片段大小  -贴图长宽 -颜色 -图片
    m_streak = CCMotionStreak::create(1.0f, 0.1f, 72, ccWHITE, IMG_MOVE_POINT_BG);
    m_streak->setFastMode(true);
    m_streak->setPosition(ccp(m_rect.origin.x + m_rect.size.width/2,
    		m_rect.origin.y + m_rect.size.height/2));
    m_parent->addChild(m_streak);

    this->addChild(m_spBg);

    this->schedule(schedule_selector(KMovePoint::updateRectPos), 0.0f);

	CCCatmullRomTo *action = CCCatmullRomTo::create(time, arrayPos);
	this->runAction(CCRepeatForever::create(action));

    return true;
}

void KMovePoint::updateRectPos()
{
	m_rect.origin = this->getPosition();
	m_rect.size.width = m_normalWidth*this->m_fScaleX;
	m_rect.size.height = m_normalHeight*this->m_fScaleY;

	m_streak->setPosition(ccp(m_rect.origin.x + m_rect.size.width/2,
	    		m_rect.origin.y + m_rect.size.height/2));
}


