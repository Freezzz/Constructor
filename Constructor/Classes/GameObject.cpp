//
//  GameObject.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "GameObject.h"
#include "GameWorld.h"

#define PTM_RATIO 32.0f
//////////////////////////////////////////////////// 
// GameObject init
//////////////////////////////////////////////////// 
bool GameObject::init(){
	return true;
}

void GameObject::createBody(){

}

void GameObject::setSelected(bool selected){
    if (selected) {
        this->objectSprite->setColor(ccc3(255, 255, 0));
    }else {
        this->objectSprite->setColor(ccc3(255, 255, 255));
    }
}

void GameObject::rotate(float newRotation){
    if (this->getParent() && objectBody) {
		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2Position = b2Vec2(this->getPosition().x/PTM_RATIO,
		                           this->getPosition().y/PTM_RATIO);
		float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS(newRotation);
        
        this->setRotation(newRotation);        
		objectBody->SetTransform(b2Position, b2Angle);
        
	}
}

void GameObject::move(cocos2d::CCPoint newPostion){
	if (this->getParent() && objectBody) {
		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2Position = b2Vec2(newPostion.x/PTM_RATIO,
		                           newPostion.y/PTM_RATIO);
		float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS(this->getRotation());

        this->setPosition(newPostion);        
		objectBody->SetTransform(b2Position, b2Angle);

	}
}

void GameObject::update(ccTime dt){
    if (this->getParent() && objectBody) {
        // Update posisiotn of node moving it to body postion
        this->setPosition( CCPointMake( objectBody->GetPosition().x * PTM_RATIO, objectBody->GetPosition().y * PTM_RATIO) );
        this->setRotation( -1 * CC_RADIANS_TO_DEGREES(objectBody->GetAngle()) );
    }
}

void GameObject::destroy(){
    GameWorld::sharedGameWorld()->physicsWorld->DestroyBody(this->objectBody);	
    this->removeFromParentAndCleanup(true);
}

//////////////////////////////////////////////////// 
// Static constructor returns autorelesed object
//////////////////////////////////////////////////// 
GameObject* GameObject::node(){
	GameObject * pRet = new GameObject();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}