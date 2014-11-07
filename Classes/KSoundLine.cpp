/*
 * KSoundLine.cpp
 *
 *  Created on: 2014-10-11
 *      Author: kangchao
 */

#include "KSoundLine.h"

KSoundLine::KSoundLine()
{
	// TODO Auto-generated constructor stub
	m_arrLines = NULL;
	m_arrLines = CCArray::create();
	CC_SAFE_RETAIN(m_arrLines);

}

KSoundLine::~KSoundLine()
{
	// TODO Auto-generated destructor stub
	CC_SAFE_RELEASE(m_arrLines);
}

// on "init" you need to initialize your instance
bool KSoundLine::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCNode::init() )
    {
        return false;
    }

    srand((unsigned int)time(NULL));

    m_isRunStop = false;
    m_isRunStopCurr = !m_isRunStop;

    //初始化线
    int lineWidthNum = 0;

    for(int i = 0; i < MAX_LINE_NUMS; i++)
    {
    	CCSprite* tmpSp = CCSprite::createWithSpriteFrameName(IMG_SOUND_LINE);
    	m_arrLines->addObject(tmpSp);
    	tmpSp->setAnchorPoint(CCPointZero);
    	tmpSp->setPosition(ccp(0 + (tmpSp->getContentSize().width + 3)*i, 0));
    	this->addChild(tmpSp);

    	lineWidthNum = i;
    }

    m_rect.size.height = ((CCSprite*)m_arrLines->objectAtIndex(0))->getContentSize().height;
    m_rect.size.width = (((CCSprite*)m_arrLines->objectAtIndex(0))->getContentSize().width + 3)*lineWidthNum;
    m_rect.origin = CCPointZero;

    this->schedule(schedule_selector(KSoundLine::updateIsRun), 0.0f);
    this->setAnchorPoint(CCPointZero);

    return true;
}

void KSoundLine::setStop(bool isRunStop)
{
	m_isRunStop = isRunStop;
}

void KSoundLine::updateIsRun()
{
	if(m_isRunStopCurr != m_isRunStop)
	{
		m_isRunStopCurr = m_isRunStop;

		if(m_isRunStopCurr)
		{
			for(int i = 0; i < m_arrLines->count(); i++)
			{
				CCSprite*tmpSp = (CCSprite*)m_arrLines->objectAtIndex(i);
				tmpSp->stopAllActions();
				tmpSp->runAction(CCScaleTo::create(0.1f, 1.0f));
			}
		}
		else
		{
			runLine();
		}
	}
}

void KSoundLine::runLine()
{
	int tmpScale = 1;
	int retScale = 1;
	int scalePlus = 2;
	int scaleOffset = 3;

	for(int i = 0; i < m_arrLines->count(); i++)
	{
		if(i <= m_arrLines->count()/2)
		{
			tmpScale += scalePlus;

			int min = tmpScale - scaleOffset;
			int max = tmpScale + scaleOffset;
			retScale = rand()%(max - min + 1) + min;

		}
		else
		{
			tmpScale -= scalePlus;

			int min = tmpScale - scaleOffset;

			if(min <= 0)
			{
				min = 1;
			}

			int max = tmpScale + scaleOffset;
			retScale = rand()%(max - min + 1) + min;

		}

		CCSprite*tmpSp = (CCSprite*)m_arrLines->objectAtIndex(i);
		tmpSp->stopAllActions();

		tmpSp->runAction(CCSequence::create(
				CCScaleTo::create(0.1f, 1.0f, retScale),
				CCCallFunc::create(this, callfunc_selector(KSoundLine::runLine)),
				NULL));
	}
}
