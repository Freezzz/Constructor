//
//  ObjectPanel.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/14/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectPanel_H__
#define __ObjectPanel_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;

////////////////////////////////////////////////////
// ObjectPanel
///////////////////////////////////////////////////
class ObjectPanel : public GameObject {
protected:
	virtual bool init();	
public:
    void createBodyAtPosition(CCPoint position);
    
	LAYER_NODE_FUNC(ObjectPanel);
};

#endif
