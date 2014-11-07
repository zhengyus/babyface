/*
 * KMainLogicLayer4Slide.cpp
 *
 *  Created on: 2014-10-31
 *      Author: kangchao
 */

#include "KMainLogicLayer4Slide.h"
#include "KManagerGameLogic.h"

USING_NS_CC;

KMainLogicLayer4Slide::KMainLogicLayer4Slide()
{
	// TODO Auto-generated constructor stub
    m_cardID = 0;
    m_cardLv = 0;
    m_selectCardLvBtnID = 0;
    m_isCanMove = false;
    m_isWinGame = false;
	m_streak = NULL;

}

KMainLogicLayer4Slide::~KMainLogicLayer4Slide()
{
	// TODO Auto-generated destructor stub
}

CCScene* KMainLogicLayer4Slide::scene(int cardID, int selectCardLvBtnID)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    KMainLogicLayer4Slide *layer = KMainLogicLayer4Slide::create(cardID, selectCardLvBtnID);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool KMainLogicLayer4Slide::init(int cardID, int selectCardLvBtnID)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    m_timeGoValue = 0.0f;
    m_cardID = cardID;
    m_score = 0;
    m_isCanPlusScore = false;
    m_isCanTouch = false;
    m_isCanPlayGame = false;
    m_selectCardLvBtnID = selectCardLvBtnID;
    m_combCurr = 0;

    srand((unsigned int)time(NULL));

    //播放背景音乐
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SOUND_BG_MUSIC, true);

    //初始化卡牌开启夜数等级信息0-3
    char tmpStr[20];
    sprintf(tmpStr, "card%dLv", m_cardID);
    m_cardLv = CCUserDefault::sharedUserDefault()->getIntegerForKey(tmpStr, T_OPEN_NUM);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(tmpStr, m_cardLv);
    CCUserDefault::sharedUserDefault()->flush();

    //初始化最大夜数1-4
	sprintf(tmpStr, "card%dMaxNum", m_cardID);
	m_maxLv = CCUserDefault::sharedUserDefault()->getIntegerForKey(tmpStr, T_MAX_NUM);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(tmpStr, m_maxLv);
	CCUserDefault::sharedUserDefault()->flush();

    //容错处理，id不能大于没开启的关卡值
    if(m_selectCardLvBtnID > m_cardLv)
    {
    	m_selectCardLvBtnID = m_cardLv;
    }

    //初始化时钟时间
	sprintf(tmpStr, "card%dLv%dTime", m_cardID, m_selectCardLvBtnID);
	m_timeValue = CCUserDefault::sharedUserDefault()->getFloatForKey(tmpStr, D_TIME);
	CCUserDefault::sharedUserDefault()->setFloatForKey(tmpStr, m_timeValue);
	CCUserDefault::sharedUserDefault()->flush();

	//初始化comb最大值
	sprintf(tmpStr, "card%dLv%dComb", m_cardID, m_selectCardLvBtnID);
	m_combMax = CCUserDefault::sharedUserDefault()->getIntegerForKey(tmpStr, D_COMB);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(tmpStr, m_combMax);
	CCUserDefault::sharedUserDefault()->flush();

//    this->schedule(schedule_selector(KMainLogicLayer4Slide::updateScore), 0.1f);

    m_soundLine = KSoundLine::create();
    m_soundLine->setPosition(ccp(760, 630));
    m_soundLine->setStop(true);


    CCSprite* spBG = CCSprite::createWithSpriteFrameName(IMG_BG);
    spBG->setAnchorPoint(CCPointZero);
    spBG->setPosition(CCPointZero);


    this->setAnchorPoint(CCPointZero);
    this->setPosition(CCPointZero);
    this->setScaleX(KRateX);
    this->setScaleY(KRateY);

    this->addChild(spBG);
    this->addChild(m_soundLine);

    //美女图(遮罩元素相关)
    //按钮遮罩
    m_cliperNode = CCNode::create();
    m_cliperNode->setAnchorPoint(CCPointZero);
    m_cliperNode->setPosition(CCPointZero);

	CCSprite* spCardImgBG;
	sprintf(tmpStr, "card%dLv%dBG.png", m_cardID, m_selectCardLvBtnID);
	spCardImgBG = CCSprite::createWithSpriteFrameName(tmpStr);
	spCardImgBG->setPosition(ccp(YS_SCREEN_W/2, YS_SCREEN_H/2));
	spCardImgBG->setTag(TAG_IMG + m_selectCardLvBtnID);

	m_cliperNode->addChild(spCardImgBG);

	CCSprite* spCardImg;
	sprintf(tmpStr, "card%dLv%d.png", m_cardID, m_selectCardLvBtnID);
	spCardImg = CCSprite::createWithSpriteFrameName(tmpStr);
	spCardImg->setTag(TAG_IMG + m_selectCardLvBtnID);
	spCardImg->setPosition(ccp(YS_SCREEN_W/2, YS_SCREEN_H/2));

	CCClippingNode* tClipper =  CCClippingNode::create();
	tClipper->setInverted(true);
	tClipper->setAnchorPoint(CCPointZero);
   	tClipper->setPosition(CCPointZero);
   	tClipper->setAlphaThreshold(0.0f);//此处一定要加，否则是矩形
   	tClipper->setTag(TAG_CLIPER);

   	m_stencilNode = CCNode::create();
   	tClipper->setStencil(m_stencilNode);
   	tClipper->addChild(spCardImg);

   	m_cliperNode->addChild(tClipper);

    //开始初始化各种按钮
    m_widget = UILayer::create();
    m_widget->setAnchorPoint(CCPointZero);
    m_widget->setPosition(CCPointZero);
    //返回按钮
    UIButton * btnBack = UIButton::create();
    btnBack->loadTextureNormal(IMG_BACK_BTN, UI_TEX_TYPE_PLIST);
    btnBack->loadTexturePressed(IMG_BACK_BTN, UI_TEX_TYPE_PLIST);
    btnBack->loadTextureDisabled(IMG_BACK_BTN, UI_TEX_TYPE_PLIST);
    btnBack->setAnchorPoint(CCP_CENTER);
    btnBack->setPosition(ccp(
    		btnBack->getContentSize().width/2,
    		btnBack->getContentSize().height/2));
    btnBack->addTouchEventListener(this, toucheventselector(KMainLogicLayer4Slide::touchEvent));
    btnBack->setTouchEnabled(true);
    btnBack->setTag(TAG_BTN_BACK);

    //第几夜按钮
    UIButton * btnLvl[m_maxLv];

    for(int i = 0; i < m_maxLv; i++)
    {
    	btnLvl[i] = UIButton::create();
    	btnLvl[i]->setTouchEnabled(true);

    	//大于当前等级不开启
    	if(i > m_cardLv)
    	{
    		btnLvl[i]->setTouchEnabled(false);
    		btnLvl[i]->loadTextureNormal(IMG_CLOSE_LVL_UNENABLE_BTN, UI_TEX_TYPE_PLIST);
    		btnLvl[i]->loadTexturePressed(IMG_CLOSE_LVL_UNENABLE_BTN, UI_TEX_TYPE_PLIST);
    		btnLvl[i]->loadTextureDisabled(IMG_CLOSE_LVL_UNENABLE_BTN, UI_TEX_TYPE_PLIST);
    	}
    	else
    	{
    		btnLvl[i]->loadTextureNormal(IMG_CLOSE_LVL_BTN, UI_TEX_TYPE_PLIST);
    		btnLvl[i]->loadTexturePressed(IMG_CLOSE_LVL_BTN, UI_TEX_TYPE_PLIST);
    		btnLvl[i]->loadTextureDisabled(IMG_CLOSE_LVL_BTN, UI_TEX_TYPE_PLIST);
    	}

    	//注意位置初始化的顺序，防止图片大小不一导致顺序混乱
    	btnLvl[i]->setAnchorPoint(CCP_CENTER);
    	    	btnLvl[i]->setPosition(ccp(
    	    			YS_SCREEN_W - btnLvl[i]->getContentSize().width/2 - 50,
    	    			YS_SCREEN_H - btnLvl[i]->getContentSize().height*i - BTN_Y_VALUE*i - BTN_Y_BEGIN_VALUE));

    	if(i == m_selectCardLvBtnID)
    	{
    		btnLvl[i]->loadTextureNormal(IMG_OPEN_LVL_BTN, UI_TEX_TYPE_PLIST);
    		btnLvl[i]->loadTexturePressed(IMG_OPEN_LVL_BTN, UI_TEX_TYPE_PLIST);
    		btnLvl[i]->loadTextureDisabled(IMG_OPEN_LVL_BTN, UI_TEX_TYPE_PLIST);
    	}

    	btnLvl[i]->addTouchEventListener(this, toucheventselector(KMainLogicLayer4Slide::touchEvent));
    	btnLvl[i]->setTag(TAG_BTN_LVL + i);
    	m_widget->addWidget(btnLvl[i]);
    }

    m_widget->addWidget(btnBack);

    //粒子特效
    m_patBatchNode = CCParticleBatchNode::createWithTexture(CCSprite::create(IMG_CLIPE_PAR)->getTexture());
    m_patBatchNode->setAnchorPoint(CCPointZero);
    m_patBatchNode->setPosition(CCPointZero);

    //添加时钟
    m_meterTime = KMeter4Time::create();
    m_meterTime->setPosition(ccp(20, 650));
    
    //初始化标签
    //秒数标签
    m_pLabel = CCLabelTTF::create("", "Arial", 24);
    m_pLabel->setAnchorPoint(CCPointZero);
    m_pLabel->setPosition(ccp(
                              m_meterTime->getPositionX(),
                              m_meterTime->getPositionY() - 24));
    sprintf(tmpStr, "%.2fs", m_timeValue);
    m_pLabel->setString(tmpStr);
    
    //失败标签
    m_pLabel4Lose = CCLabelTTF::create("YOU LOSE!", "Arial", 100);
    m_pLabel4Lose->setPosition(ccp(YS_SCREEN_W/2, YS_SCREEN_H + 50));
    
    //胜利标签
    m_pLabel4Win = CCLabelTTF::create("YOU WIN!", "Arial", 100);
    m_pLabel4Win->setPosition(ccp(YS_SCREEN_W/2, YS_SCREEN_H/2));
    m_pLabel4Win->setVisible(false);
    m_pLabel4Win->setScale(0);

    //初始化comb
    m_meterComb = KMeter4Comb::create();
    m_meterComb->setPosition(ccp(30, 100));

    this->addChild(m_cliperNode);
    this->addChild(m_widget);
    this->addChild(m_meterTime);
    this->addChild(m_meterComb);
    this->addChild(m_pLabel);
    this->addChild(m_patBatchNode);
    this->addChild(m_pLabel4Lose);
    this->addChild(m_pLabel4Win);

    return true;
}

void KMainLogicLayer4Slide::onEnter()
{
	CCLayer::onEnter();
}

void KMainLogicLayer4Slide::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
	m_isCanTouch = true;
	m_isCanPlayGame = true;
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 128, true);
	this->setTouchEnabled(true);
    
    //开启时钟计时
	this->schedule(schedule_selector(KMainLogicLayer4Slide::updateTime), 0.0f);

}

void KMainLogicLayer4Slide::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	m_isCanTouch = false;
	m_isCanPlayGame = false;
	this->setTouchEnabled(false);
	CCLayer::onExit();
}

void KMainLogicLayer4Slide::touchEvent(CCObject* pSender, TouchEventType type)
{
	if(!m_isCanTouch)
	{
		return;
	}

	switch (type)
	{
		case cocos2d::ui::TOUCH_EVENT_BEGAN:
		{
			break;
		}
		case cocos2d::ui::TOUCH_EVENT_ENDED:
		{
			int tTag = ((UILayer*)pSender)->getTag();

			switch(tTag)
			{
				case TAG_BTN_BACK:
				{
					break;
				}
				case TAG_BTN_LVL + 0:
				{
					if(tTag - TAG_BTN_LVL != m_selectCardLvBtnID)
					{
						updateLvBtn(tTag - TAG_BTN_LVL);
					}

					break;
				}
				case TAG_BTN_LVL + 1:
				{
					if(tTag - TAG_BTN_LVL != m_selectCardLvBtnID)
					{
						updateLvBtn(tTag - TAG_BTN_LVL);
					}

					break;
				}
				case TAG_BTN_LVL + 2:
				{
					if(tTag - TAG_BTN_LVL != m_selectCardLvBtnID)
					{
						updateLvBtn(tTag - TAG_BTN_LVL);
					}

					break;
				}
				case TAG_BTN_LVL + 3:
				{
					if(tTag - TAG_BTN_LVL != m_selectCardLvBtnID)
					{
						updateLvBtn(tTag - TAG_BTN_LVL);
					}

					break;
				}
				case TAG_BTN_LVL + 4:
				{
					break;
				}
				default:
				{
					break;
				}
			}

			break;
		}
		default:
		{
			break;
		}
	}
}

bool KMainLogicLayer4Slide::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
	if(!m_isCanTouch)
	{
		return false;
	}
    
    if(!m_isCanPlayGame)
    {

    	m_isCanTouch = false;

    	//失败情况
    	if(!m_isWinGame)
    	{
			updateLvBtn(m_selectCardLvBtnID);//重新开始
    	}
    	//胜利情况
    	else
    	{
    		nextLv();//胜利情况进入下一夜
    	}

    	return false;
    }
    
	m_isCanTouch = false;
	CCPoint location = touch->getLocation();
	location = this->convertToNodeSpace(location);
	m_begin = location;

	//初始化刀光滑条
	//-渐隐时间 -渐隐片段大小  -贴图长宽 -颜色 -图片
	if(!m_streak)
	{
		m_streak = CCMotionStreak::create(0.1f, 0.5f, 10, ccBLACK, IMG_MOVE_POINT_BG);
		m_streak->setFastMode(true);
		m_streak->setPosition(location);
		this->addChild(m_streak);
	}

	return true;
}

void KMainLogicLayer4Slide::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    if(!m_isCanPlayGame)
    {
    	if(m_streak)
    	{
    		this->removeChild(m_streak);
    		m_streak = NULL;
    	}

//        m_isCanTouch = true;

        moveByNodeCliper(false);
        return;
    }
    
	CCPoint location = touch->getLocation();
	location = this->convertToNodeSpace(location);

	if(m_streak)
	{
		m_streak->setPosition(location);

		CCSprite *spImg = (CCSprite*)m_cliperNode->getChildByTag(TAG_IMG + m_selectCardLvBtnID);

		CCRect tR;
		tR.origin = ccp(spImg->getPositionX() - spImg->getContentSize().width/2,
				spImg->getPositionY() - spImg->getContentSize().height/2);
		tR.size = spImg->getContentSize();

		if(ccpDistance(m_begin, location) > MOVE_DIST)
		{
			if(tR.containsPoint(location))
			{
				//求旋转角度
				CCPoint v = ccpSub(location, m_begin);
				float rad = ccpAngle(v, ccp(-1, 0));
				float deg = CC_RADIANS_TO_DEGREES(rad);  // 弧度转角度

				if(location.y < m_begin.y)
				{
					deg = -deg;
				}

				moveByNodeCliper(true);

				CCSprite* tSp = CCSprite::createWithSpriteFrameName(IMG_CLIPE_LINE);
				tSp->setPosition(location);
				tSp->setRotation(deg);

				int max = 3;
				int min = 1;
				int rd = rand() % (max - min + 1) + min;

				tSp->setScaleX(rd);

				m_stencilNode->addChild(tSp);
				m_begin = location;

				SimpleAudioEngine::sharedEngine()->playEffect(SOUND_CLIP);

				CCParticleSystemQuad* ptc = CCParticleSystemQuad::create(PLIST_PAR);
				ptc->setPosition(location);
				ptc->setPositionType(kCCPositionTypeFree);
				ptc->setAutoRemoveOnFinish(true);
			    m_patBatchNode->addChild(ptc);

			    //更新comb meter
			    m_combCurr++;

			    if(m_combCurr > m_combMax)
			    {
			    	m_combCurr = m_combMax;
			    }

			    int p = (m_combCurr*100)/m_combMax;

			    //comb值满通关 胜利了~~~
			    if(100 == p)
			    {
			    	m_isWinGame = true;
			    	m_isCanTouch = false;
					m_isCanPlayGame = false;
					CCParticleSystemQuad* ptc = CCParticleSystemQuad::create(PLIST_PAR_COMB);
					ptc->setPosition(ccp(YS_SCREEN_W/2, YS_SCREEN_H/2));
					ptc->setPositionType(kCCPositionTypeFree);
					ptc->setAutoRemoveOnFinish(true);
					m_patBatchNode->addChild(ptc);

					m_cliperNode->getChildByTag(TAG_CLIPER)->setVisible(false);

					this->unschedule(schedule_selector(KMainLogicLayer4Slide::updateTime));

					//播放文字动画
					m_pLabel4Win->setVisible(true);
					m_pLabel4Win->runAction(CCSequence::create(
							CCScaleTo::create(0.1f, 1.0f),
							CCDelayTime::create(1.0f),
							CCFadeOut::create(3.0f),
							CCCallFunc::create(this, callfunc_selector(KMainLogicLayer4Slide::setIsCanTouch)),
                            NULL));
			    }

			    m_meterComb->setValue(p);
			}
		}
	}
}

void KMainLogicLayer4Slide::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    if(!m_isCanPlayGame)
    {
        return;
    }
    
	this->removeChild(m_streak);
	m_streak = NULL;
	m_isCanTouch = true;

	moveByNodeCliper(false);
}

void KMainLogicLayer4Slide::updateLvBtn(int currSelectID)
{
	/*
	m_isCanPlayGame = false;

	CCScene* pScene = KManagerGameLogic::shareKManagerGameLogic()->gameScene4Type(
	    		m_cardID,
	    		currSelectID);

	CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));

	*/
	this->unschedule(schedule_selector(KMainLogicLayer4Slide::updateTime));
	m_isCanPlayGame = false;

	runVisibleFalseAction(m_cliperNode);

	this->runAction(CCSequence::create(
			CCDelayTime::create(TIME_4_FADEOUT),
			CCCallFuncND::create(this, callfuncND_selector(KMainLogicLayer4Slide::runBtn), (void*)currSelectID),
			NULL));
}

//一定要加上CCNode *pSender参数，否则后面的void *currSelectID不执行
void KMainLogicLayer4Slide::runBtn(CCNode *pSender, void *currSelectID)
{
	m_cliperNode->setVisible(false);

	CCScene* pScene = KManagerGameLogic::shareKManagerGameLogic()->gameScene4Type(
			m_cardID,
			int(currSelectID));

	CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));

}

void KMainLogicLayer4Slide::visit()
{
    CCLayer::visit();
}

void KMainLogicLayer4Slide::nextLv()
{
	int tmpCurrLv = m_selectCardLvBtnID + 1;

	if(m_cardLv < tmpCurrLv && tmpCurrLv < m_maxLv)
	{
		m_isCanTouch = false;

		//更新按钮选择信息
		char tmpStr[20];
		sprintf(tmpStr, "card%dLv", m_cardID);
		CCUserDefault::sharedUserDefault()->setIntegerForKey(tmpStr, tmpCurrLv);
		CCUserDefault::sharedUserDefault()->flush();
		m_cardLv = CCUserDefault::sharedUserDefault()->getIntegerForKey(tmpStr);

		updateLvBtn(tmpCurrLv);
	}
	else
	{
		m_isCanTouch = true;
	}
}

void KMainLogicLayer4Slide::moveByNodeCliper(bool isCanMove)
{
	if(m_isCanMove != isCanMove)
	{
		m_isCanMove = isCanMove;

		if(m_isCanMove)
		{
			m_cliperNode->runAction(CCRepeatForever::create(CCSequence::create(
					CCMoveBy::create(0.05f, ccp(10,0)),
					CCMoveBy::create(0.05f, ccp(-10,0)),
					NULL)));

			this->scheduleOnce(schedule_selector(KMainLogicLayer4Slide::stopNodeCliper), 0.2f);
		}
		else
		{
			m_cliperNode->stopAllActions();
			m_cliperNode->setPosition(CCPointZero);
		}
	}
}

void KMainLogicLayer4Slide::stopNodeCliper()
{
	moveByNodeCliper(false);
}

void KMainLogicLayer4Slide::runVisibleFalseAction(CCNode* node)
{
	int offSetX = m_cliperNode->getChildByTag(TAG_IMG + m_selectCardLvBtnID)->getContentSize().width/2;

	node->runAction(CCMoveBy::create(TIME_4_FADEOUT, ccp(-YS_SCREEN_W/2 - offSetX, 0)));

	/*
	CCObject* tObj = NULL;
	CCArray* nodeArr = node->getChildren();

	CCARRAY_FOREACH(nodeArr, tObj)
	{
		CCNode* tn = (CCNode*)tObj;

		if(tn->isRunning())
		{
			tn->runAction(CCFadeOut::create(TIME_4_FADEOUT));
			runVisibleFalseAction(tn);
		}
	}
	*/
}

void KMainLogicLayer4Slide::updateTime(float dt)
{
    char tmpStr[30];
	m_timeGoValue += dt;
    
	float subTime = m_timeValue - m_timeGoValue;
    
    //时间到 失败了~~~
	if(subTime < 0)
	{
		subTime = 0;
        m_isCanTouch = false;
        m_isCanPlayGame = false;
        m_pLabel4Lose->runAction(CCSequence::create(CCEaseBounceOut::create(CCMoveTo::create(TIME_BOUDLE, ccp(m_pLabel4Lose->getPositionX(), YS_SCREEN_H/2))),
                                                    CCCallFunc::create(this, callfunc_selector(KMainLogicLayer4Slide::setIsCanTouch)),
                                                    NULL));
        
        this->unschedule(schedule_selector(KMainLogicLayer4Slide::updateTime));
	}
    
    sprintf(tmpStr, "%.2fs", subTime);
    m_pLabel->setString(tmpStr);
    
	float p = subTime/m_timeValue;
	int tValue = p*100;
	m_meterTime->setValue(tValue);
    
}

void KMainLogicLayer4Slide::setIsCanTouch()
{
    m_isCanTouch = true;
}
