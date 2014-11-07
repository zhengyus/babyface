/*
 * KMeter4Time.h
 *
 *  Created on: 2014-11-4
 *      Author: kangchao
 */

#ifndef KMETER4TIME_H_
#define KMETER4TIME_H_

#include "cocos2d.h"
#include "Define.h"

USING_NS_CC;

#define TIME_VALUE 100

class KMeter4Time : public CCNode
{
public:
	KMeter4Time();
	virtual ~KMeter4Time();

	virtual bool init();

	CREATE_FUNC(KMeter4Time);

	//仪表数值
	void setValue(int value);
	//仪表数值
	int getValue();

private:

	int m_value;
	CCSprite* m_line;
};

#endif /* KMETER4TIME_H_ */
