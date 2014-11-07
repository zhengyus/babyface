/*
 * KSoundLine.h
 *
 *  Created on: 2014-10-11
 *      Author: kangchao
 *      声音线
 */

#ifndef KSOUNDLINE_H_
#define KSOUNDLINE_H_

#include "cocos2d.h"
#include "Define.h"
#include "SimpleAudioEngine.h"

#define MAX_LINE_NUMS 8

USING_NS_CC;
using namespace CocosDenshion;

class KSoundLine : public CCNode
{
public:
	KSoundLine();
	virtual ~KSoundLine();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static node()" method manually
	CREATE_FUNC(KSoundLine);

	void setStop(bool isRunStop);

	CCRect m_rect;

private:

	void updateIsRun();
	void runLine();

	bool m_isRunStop;
	bool m_isRunStopCurr;
	CCArray* m_arrLines;

};

#endif /* KSOUNDLINE_H_ */
