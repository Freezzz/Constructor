//
//  MainMenuScene.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/23/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __MainMenuScene_H__
#define __MainMenuScene_H__

#include "cocos2d.h"
using namespace cocos2d;



////////////////////////////////////////////////////
// MainMenuScene
///////////////////////////////////////////////////
class MainMenuScene : public CCLayer {
protected:
	virtual bool init();
	
	void newGameTap( CCObject * sender );
	void sanboxtap( CCObject * sender );
public:
	////////////////////////////////////////////////////
	// Static factory creation methods
	////////////////////////////////////////////////////
	static CCScene* scene();
	LAYER_NODE_FUNC(MainMenuScene);
};

#endif
