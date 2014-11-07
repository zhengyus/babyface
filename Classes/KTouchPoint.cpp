/*
 * KTouchPoint.cpp
 *
 *  Created on: 2014-10-15
 *      Author: kangchao
 */

#include "KTouchPoint.h"

KTouchPoint::KTouchPoint()
{
	// TODO Auto-generated constructor stub
	m_type = 0;
}

KTouchPoint::~KTouchPoint()
{
	// TODO Auto-generated destructor stub
}

// on "init" you need to initialize your instance
bool KTouchPoint::init(int type, CCPoint pos)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCNode::init() )
    {
        return false;
    }

    m_type = type;

    
    m_rect.size.width = 50;
	m_rect.size.height = 50;
    
    this->setAnchorPoint(CCPointZero);
    this->setPosition(ccp(pos.x - m_rect.size.width/2,
                          pos.y - m_rect.size.height/2));
    
    m_rect.origin = this->getPosition();


	m_circle0 = CCSprite::createWithSpriteFrameName(IMG_TOUCH_POINT_BG);
	m_circle1 = CCSprite::createWithSpriteFrameName(IMG_TOUCH_POINT_BG);
	m_circle0->setPosition(ccp(m_rect.size.width/2, m_rect.size.height/2));
	m_circle1->setPosition(ccp(m_rect.size.width/2, m_rect.size.height/2));

	m_bigCircle0 = CCSprite::createWithSpriteFrameName(IMG_TOUCH_POINT_BG);
	m_bigCircle1 = CCSprite::createWithSpriteFrameName(IMG_TOUCH_POINT_BG);
	m_bigCircle0->setPosition(ccp(m_rect.size.width/2, m_rect.size.height/2));
	m_bigCircle1->setPosition(ccp(m_rect.size.width/2, m_rect.size.height/2));
	m_bigCircle0->setVisible(false);
	m_bigCircle1->setVisible(false);

	this->scheduleOnce(schedule_selector(KTouchPoint::runCircle0), 0.0f);
	this->scheduleOnce(schedule_selector(KTouchPoint::runCircle1), ACTION_TIME/2);
	this->scheduleOnce(schedule_selector(KTouchPoint::runBigCircle0), 0.0f);
	this->scheduleOnce(schedule_selector(KTouchPoint::runBigCircle1), ACTION_TIME/4);

    m_spBg = CCSprite::createWithSpriteFrameName(IMG_TOUCH_POINT);
    m_spBg->setPosition(ccp(m_rect.size.width/2, m_rect.size.height/2));

    this->addChild(m_circle0);
    this->addChild(m_circle1);
    this->addChild(m_bigCircle0);
    this->addChild(m_bigCircle1);
    this->addChild(m_spBg);

    this->schedule(schedule_selector(KTouchPoint::updateRectPos), 0.0f);

    return true;
}

void KTouchPoint::updateRectPos()
{
	m_rect.origin = this->getPosition();
}

void KTouchPoint::runCircle0()
{
	m_circle0->runAction(CCRepeatForever::create(CCFadeOut::create(ACTION_TIME)));

	m_circle0->runAction(CCRepeatForever::create(CCSequence::create(
			CCScaleTo::create(ACTION_TIME, ACTION_SCALE),
			CCCallFuncN::create(this, callfuncN_selector(KTouchPoint::resetCircle)),
			NULL)));

}

void KTouchPoint::runCircle1()
{
	m_circle1->runAction(CCRepeatForever::create(CCFadeOut::create(ACTION_TIME)));

	m_circle1->runAction(CCRepeatForever::create(CCSequence::create(
			CCScaleTo::create(ACTION_TIME, ACTION_SCALE),
			CCCallFuncN::create(this, callfuncN_selector(KTouchPoint::resetCircle)),
			NULL)));

}

void KTouchPoint::runBigCircle0()
{
	m_bigCircle0->runAction(CCRepeatForever::create(CCFadeOut::create(ACTION_TIME/2)));

	m_bigCircle0->runAction(CCRepeatForever::create(CCSequence::create(
			CCScaleTo::create(ACTION_TIME/2, ACTION_SCALE*2),
			CCCallFuncN::create(this, callfuncN_selector(KTouchPoint::resetCircle)),
			NULL)));

}

void KTouchPoint::runBigCircle1()
{
	m_bigCircle1->runAction(CCRepeatForever::create(CCFadeOut::create(ACTION_TIME/2)));

	m_bigCircle1->runAction(CCRepeatForever::create(CCSequence::create(
			CCScaleTo::create(ACTION_TIME/2, ACTION_SCALE*2),
			CCCallFuncN::create(this, callfuncN_selector(KTouchPoint::resetCircle)),
			NULL)));

}

void KTouchPoint::resetCircle(CCNode *np)
{
	((CCSprite*)np)->setScale(1.0f);
	((CCSprite*)np)->setOpacity(255);
}

void KTouchPoint::setCircleBig(bool isCanBig)
{
	if(isCanBig)
	{
		m_circle0->setVisible(false);
		m_circle1->setVisible(false);
		m_bigCircle0->setVisible(true);
		m_bigCircle1->setVisible(true);
	}
	else
	{
		m_circle0->setVisible(true);
		m_circle1->setVisible(true);
		m_bigCircle0->setVisible(false);
		m_bigCircle1->setVisible(false);
	}
}
