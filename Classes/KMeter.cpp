/*
 * KMeter.cpp
 *
 *  Created on: 2014-10-12
 *      Author: Administrator
 */

#include "KMeter.h"

KMeter::KMeter()
{
	// TODO Auto-generated constructor stub

}

KMeter::~KMeter()
{
	// TODO Auto-generated destructor stub
}

// on "init" you need to initialize your instance
bool KMeter::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCNode::init() )
    {
        return false;
    }

    m_value = 0;

    CCSprite* spMeter = CCSprite::createWithSpriteFrameName(IMG_CUP_BG);
    spMeter->setPosition(ccp(
    		spMeter->getContentSize().width/2,
    		spMeter->getContentSize().height/2));
    spMeter->setAnchorPoint(ccp(0.5f, 0.5f));

    m_pin = UILoadingBar::create();
    m_pin->loadTexture(IMG_CUP_FONT, UI_TEX_TYPE_PLIST);
    m_pin->setAnchorPoint(ccp(0.5f, 0.5f));
    m_pin->setPosition(ccp(
    		spMeter->getContentSize().width/2,
    		spMeter->getContentSize().height/2
    		));
    m_pin->setRotation(-90);

    m_rect.size = spMeter->getContentSize();
    m_rect.origin = CCPointZero;

    m_widget = UILayer::create();
    m_widget->setAnchorPoint(CCPointZero);
    m_widget->setPosition(CCPointZero);

    m_widget->addWidget(m_pin);

    this->setAnchorPoint(CCPointZero);
    this->addChild(spMeter);
    this->addChild(m_widget);

    return true;
}

void KMeter::setValue(float value)
{
	m_value = value;
	m_pin->setPercent(m_value);
}

