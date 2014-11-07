/*
 * KMeter4Comb.h
 *
 *  Created on: 2014-11-4
 *      Author: kangchao
 */

#ifndef KMETER4COMB_H_
#define KMETER4COMB_H_

#include "cocos2d.h"
#include "Define.h"

USING_NS_CC;

#define COMB_VALUE 100

class KMeter4Comb : public CCNode
{
public:
	KMeter4Comb();
	virtual ~KMeter4Comb();

virtual bool init();

	CREATE_FUNC(KMeter4Comb);

	//仪表数值
	void setValue(int value);
	//仪表数值
	int getValue();

private:

	int m_value;
	CCSprite* m_line;
};

#endif /* KMETER4COMB_H_ */
