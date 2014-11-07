#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class IAPScene : public cocos2d::CCLayer
{
public:

    virtual bool init();  


    static cocos2d::CCScene* scene();
    
    void stor(CCObject*, CCControlEvent);
    
    // implement the "static node()" method manually
    CREATE_FUNC(IAPScene);
};

#endif // __HELLOWORLD_SCENE_H__
