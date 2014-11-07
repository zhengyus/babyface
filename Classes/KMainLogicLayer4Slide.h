/*
 * KMainLogicLayer4Slide.h
 *
 *  Created on: 2014-10-31
 *      Author: kangchao
 */

#ifndef KMAINLOGICLAYER4SLIDE_H_
#define KMAINLOGICLAYER4SLIDE_H_

#include "cocos2d.h"
#include "Define.h"
#include "KSoundLine.h"
#include "KTouchPoint.h"
#include "KMeter4Time.h"
#include "KMeter4Comb.h"

#define TAG_IMG 20
#define TAG_BTN_BACK 0
#define TAG_BTN_LVL 10
#define TAG_CLIPER 20
#define TIME_4_FADEOUT 1.0f
#define TIME_BOUDLE 2.0f
#define MOVE_DIST 50
#define D_TIME 6.0f
#define D_COMB 60

USING_NS_CC;

class KMainLogicLayer4Slide : public CCLayer
{
public:

	KMainLogicLayer4Slide();
    ~KMainLogicLayer4Slide();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    //cardID传入的是哪个卡牌
    //selectCardLvBtnID传入当前选择的是哪个按钮
    virtual bool init(int cardID, int selectCardLvBtnID);

    void touchEvent(CCObject* pSender, TouchEventType type);

//    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);


    virtual void visit();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();


    void updateLvBtn(int currSelectID);
    void moveByNodeCliper(bool isCanMove);//遮罩node抖动逻辑判断
    void runVisibleFalseAction(CCNode* node);//遮罩node渐变消失动画
    void nextLv();//进入下一个btnLv
    void stopNodeCliper();//停止移动
    void runBtn(CCNode *pSender, void *currSelectID);
    void updateTime(float dt);
    void setIsCanTouch();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(int cardID, int selectCardLvBtnID);

    // implement the "static node()" method manually
//    CREATE_FUNC(KMainLogicLayer);
    static KMainLogicLayer4Slide* create(int cardID, int selectCardLvBtnID)
    {
    	KMainLogicLayer4Slide *pRet = new KMainLogicLayer4Slide();
        if (pRet && pRet->init(cardID, selectCardLvBtnID))
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = 0;
            return 0;
        }
    }

private:

    bool m_isCanPlusScore;//是否可以加分
    bool m_isCanTouch;//是否可以点击
    bool m_isCanPlayGame;//是否可以玩游戏
    bool m_isWinGame;//是否胜利

    int m_score;
    KSoundLine* m_soundLine;

    UILayer * m_widget;
    CCMotionStreak* m_streak;//滑条
    int m_maxLv;//最大夜数1-3
    int m_cardID;//哪张卡牌
    int m_cardLv;//卡牌开启的等级开了几夜0-2
    int m_selectCardLvBtnID;//选择了哪一个按钮ID 也是记录了当前属于哪个关卡（第几夜） 0-

    CCNode* m_cliperNode;//遮罩层
    CCNode* m_stencilNode;//遮罩

    int m_moveDist;//移动距离
    CCPoint m_begin;//按下点
    bool m_isCanMove;//当前抖动状态

    CCParticleBatchNode* m_patBatchNode;
    
    KMeter4Time *m_meterTime;
    KMeter4Comb *m_meterComb;
    float m_timeValue;
    float m_timeGoValue;
    
    CCLabelTTF* m_pLabel;
    CCLabelTTF* m_pLabel4Lose;
    CCLabelTTF* m_pLabel4Win;

    int m_combMax;
    int m_combCurr;
};

#endif /* KMAINLOGICLAYER4SLIDE_H_ */
