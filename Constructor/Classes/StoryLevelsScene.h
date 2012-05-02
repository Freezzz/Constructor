//
//  StoryLevelsScene.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/23/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __StoryLevelsScene_H__
#define __StoryLevelsScene_H__

#include "cocos2d.h"
#include "Serialization/LevelManager.h"
using namespace cocos2d;



////////////////////////////////////////////////////
// StoryLevelsScene
///////////////////////////////////////////////////
class StoryLevelsScene : public CCLayer {
private:
	vector<StoryLevelDescription*> * m_levels;
protected:
	virtual bool init();
	
	void levelSelectedTap( CCObject * sender );
	void mainMenuTap( CCObject * sender );
public:
	////////////////////////////////////////////////////
	// Static factory creation methods
	////////////////////////////////////////////////////
	static CCScene* scene();
	LAYER_NODE_FUNC(StoryLevelsScene);
};

#endif
