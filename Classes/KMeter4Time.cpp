/*
 * KMeter4Time.cpp
 *
 *  Created on: 2014-11-4
 *      Author: kangchao
 */

#include "KMeter4Time.h"

KMeter4Time::KMeter4Time()
{
	// TODO Auto-generated constructor stub

}

KMeter4Time::~KMeter4Time()
{
	// TODO Auto-generated destructor stub
}

bool KMeter4Time::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCNode::init() )
    {
        return false;
    }

    m_value = TIME_VALUE;
    
    
    CCSprite* tclock = CCSprite::createWithSpriteFrameName(IMG_CLOCK);
    tclock->setPosition(ccp(
    		tclock->getContentSize().width/2,
    		tclock->getContentSize().height/2));

    CCSprite* lineBGI= CCSprite::createWithSpriteFrameName(IMG_CLOCK_TIME_BG);
    lineBGI->setAnchorPoint(CCPointZero);
    lineBGI->setPosition(ccp(
    		tclock->getContentSize().width/2,
    		0));

    m_line = CCSprite::createWithSpriteFrameName(IMG_CLOCK_TIME_LINE);
    m_line->setAnchorPoint(CCPointZero);
    m_line->setPosition(ccp(
        	5,
        	lineBGI->getContentSize().height/2 - m_line->getContentSize().height/2));
    lineBGI->addChild(m_line);

    this->setAnchorPoint(CCPointZero);
    this->addChild(lineBGI);
    this->addChild(tclock);

     
    return true;
}

void KMeter4Time::setValue(int value)
{
	m_value = value;

	float sv = float(m_value)/float(TIME_VALUE);

	m_line->setScaleX(sv);
}

int KMeter4Time::getValue()
{
	return m_value;
}
