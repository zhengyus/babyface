#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "IAP.framework/Headers/IAPHelper.h"
#include "IAP.framework/Headers/Product.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;

class IAPScene : public CCLayer , IAPDelegate
{
public:
    ~IAPScene();
    
public:

    virtual bool init();  

    static cocos2d::CCScene* scene();
    
    void store(CCObject*, CCControlEvent);

    void productsHandle(std::vector<Product>);
    void purchased();
    void purchaseFailed();
    
    void buy(CCObject*,TouchEventType);
    
    CREATE_FUNC(IAPScene);
    
private:
    
    IAPHelper       *_iapHelper;
    vector<Product> _products;
    
};

#endif // __HELLOWORLD_SCENE_H__
