//
//  KMainLogicLayer.h
//  BitBattle
//
//  Created by kc on 14-9-15.
//
//

#ifndef __BitBattle__KMainLogicLayer__
#define __BitBattle__KMainLogicLayer__

#include "cocos2d.h"
#include "Define.h"
#include "KSoundLine.h"
#include "KMeter.h"
#include "KTouchPoint.h"
#include "KMovePoint.h"

#define TAG_BTN_BACK 0
#define TAG_BTN_LVL 10
#define TAG_IMG 20
#define LV_SOUND_0 33
#define LV_SOUND_1 66
#define LV_SOUND_2 100
#define OUT_TIME 3.0f
#define TIME_CAN_PLAY_SOUND 3.0f
#define TIME_RESET_SOUND 2.0f

USING_NS_CC;

struct KTest
{
	CCPoint m_point;
	CCLabelTTF* m_label;
};


struct s_myRectDate
{
	KMovePoint* m_movePoint;//移动点
    bool isCanPlusScore;//是否可以加分
};

class KMainLogicLayer : public CCLayer
{
public:
    
    KMainLogicLayer();
    ~KMainLogicLayer();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    //cardID传入的是哪个卡牌
    //selectCardLvBtnID传入当前选择的是哪个按钮
    virtual bool init(int cardID, int selectCardLvBtnID);
    
    void touchEvent(CCObject* pSender, TouchEventType type);

//    virtual void registerWithTouchDispatcher(void);
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    
    virtual void visit();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    void updateScore();
    void updateIsPZ();//判断是否都已经触摸上移动点的逻辑
    void updatePlaySound();
    void updateLvBtn(int currSelectID);
    void updateCostTime(float dt);
    void resetSound();//重置声音相关
    void nextLv();//进入下一个btnLv
    void imgActionCallBack();//
    void setCanPlaySound();//设置可以播放声音
    void isScaleTouchPoint(bool isCanScale);//是否改变触点的大小，根据移动点是否都被摸上判断

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(int cardID, int selectCardLvBtnID);
    
    // implement the "static node()" method manually
//    CREATE_FUNC(KMainLogicLayer);
    static KMainLogicLayer* create(int cardID, int selectCardLvBtnID)
    {
        KMainLogicLayer *pRet = new KMainLogicLayer();
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
    std::vector<s_myRectDate> m_vRectDate;//存储移动点s_myRectDate结构容器
    
    bool m_isCanPlusScore;//是否可以加分
    bool m_isCanTouch;//是否可以点击
    bool m_isCanScale;//是否可以放大
    bool m_isCanPlaySound;//是否可以播放声音
    bool m_isCanPlayGame;//是否可以玩游戏
    bool m_isInBug;//bug阶段标志

    CCLabelTTF* m_pLabel;//酒杯百分比
    CCLabelTTF* m_pLabel4Time;//计算用的时间
    CCLabelTTF* m_pLabel4MinTime;//计算用的最短时间
    
    int m_score;
    
    CCArray *m_arrRect;//存储触摸点容器KTouchPoint
    CCDictionary m_dic;//多点对应字典

    KSoundLine* m_soundLine;
    KMeter* m_meter;

    UILayer * m_widget;
    int m_maxLv;//最大夜数1-3
    int m_cardID;//哪张卡牌
    int m_cardLv;//卡牌开启的等级开了几夜0-2
    int m_selectCardLvBtnID;//选择了哪一个按钮ID 也是记录了当前属于哪个关卡（第几夜） 0-2
    int m_effectID;//播放声音ID
    std::vector< std::vector<CCPointArray*> > m_vLvArrPoint;//多种情况点情况
    float m_movePointTime[3];//移动点速度 值越小越快
    
    float m_costTime;//通关所花费的时间
    float m_minTime;//通关所花费最短时间

#if(DEBUG_RECT == 1)
    //test point
    std::vector<KTest> m_vTest;
#endif
};

#endif /* defined(__BitBattle__KMainLogicLayer__) */
