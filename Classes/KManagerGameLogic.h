/*
 * KManagerGameLogic.h
 *
 *  Created on: 2014-10-30
 *      Author: kangchao
 */

#ifndef KMANAGERGAMELOGIC_H_
#define KMANAGERGAMELOGIC_H_

#include "cocos2d.h"
#include "Define.h"

USING_NS_CC;

class KManagerGameLogic
{
public:
	KManagerGameLogic();
	virtual ~KManagerGameLogic();

	static KManagerGameLogic* shareKManagerGameLogic();

	CCScene* gameScene4Type(int cardID, int Lv);
};

#endif /* KMANAGERGAMELOGIC_H_ */
