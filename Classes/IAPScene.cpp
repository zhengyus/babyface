#include "IAPScene.h"
#include "AppMacros.h"


CCScene* IAPScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    IAPScene *layer = IAPScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool IAPScene::init()
{

    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCControlButton* btn = CCControlButton::create("商店", "Heiti SC",30);
    btn->setPosition(ccp(2480/2, 1536/2));
//    btn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::switchView), CCControlEventTouchDown);
    addChild(btn);
    
    return true;
}
