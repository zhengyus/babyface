/*
 * KMeter4Comb.cpp
 *
 *  Created on: 2014-11-4
 *      Author: kangchao
 */

#include "KMeter4Comb.h"

KMeter4Comb::KMeter4Comb()
{
	// TODO Auto-generated constructor stub

}

KMeter4Comb::~KMeter4Comb()
{
	// TODO Auto-generated destructor stub
}

bool KMeter4Comb::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCNode::init() )
    {
        return false;
    }

    m_value = 0;


    CCSprite* lineBGI= CCSprite::createWithSpriteFrameName(IMG_LOVE);
    lineBGI->setAnchorPoint(CCPointZero);
    lineBGI->setPosition(ccp(
    		0,
    		0));

    m_line = CCSprite::createWithSpriteFrameName(IMG_LOVE_LINE);
    m_line->setAnchorPoint(CCPointZero);
    m_line->setPosition(ccp(
        	57,
        	57));

    m_line->setScaleY(float(m_value)/float(COMB_VALUE));

//    ccBlendFunc tmp_oBlendFunc = {GL_ONE, GL_ONE};
//    m_line->setBlendFunc(tmp_oBlendFunc);

    this->setAnchorPoint(CCPointZero);

    this->addChild(m_line);
    this->addChild(lineBGI);


    return true;
}

void KMeter4Comb::setValue(int value)
{
	m_value = value;

	float sv = float(m_value)/float(COMB_VALUE);

	m_line->setScaleY(sv);
}

int KMeter4Comb::getValue()
{
	return m_value;
}


