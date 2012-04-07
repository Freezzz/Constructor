//
//  GameObject.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __GameObject_H__
#define __GameObject_H__

#include "cocos2d.h"
#include "Box2D.h"
using namespace cocos2d;


////////////////////////////////////////////////////
// GameObject
///////////////////////////////////////////////////
class GameObject : public CCNode {
protected:
    
	// Methods
	virtual bool init();
	virtual void createBody();
    virtual void update(ccTime dt);
public:
	// Properties
	// Properties
    b2Body * objectBody;
    CCSprite * objectSprite;
    
    CCPoint originalPosition;
    float originalRotation;
    

    virtual void move(CCPoint newPostion);
    virtual void rotate(float newRotation);
    virtual void destroy();
    
    void setSelected(bool selected);
    
    static GameObject* node();
};

#endif
