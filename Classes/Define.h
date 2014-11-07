//
//  Define.h
//  BitBattle
//
//  Created by kc on 14-9-15.
//
//

#ifndef __BitBattle__Define__
#define __BitBattle__Define__

#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

#define T_MAX_NUM 4//测试阶段的宏变量(1-4)
#define T_OPEN_NUM 0//测试开启关卡数量(0-3)
#define T_MIN_TIME -1.0f//测试通关最短时间

#define DEBUG_RECT 0//0-关闭draw矩形 1-开启draw矩形

#define CCP_CENTER ccp(0.5f, 0.5f)

#define SCREEN CCDirector::sharedDirector()->getWinSize()	//设备屏幕宽高
#define YS_SCREEN_W 1024.0f	//基准屏幕宽度大小
#define YS_SCREEN_H 768.0f	//基准屏幕高度大小
#define KRateX float(SCREEN.width/YS_SCREEN_W)	//放缩比率
#define KRateY float(SCREEN.height/YS_SCREEN_H)	//放缩比率

#define BTN_Y_VALUE 50//第几夜按钮间隙位置值
#define BTN_Y_BEGIN_VALUE 230//第几夜按钮开始位置值

#define IMG_MOVE_POINT "movePoint.png"
#define IMG_MOVE_POINT_BG "movePointBG.png"
#define IMG_SOUND_LINE "soundLine.png"
#define IMG_TOUCH_POINT "touchPoint.png"
#define IMG_TOUCH_POINT_BG "touchPointBG.png"
#define IMG_BACK_BTN "backBtn.png"
#define IMG_CLOSE_LVL_BTN "lvlCloseBtn.png"
#define IMG_CLOSE_LVL_UNENABLE_BTN "lvlCloseUnEnableBtn.png"
#define IMG_OPEN_LVL_BTN "lvlOpenBtn.png"
#define IMG_CUP_BG "cupBG.png"
#define IMG_CUP_FONT "cupFont.png"
#define IMG_BG "BG.jpg"
#define IMG_CLIPE_LINE "clipeLine.png"
#define IMG_CLIPE_PAR "clipPar.png"
#define IMG_CLOCK "meterTimeImg.png"
#define IMG_CLOCK_TIME_BG "meterTime.png"
#define IMG_CLOCK_TIME_LINE "meterTimeLine.png"
#define IMG_LOVE "meterLove.png"
#define IMG_LOVE_LINE "meterLoveLine.png"

#define PLIST_GAME_LOGIC "gameLogicImg.plist"
#define PLIST_PAR "clipPar.plist"
#define PLIST_PAR_COMB "combPa.plist"

#define SOUND_BG_MUSIC "bg.mp3"
#define SOUND_CLIP "clip.mp3"

enum typeGameLogic
{
	TYPE_GAME_LOGIC_MOVE_POINT = 0,
	TYPE_GAME_LOGIC_SLIDER
};


#endif /* defined(__BitBattle__Define__) */
