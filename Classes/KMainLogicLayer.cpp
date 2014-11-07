//
//  KMainLogicLayer.cpp
//  BitBattle
//
//  Created by kc on 14-9-15.
//
//

#include "KMainLogicLayer.h"
#include "KManagerGameLogic.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "K4IOSAudioEngine.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif

USING_NS_CC;

KMainLogicLayer::KMainLogicLayer()
{
	// TODO Auto-generated constructor stub
    m_cardID = 0;
    m_cardLv = 0;
    m_selectCardLvBtnID = 0;
	m_vRectDate.clear();
	m_vLvArrPoint.clear();
	m_dic.removeAllObjects();
	m_effectID = -1;
	m_isInBug = false;
	m_isCanScale = false;

	m_movePointTime[0] = 6.0f;
	m_movePointTime[1] = 3.0f;
	m_movePointTime[2] = 2.0f;

	m_arrRect = NULL;
	m_arrRect = CCArray::create();
	CC_SAFE_RETAIN(m_arrRect);
    
    m_costTime = 0.0f;

	//test
#if(DEBUG_RECT == 1)
	m_vTest.clear();
#endif
}

KMainLogicLayer::~KMainLogicLayer()
{
	// TODO Auto-generated destructor stub
	CC_SAFE_RELEASE(m_arrRect);

	//清除移动点数据
	std::vector<CCPointArray*> vArrPointTmp;

	for(int i = 0; i < m_vLvArrPoint.size(); i++)
	{
		vArrPointTmp = m_vLvArrPoint[i];

		for(int j = 0; j < vArrPointTmp.size(); j++)
		{
			vArrPointTmp[j]->release();
		}

		vArrPointTmp.clear();
	}

	m_vLvArrPoint.clear();

	//清除触摸点
	for(int i = 0; i < m_vRectDate.size(); i++)
	{
		m_vRectDate[i].m_movePoint->m_parent->removeChild(m_vRectDate[i].m_movePoint->m_streak);
		this->removeChild(m_vRectDate[i].m_movePoint);
	}

	m_vRectDate.clear();
}

CCScene* KMainLogicLayer::scene(int cardID, int selectCardLvBtnID)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    KMainLogicLayer *layer = KMainLogicLayer::create(cardID, selectCardLvBtnID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool KMainLogicLayer::init(int cardID, int selectCardLvBtnID)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    m_cardID = cardID;
    m_score = 0;
    m_isCanPlusScore = false;
    m_isCanPlaySound = true;
    m_isCanTouch = false;
    m_isCanPlayGame = false;
    m_selectCardLvBtnID = selectCardLvBtnID;
    
    //播放背景音乐
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SOUND_BG_MUSIC, true);

    //初始化卡牌开启夜数等级信息0-3
    char tmpStr[30];
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
    
    //初始化最短时间数据
    sprintf(tmpStr, "card%dLv%dMinTime", m_cardID, m_selectCardLvBtnID);
    m_minTime = CCUserDefault::sharedUserDefault()->getFloatForKey(tmpStr, T_MIN_TIME);
	CCUserDefault::sharedUserDefault()->setFloatForKey(tmpStr, m_minTime);
	CCUserDefault::sharedUserDefault()->flush();

    
    m_pLabel = CCLabelTTF::create("0%", "Arial", 24);
    m_pLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    
    sprintf(tmpStr, "耗时: %.2fs", m_costTime);
    m_pLabel4Time = CCLabelTTF::create(tmpStr, "Arial", 25);
    m_pLabel4Time->setAnchorPoint(ccp(0.0f, 0.5f));
    
    sprintf(tmpStr, "最短时间: %.2fs", m_minTime);
    m_pLabel4MinTime = CCLabelTTF::create(tmpStr, "Arial", 25);
    m_pLabel4MinTime->setAnchorPoint(ccp(0.0f, 0.5f));

    if(m_minTime < 0)
    {
        m_pLabel4MinTime->setVisible(false);
    }

    m_soundLine = KSoundLine::create();
    m_soundLine->setPosition(ccp(760, 630));
    m_soundLine->setStop(true);


    m_meter = KMeter::create();
    m_meter->setPosition(ccp(0, 400));
    m_meter->setValue(0);

    m_pLabel->setPosition(ccp(m_meter->m_rect.size.width/2,
                              480));
    
    m_pLabel4Time->setPosition(ccp(10,
                                   380));
    
    m_pLabel4MinTime->setPosition(ccp(10,
                                   320));


    CCSprite* spBG = CCSprite::createWithSpriteFrameName(IMG_BG);
    spBG->setAnchorPoint(CCPointZero);
    spBG->setPosition(CCPointZero);


    this->setAnchorPoint(CCPointZero);
    this->setPosition(CCPointZero);
    this->setScaleX(KRateX);
    this->setScaleY(KRateY);

    this->addChild(spBG);
    this->addChild(m_soundLine);
    this->addChild(m_meter);
    this->addChild(m_pLabel);
    this->addChild(m_pLabel4Time);
    this->addChild(m_pLabel4MinTime);


    //美女图
	CCSprite* spCardImg;
	sprintf(tmpStr, "card%dLv%d.png", m_cardID, m_selectCardLvBtnID);
	spCardImg = CCSprite::createWithSpriteFrameName(tmpStr);
	spCardImg->setTag(TAG_IMG + m_selectCardLvBtnID);
	spCardImg->setPosition(ccp(YS_SCREEN_W/2, YS_SCREEN_H/2));
	this->addChild(spCardImg);
	/*
	CCSprite* spCardImg[m_maxLv];

	for(int i = 0; i < m_maxLv; i++)
	{
		sprintf(tmpStr, "card%dLv%d.png", m_cardID, i);
		spCardImg[i] = CCSprite::createWithSpriteFrameName(tmpStr);
		spCardImg[i]->setTag(TAG_IMG + i);
		spCardImg[i]->setVisible(false);
		spCardImg[i]->setPosition(ccp(YS_SCREEN_W/2, YS_SCREEN_H/2));

		if(i == m_selectCardLvBtnID)
		{
			spCardImg[i]->setVisible(true);
		}

		this->addChild(spCardImg[i]);
	}
	*/

	//初始化移动的点
    CCPointArray *array[7];
    array[0] = CCPointArray::create(20);
	array[0]->addControlPoint(ccp(270, 406));
	array[0]->addControlPoint(ccp(206, 263));
	array[0]->addControlPoint(ccp(305, 135));
	array[0]->addControlPoint(ccp(400, 217));
	array[0]->addControlPoint(ccp(410, 394));
	array[0]->addControlPoint(ccp(270, 406));

	array[1] = CCPointArray::create(20);
	array[1]->addControlPoint(ccp(639, 395));
	array[1]->addControlPoint(ccp(707, 247));
	array[1]->addControlPoint(ccp(590, 132));
	array[1]->addControlPoint(ccp(497, 216));
	array[1]->addControlPoint(ccp(516, 394));
	array[1]->addControlPoint(ccp(639, 395));

	array[2] = CCPointArray::create(20);
	array[2]->addControlPoint(ccp(270, 406));
	array[2]->addControlPoint(ccp(206, 263));
	array[2]->addControlPoint(ccp(305, 135));
	array[2]->addControlPoint(ccp(400, 217));
	array[2]->addControlPoint(ccp(410, 394));
	array[2]->addControlPoint(ccp(270, 406));

	array[3] = CCPointArray::create(20);
	array[3]->addControlPoint(ccp(590, 132));
	array[3]->addControlPoint(ccp(497, 216));
	array[3]->addControlPoint(ccp(516, 394));
	array[3]->addControlPoint(ccp(639, 395));
	array[3]->addControlPoint(ccp(707, 247));
	array[3]->addControlPoint(ccp(590, 132));

	array[4] = CCPointArray::create(20);
	array[4]->addControlPoint(ccp(401, 463));
	array[4]->addControlPoint(ccp(399, 282));
	array[4]->addControlPoint(ccp(401, 463));

	array[5] = CCPointArray::create(20);
	array[5]->addControlPoint(ccp(604, 449));
	array[5]->addControlPoint(ccp(607, 267));
	array[5]->addControlPoint(ccp(604, 449));

	array[6] = CCPointArray::create(20);
	array[6]->addControlPoint(ccp(458, 93));
	array[6]->addControlPoint(ccp(458, 93));

	array[0]->retain();
	array[1]->retain();
	array[2]->retain();
	array[3]->retain();
	array[4]->retain();
	array[5]->retain();
	array[6]->retain();

	std::vector<CCPointArray*> vArrPoint;
	vArrPoint.clear();
	vArrPoint.push_back(array[0]);
	vArrPoint.push_back(array[1]);
	m_vLvArrPoint.push_back(vArrPoint);

	vArrPoint.clear();
	vArrPoint.push_back(array[2]);
	vArrPoint.push_back(array[3]);
	m_vLvArrPoint.push_back(vArrPoint);

	vArrPoint.clear();
	vArrPoint.push_back(array[4]);
	vArrPoint.push_back(array[5]);
	vArrPoint.push_back(array[6]);
	m_vLvArrPoint.push_back(vArrPoint);

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
    btnBack->addTouchEventListener(this, toucheventselector(KMainLogicLayer::touchEvent));
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

    	btnLvl[i]->addTouchEventListener(this, toucheventselector(KMainLogicLayer::touchEvent));
    	btnLvl[i]->setTag(TAG_BTN_LVL + i);
    	m_widget->addWidget(btnLvl[i]);
    }

    m_widget->addWidget(btnBack);

    this->addChild(m_widget);

    return true;
}

void KMainLogicLayer::onEnter()
{
	CCLayer::onEnter();
}

void KMainLogicLayer::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();

	m_isCanTouch = true;
	m_isCanPlayGame = true;
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 128);
//	this->setTouchEnabled(true);

	//切换动画结束后才可以显示点
	std::vector<CCPointArray*> vArrPointTmp = m_vLvArrPoint[m_selectCardLvBtnID];

	for(int i = 0; i < vArrPointTmp.size(); i++)
	{
		KMovePoint* movePoint = KMovePoint::create(0, m_movePointTime[m_selectCardLvBtnID], vArrPointTmp[i], this);
		this->addChild(movePoint);
		s_myRectDate tmpRectDate;
		tmpRectDate.m_movePoint = movePoint;
		tmpRectDate.isCanPlusScore = false;
		m_vRectDate.push_back(tmpRectDate);
	}
    
    //切换结束后开机各类逻辑计时器
    this->schedule(schedule_selector(KMainLogicLayer::updateScore), 0.1f);
    this->schedule(schedule_selector(KMainLogicLayer::updateIsPZ), 0.0f);
    this->schedule(schedule_selector(KMainLogicLayer::updatePlaySound), 0.0f);
    this->schedule(schedule_selector(KMainLogicLayer::updateCostTime), 0.0f);

}

void KMainLogicLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	m_isCanTouch = false;
	m_isCanPlayGame = false;
	this->setTouchEnabled(false);
    
    //切换结束后开机各类逻辑计时器
    this->unschedule(schedule_selector(KMainLogicLayer::updateScore));
    this->unschedule(schedule_selector(KMainLogicLayer::updateIsPZ));
    this->unschedule(schedule_selector(KMainLogicLayer::updatePlaySound));
    this->unschedule(schedule_selector(KMainLogicLayer::updateCostTime));
	CCLayer::onExit();
}

void KMainLogicLayer::touchEvent(CCObject* pSender, TouchEventType type)
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
#if(DEBUG_RECT == 1)
					for(int i = 0; i < m_vTest.size(); i++)
					{
						this->removeChild(m_vTest[i].m_label);
					}

					m_vTest.clear();
#endif
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

void KMainLogicLayer::updateScore()
{
	if(!m_isCanTouch || !m_isCanPlayGame)
	{
		m_score = 0;
		char retStr[50];
		sprintf(retStr, "%d%%", m_score);
		m_pLabel->setString(retStr);
		m_meter->setValue(m_score);
		return;
	}

	//移动点“都”被碰上了
    if(m_isCanPlusScore)
    {
        m_score++;

        if(m_score > 100)
        {
        	m_score = 100;
        }

        isScaleTouchPoint(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        K4IOSAudioEngine::shareK4IOSAudioEngine()->playVibrate();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
#endif
    }
    //移动点没有“都”被碰上
    else
    {
        m_score--;

        if(m_score < 0)
        {
        	m_score = 0;
        }

        isScaleTouchPoint(false);
    }

    char strtmp[50];
	sprintf(strtmp, "%d%%", m_score);
	m_pLabel->setString(strtmp);
    m_meter->setValue(m_score);
}

//void KMainLogicLayer::registerWithTouchDispatcher(void)
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 128);
//}

void KMainLogicLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if(!m_isCanPlayGame || !m_isCanTouch || m_isInBug)
	{
		return;
	}

    CCSetIterator iter = pTouches->begin();

    for (; iter != pTouches->end(); iter++)
    {
        CCTouch* pTouch = (CCTouch*)(*iter);
        CCPoint location = pTouch->getLocation();
        location = this->convertToNodeSpace(location);

        KTouchPoint *tmprect = KTouchPoint::create(0, ccp(
        		location.x,
        		location.y));

        
        m_dic.setObject(tmprect, pTouch->getID());
        m_arrRect->addObject(tmprect);
        this->addChild(tmprect);

    }

}

void KMainLogicLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

	if(!m_isCanPlayGame || !m_isCanTouch || m_isInBug)
	{
		return;
	}

    CCSetIterator iter = pTouches->begin();

    for (; iter != pTouches->end(); iter++)
    {

        CCTouch* pTouch = (CCTouch*)(*iter);
        CCPoint location = pTouch->getLocation();
        location = this->convertToNodeSpace(location);

        //执行到此处判断里证明处于切换场景时，没有对按下时间初始化引起的崩溃
        //处理切换场景时没有初始化按下点导致的崩溃问题
        //解决办法在此处做个标量，证明处于bug阶段
        //直到抬起该点方可解除该标量
        if(0 == m_dic.count())
        {
        	m_isInBug = true;
        	return;
        }

        if(m_isInBug)
        {
        	return;
        }

        KTouchPoint* tmprect = (KTouchPoint*)m_dic.objectForKey(pTouch->getID());

        tmprect->setPosition(ccp(
        		location.x - tmprect->m_rect.size.width/2,
                location.y - tmprect->m_rect.size.height/2));

    }

}

void KMainLogicLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if(!m_isCanPlayGame || !m_isCanTouch)
	{
		return;
	}

	m_isInBug = false;

    CCSetIterator iter = pTouches->begin();

    for (; iter != pTouches->end(); iter++)
    {
        CCTouch* pTouch = (CCTouch*)(*iter);
        KTouchPoint* tmprect = (KTouchPoint*)m_dic.objectForKey(pTouch->getID());
        m_arrRect->removeObject(tmprect);
        m_dic.removeObjectForKey(pTouch->getID());
        this->removeChild(tmprect);

#if(DEBUG_RECT == 1)
        //test
        CCPoint location = pTouch->getLocation();
        location = this->convertToNodeSpace(location);
        char strTmp[50];
        KTest tmpTest;
        tmpTest.m_point = location;
        int tx = location.x;
        int ty = location.y;
        sprintf(strTmp, "(%d, %d)", tx, ty);
        tmpTest.m_label = CCLabelTTF::create(strTmp, "Arial", 24);
        tmpTest.m_label->setAnchorPoint(ccp(0.5f, 0.5f));
        tmpTest.m_label->setPosition(ccp(tx - 30, ty - 30));
        this->addChild(tmpTest.m_label);
        m_vTest.push_back(tmpTest);
#endif
    }
}

void KMainLogicLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator iter = pTouches->begin();

	for (; iter != pTouches->end(); iter++)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		KTouchPoint* tmprect = (KTouchPoint*)m_dic.objectForKey(pTouch->getID());
		m_arrRect->removeObject(tmprect);
		m_dic.removeObjectForKey(pTouch->getID());
		this->removeChild(tmprect);
	}
}

void KMainLogicLayer::updateLvBtn(int currSelectID)
{
	m_isCanPlayGame = false;
	CCScene* pScene = KManagerGameLogic::shareKManagerGameLogic()->gameScene4Type(
	    		m_cardID,
	    		currSelectID);
//	CCScene* pScene = KMainLogicLayer::scene(0, currSelectID);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
	/*
	m_isCanPlayGame = false;
	UIButton* btnLvl = NULL;
	CCSprite* spImg = NULL;
	btnLvl = ((UIButton*)m_widget->getWidgetByTag(TAG_BTN_LVL + m_selectCardLvBtnID));
	btnLvl->loadTextureNormal(IMG_CLOSE_LVL_BTN, UI_TEX_TYPE_PLIST);
	btnLvl->loadTexturePressed(IMG_CLOSE_LVL_BTN, UI_TEX_TYPE_PLIST);
	btnLvl->loadTextureDisabled(IMG_CLOSE_LVL_BTN, UI_TEX_TYPE_PLIST);

	spImg = ((CCSprite*)this->getChildByTag(TAG_IMG + m_selectCardLvBtnID));
	spImg->setVisible(false);
	spImg->stopAllActions();
	spImg->setOpacity(0);
//	spImg->runAction(CCSequence::create(
//			CCFadeOut::create(OUT_TIME),
//			NULL));

	//清除之前的点
	for(int i = 0; i < m_vRectDate.size(); i++)
	{
		m_vRectDate[i].m_movePoint->m_parent->removeChild(m_vRectDate[i].m_movePoint->m_streak);
		this->removeChild(m_vRectDate[i].m_movePoint);
	}

	m_vRectDate.clear();

	m_selectCardLvBtnID = currSelectID;
	btnLvl = ((UIButton*)m_widget->getWidgetByTag(TAG_BTN_LVL + m_selectCardLvBtnID));
	btnLvl->loadTextureNormal(IMG_OPEN_LVL_BTN, UI_TEX_TYPE_PLIST);
	btnLvl->loadTexturePressed(IMG_OPEN_LVL_BTN, UI_TEX_TYPE_PLIST);
	btnLvl->loadTextureDisabled(IMG_OPEN_LVL_BTN, UI_TEX_TYPE_PLIST);
	btnLvl->setTouchEnabled(true);

	spImg = ((CCSprite*)this->getChildByTag(TAG_IMG + m_selectCardLvBtnID));
	spImg->setOpacity(0);
	spImg->setVisible(true);
	spImg->stopAllActions();
	spImg->runAction(CCSequence::create(
			CCFadeIn::create(OUT_TIME),
			CCCallFunc::create(this, callfunc_selector(KMainLogicLayer::imgActionCallBack)),
			NULL));
	*/
}

void KMainLogicLayer::visit()
{
    CCLayer::visit();
    
#if (DEBUG_RECT == 1)


    ccColor4F color;
    /*
    color.r = 0;
    color.g = 255;
    color.b = 0;
    color.a = 255;
    
    for(int i = 0; i < m_vRectDate.size(); i++)
    {
    	ccDrawSolidRect(convertToWorldSpace(m_vRectDate[i].m_movePoint->m_rect.origin),
    			ccp(convertToWorldSpace(m_vRectDate[i].m_movePoint->m_rect.origin).x + m_vRectDate[i].m_movePoint->m_rect.size.width,
    					convertToWorldSpace(m_vRectDate[i].m_movePoint->m_rect.origin).y + m_vRectDate[i].m_movePoint->m_rect.size.height),
    			color);
    }
	*/

	color.r = 0;
	color.g = 255;
	color.b = 0;
	color.a = 255;

    for(int i = 0; i < m_arrRect->count(); i++)
	{
    	KTouchPoint* tmprect = ((KTouchPoint*)m_arrRect->objectAtIndex(i));

		ccDrawSolidRect(convertToWorldSpace(tmprect->m_rect.origin),
				ccp(convertToWorldSpace(tmprect->m_rect.origin).x + tmprect->m_rect.size.width,
						convertToWorldSpace(tmprect->m_rect.origin).y + tmprect->m_rect.size.height),
				color);

	}

    for(int i = 0; i < m_vTest.size(); i++)
   	{
   		ccDrawSolidRect(convertToWorldSpace(ccp(m_vTest[i].m_point.x - 3, m_vTest[i].m_point.y - 3)),
   				ccp(convertToWorldSpace(ccp(m_vTest[i].m_point.x - 3, m_vTest[i].m_point.y - 3)).x + 6,
   						convertToWorldSpace(ccp(m_vTest[i].m_point.x - 3, m_vTest[i].m_point.y - 3)).y + 6),
   				color);

   	}

#endif
}

void KMainLogicLayer::updateIsPZ()
{
	if(!m_isCanTouch || !m_isCanPlayGame)
	{
		return;
	}

	m_isCanPlusScore = true;

	for(int i = 0; i < m_vRectDate.size(); i++)
	{
		m_vRectDate[i].isCanPlusScore = false;
	}

	for(int i = 0; i < m_arrRect->count(); i++)
	{
		KTouchPoint* tmprect = ((KTouchPoint*)m_arrRect->objectAtIndex(i));

		for(int j = 0; j < m_vRectDate.size(); j++)
		{
			if(m_vRectDate[j].m_movePoint->m_rect.intersectsRect(tmprect->m_rect))
			{
				m_vRectDate[j].isCanPlusScore = true;
			}
		}
	}

	for(int i = 0; i < m_vRectDate.size(); i++)
	{
		if(m_vRectDate[i].isCanPlusScore == false)
		{
			m_isCanPlusScore = false;
			break;
		}
	}
}

void KMainLogicLayer::updatePlaySound()
{
	if(!m_isCanPlaySound)
	{
		return;
	}

	if(m_score == LV_SOUND_0 && m_isCanPlusScore)
	{
		m_isCanPlaySound = false;
		this->scheduleOnce(schedule_selector(KMainLogicLayer::setCanPlaySound), TIME_CAN_PLAY_SOUND);
		this->unschedule(schedule_selector(KMainLogicLayer::resetSound));

		if(m_effectID != -1)
		{
			SimpleAudioEngine::sharedEngine()->stopEffect(m_effectID);
		}

		char strTmp[50];
		sprintf(strTmp, "soundCard%dBtn%dNum%d.mp3", m_cardID, m_selectCardLvBtnID, 0);
		m_effectID = SimpleAudioEngine::sharedEngine()->playEffect(strTmp);
		m_soundLine->setStop(false);
		this->scheduleOnce(schedule_selector(KMainLogicLayer::resetSound), TIME_RESET_SOUND);
	}
	else if(m_score == LV_SOUND_1 && m_isCanPlusScore)
	{
		m_isCanPlaySound = false;
		this->scheduleOnce(schedule_selector(KMainLogicLayer::setCanPlaySound), TIME_CAN_PLAY_SOUND);
		this->unschedule(schedule_selector(KMainLogicLayer::resetSound));

		if(m_effectID != -1)
		{
			SimpleAudioEngine::sharedEngine()->stopEffect(m_effectID);
		}

		char strTmp[50];
		sprintf(strTmp, "soundCard%dBtn%dNum%d.mp3", m_cardID, m_selectCardLvBtnID, 1);
		m_effectID = SimpleAudioEngine::sharedEngine()->playEffect(strTmp);
		m_soundLine->setStop(false);
		this->scheduleOnce(schedule_selector(KMainLogicLayer::resetSound), TIME_RESET_SOUND);
	}
	else if(m_score == LV_SOUND_2 && m_isCanPlusScore)
	{
		m_isCanPlaySound = false;
		this->scheduleOnce(schedule_selector(KMainLogicLayer::setCanPlaySound), TIME_CAN_PLAY_SOUND);

		this->unschedule(schedule_selector(KMainLogicLayer::resetSound));

		if(m_effectID != -1)
		{
			SimpleAudioEngine::sharedEngine()->stopEffect(m_effectID);
		}

		char strTmp[50];
		sprintf(strTmp, "soundCard%dBtn%dNum%d.mp3", m_cardID, m_selectCardLvBtnID, 2);
		m_effectID = SimpleAudioEngine::sharedEngine()->playEffect(strTmp);
		m_soundLine->setStop(false);
		this->scheduleOnce(schedule_selector(KMainLogicLayer::resetSound), TIME_RESET_SOUND);

		//进入下一关
		nextLv();
        //更新最短时间
        char tmpStr[30];
        
        if(m_minTime < 0)
        {
            m_minTime = m_costTime;
            m_pLabel4MinTime->setVisible(true);
        }
        else if(m_costTime < m_minTime)
        {
            m_minTime = m_costTime;
        }
        
        sprintf(tmpStr, "最短时间: %.2fs", m_minTime);
        m_pLabel4MinTime->setString(tmpStr);
        
        sprintf(tmpStr, "card%dLv%dMinTime", m_cardID, m_selectCardLvBtnID);
        CCUserDefault::sharedUserDefault()->setFloatForKey(tmpStr, m_minTime);
        CCUserDefault::sharedUserDefault()->flush();

        m_costTime = 0.0f;
        m_score = 0;
	}

}

void KMainLogicLayer::resetSound()
{
	m_effectID = -1;
	m_soundLine->setStop(true);
}

void KMainLogicLayer::nextLv()
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
}

//目前不用此函数了
void KMainLogicLayer::imgActionCallBack()
{
	//增加新的点
	std::vector<CCPointArray*> vArrPointTmp = m_vLvArrPoint[m_selectCardLvBtnID];

	for(int i = 0; i < vArrPointTmp.size(); i++)
	{
		KMovePoint* movePoint = KMovePoint::create(0, m_movePointTime[m_selectCardLvBtnID], vArrPointTmp[i], this);
		this->addChild(movePoint);
		s_myRectDate tmpRectDate;
		tmpRectDate.m_movePoint = movePoint;
		tmpRectDate.isCanPlusScore = false;
		m_vRectDate.push_back(tmpRectDate);
	}

	m_isCanTouch = true;
	m_isCanPlayGame = true;
}

void KMainLogicLayer::setCanPlaySound()
{
	m_isCanPlaySound = true;
}

void KMainLogicLayer::isScaleTouchPoint(bool isCanScale)
{
	if(m_isCanScale != isCanScale)
	{
		m_isCanScale = isCanScale;

		if(m_isCanScale)
		{
			for(int i = 0; i < m_arrRect->count(); i++)
			{
				KTouchPoint* tmprect = ((KTouchPoint*)m_arrRect->objectAtIndex(i));
				tmprect->setCircleBig(true);
			}

            //移动点变大
			for(int i = 0; i < m_vRectDate.size(); i++)
			{
				m_vRectDate[i].m_movePoint->setScale(1.0f);//改成2倍即可
			}
		}
		else
		{
			for(int i = 0; i < m_arrRect->count(); i++)
			{
				KTouchPoint* tmprect = ((KTouchPoint*)m_arrRect->objectAtIndex(i));
				tmprect->setCircleBig(false);
			}

            //移动点恢复原始大小
			for(int i = 0; i < m_vRectDate.size(); i++)
			{
				m_vRectDate[i].m_movePoint->setScale(1.0f);
			}
		}
	}

}

void KMainLogicLayer::updateCostTime(float dt)
{
    char tmpStr[20];
    m_costTime += dt;
    
    sprintf(tmpStr, "耗时: %.2fs", m_costTime);
    m_pLabel4Time->setString(tmpStr);
}
