//
//  CreationLayer.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __CreationLayer_H__
#define __CreationLayer_H__

#import "cocos2d.h"


using namespace cocos2d;


////////////////////////////////////////////////////
// CreationLayer - gui layer containing menus 
// for creation screen
///////////////////////////////////////////////////
class CreationLayer : public CCLayer {
private:
	virtual bool init();
    CCLabelTTF * labelPause;
    CCLabelTTF * labelPlay;
    
    void onPlayButton(CCObject * sender);
    void onPauseButton(CCObject * sender);	
    
    void onDeleteButton(CCObject * sender);
    void onResetButton(CCObject * sender);    
public:
	LAYER_NODE_FUNC(CreationLayer);
};

#endif
