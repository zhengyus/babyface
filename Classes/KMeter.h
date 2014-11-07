/*
 * KMeter.h
 *
 *  Created on: 2014-10-12
 *      Author: Administrator
 *      进度条
 */

#ifndef KMETER_H_
#define KMETER_H_

#include "cocos2d.h"
#include "Define.h"

#define MAX_LINE_NUM 9

USING_NS_CC;

class KMeter : public CCNode
{
public:
	KMeter();
	virtual ~KMeter();

	virtual bool init();

	CREATE_FUNC(KMeter);

	//仪表数值
	void setValue(float value);

	CCRect m_rect;

private:

	float m_value;
	UILoadingBar* m_pin;
	UILayer * m_widget;
};

#endif /* KMETER_H_ */
