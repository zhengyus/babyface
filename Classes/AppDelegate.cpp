#include "AppDelegate.h"
#include "KMainLogicLayer.h"
#include "KManagerGameLogic.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    //loading........................
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_GAME_LOGIC);

    //初始化关卡游戏类型
	int type;
	char tmpStr[50];
	typeGameLogic t[4] = {
			TYPE_GAME_LOGIC_MOVE_POINT,
			TYPE_GAME_LOGIC_MOVE_POINT,
			TYPE_GAME_LOGIC_MOVE_POINT,
			TYPE_GAME_LOGIC_SLIDER,
	};

	for(int i = 0; i < T_MAX_NUM; i++)
	{
		sprintf(tmpStr, "card%dLv%dType", 0, i);
		type = CCUserDefault::sharedUserDefault()->getIntegerForKey(tmpStr, t[i]);
		CCUserDefault::sharedUserDefault()->setIntegerForKey(tmpStr, type);
		CCUserDefault::sharedUserDefault()->flush();
	}

	//初始化场景
    // create a scene. it's an autorelease object
//    CCScene *pScene = KMainLogicLayer::scene(0, 1);
    CCScene *pScene = KManagerGameLogic::shareKManagerGameLogic()->gameScene4Type(
    		0,
    		0);

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
