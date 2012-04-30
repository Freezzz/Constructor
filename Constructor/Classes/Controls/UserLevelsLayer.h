//
//  UserLevelsLayer.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/24/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __UserLevelsLayer_H__
#define __UserLevelsLayer_H__

#include "cocos2d.h"
#include "Serialization/LevelManager.h"
using namespace cocos2d;


////////////////////////////////////////////////////
// UserLevelsLayer
///////////////////////////////////////////////////
class UserLevelsLayer : public CCLayer {
private:
	CCPoint m_beginPos;
	CCMenu* m_menu;
	vector<UserLevelDescription*> * m_levels;
	
	virtual bool init();
	
	// Overrides for scrolling
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	
	void populateMenu();
	
	void levelTapCallback(CCObject* sender);
	void closeButtonCallback(CCObject* sender);
public:
	
	void refreshList();
	LAYER_NODE_FUNC(UserLevelsLayer);
};

#endif
