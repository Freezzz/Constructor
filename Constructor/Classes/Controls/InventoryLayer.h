//
//  InventoryLayer.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/14/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __InventoryLayer_H__
#define __InventoryLayer_H__
#include "cocos2d.h"
#include "Constants.h"

class GameObject;
using namespace cocos2d;

////////////////////////////////////////////////////
// InventoryLayer - layer that represents 
// inventrory of player
///////////////////////////////////////////////////
class InventoryLayer : public CCLayer {
private:
	virtual bool init();
    
    CCSprite * m_boxButton;
    CCSprite * m_pointButton;
    CCSprite * m_panelButton;
    CCSprite * m_springButton;    
	
public:
    //////////////////////////////////////////////////// 
    // Shows and hides inventory 
    //////////////////////////////////////////////////// 
    void setOnScreen(bool isOnscreen);
    
    //////////////////////////////////////////////////// 
    // Returns wich button type was tapped 
    //////////////////////////////////////////////////// 
    ObjectType getObjectTypeForTapCoordinates(CCPoint location);
    
	//////////////////////////////////////////////////// 
	// Creates object if tap location is on any button,
	// null othewise
	//////////////////////////////////////////////////// 
	GameObject* getGameObjectForTapLocation(CCPoint location);
	
	LAYER_NODE_FUNC(InventoryLayer);
};

#endif
