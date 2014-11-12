#include "IAPScene.h"

IAPScene::~IAPScene()
{
    
}

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
        return false;
    
    CCControlButton* btn = CCControlButton::create("商店", "Heiti SC",30);
    btn->setPosition(ccp(2480/2, 1536/2));
    btn->addTargetWithActionForControlEvents(this, cccontrol_selector(IAPScene::store), CCControlEventTouchDown);
    addChild(btn);
    
    _iapHelper = IAPHelper::getInstance(*this);
    
    return true;
    
}

void IAPScene::store(CCObject*, CCControlEvent)
{
    _iapHelper->requestProducts();
}

void IAPScene::productsHandle(vector<Product> products)
{
    _products = products;
    Widget* widget = GUIReader::shareReader()->widgetFromJsonFile("ItemProduct.ExportJson");
    widget->setAnchorPoint(ccp(.5, .5));
    
    UILayer* layer = UILayer::create();
    float x = widget->getContentSize().width/2;
    float y = widget->getContentSize().height/2 +20;
    for (int i = 0; i < products.size(); ++i)
    {
        Widget* product = widget->clone();
        
        Product p = products[i];
        
        ImageView* img = static_cast<ImageView*>(product->getChildByName("Image_product"));
        Label* title = static_cast<Label*>(product->getChildByName("Label_title"));
        Label* price = static_cast<Label*>(product->getChildByName("Label_price"));
        Button* buy = static_cast<Button*>(product->getChildByName("Button_buy"));
        
        title->setText(p.title);
        price->setText(p.price);
        buy->setTag(p.identifier);
        buy->addTouchEventListener(this, toucheventselector(IAPScene::buy));
        
        product->setPosition(ccp(x, y));
        x += widget->getContentSize().width;
        
        layer->addWidget(product);
    }
    addChild(layer);
}

void IAPScene::purchased()
{
    CCLog("ccccc purchased");
}

void IAPScene::purchaseFailed()
{
    CCLog("ccccc purchaseFailed");
}

void IAPScene::buy(CCObject* obj,TouchEventType eventType)
{
    if (TOUCH_EVENT_ENDED == eventType)
    {
        CCNode* node = static_cast<CCNode*>(obj);
        
        _iapHelper->buyProductById(node->getTag());
    }
}
