//
//  CreationLayer.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __CreationLayer_H__
#define __CreationLayer_H__

#include "cocos2d.h"


using namespace cocos2d;

class UserLevelsLayer;
class UserLevelSaveLayer;
////////////////////////////////////////////////////
// CreationLayer - gui layer containing menus 
// for creation screen
///////////////////////////////////////////////////
class CreationLayer : public CCLayer {
private:
	virtual bool init();
	CCLabelTTF * m_labelPause;
	CCLabelTTF * m_labelPlay;
	CCMenu *m_menu;

	UserLevelsLayer * m_userLevelsLayer;
	UserLevelSaveLayer * m_userLevelSaveLayer;
	void onPlayButton(CCObject * sender);
	void onPauseButton(CCObject * sender);

	void onSaveButton(CCObject * sender);
	void onLoadButton(CCObject * sender);

	void onDeleteButton(CCObject * sender);
	void onResetButton(CCObject * sender);

	void mainMenuTap(cocos2d::CCObject *sender);
public:
	void setOnScreen(bool isOnscreen);
	void setMenuEnable(bool isEnabled);

public:
	LAYER_NODE_FUNC(CreationLayer);

};

#endif
