//
//  InventoryLayer.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/14/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "InventoryLayer.h"
#include "../GameObjects/GameObject.h"
#include "../GameObjects/ObjectFixedPoint.h"
#include "../GameObjects/ObjectSimpleBox.h"
#include "../GameObjects/ObjectSpring.h"
#include "../GameObjects/ObjectPanel.h"

#define BUTTON_SIZE 90

//////////////////////////////////////////////////// 
// InventoryLayer init
//////////////////////////////////////////////////// 
bool InventoryLayer::init(){
    // Set achor point to middle left
    this->setAnchorPoint(CCPoint(0,0.5));

    // Background images
    CCSprite * bg = CCSprite::spriteWithFile("inventory_bg.png");
    this->setContentSize(bg->getContentSize());
    this->addChild(bg);
    bg->setAnchorPoint(CCPoint(0,0.5));
    
    
    // Buttons
    boxButton = CCSprite::spriteWithFile("box.png");
    boxButton->setPosition(CCPoint(35, BUTTON_SIZE * 1));
    this->addChild(boxButton);
    
    
    pointButton = CCSprite::spriteWithFile("fixed.png");
    pointButton->setPosition(CCPoint(35, BUTTON_SIZE * 0));
    this->addChild(pointButton);
    
    
    panelButton= CCSprite::spriteWithFile("panel_btn.png");
    panelButton->setPosition(CCPoint(35, BUTTON_SIZE * -1));
    this->addChild(panelButton);
    
    springButton = CCSprite::spriteWithFile("spring_btn.png");
    springButton->setPosition(CCPoint(35, BUTTON_SIZE * -2));
    this->addChild(springButton);
    
	return true;
}

//////////////////////////////////////////////////// 
// Shows and hides inventory 
//////////////////////////////////////////////////// 
void InventoryLayer::setOnScreen(bool isOnscreen){
    CCPoint location = this->getPosition();
    location.x = 0; // assuming it will be always on the left side of the screen
    if (!isOnscreen) {
        location.x -= this->getContentSize().width;
    }
    this->runAction(CCMoveTo::actionWithDuration(0.5, location));
}


//////////////////////////////////////////////////// 
// Returns wich button type was tapped 
//////////////////////////////////////////////////// 
ObjectType InventoryLayer::getObjectTypeForTapCoordinates(cocos2d::CCPoint location){
    CCPoint pointLocal = CCPoint(location.x - this->getPosition().x, location.y - this->getPosition().y);        
    if (CCRect::CCRectContainsPoint(boxButton->boundingBox(), pointLocal)) {
        boxButton->runAction(CCBlink::actionWithDuration(0.2, true));
        return simpleBox;
    }
    if (CCRect::CCRectContainsPoint(pointButton->boundingBox(), pointLocal)) {
        pointButton->runAction(CCBlink::actionWithDuration(0.2, true));        
        return fixedPoint;
    }
    if (CCRect::CCRectContainsPoint(panelButton->boundingBox(), pointLocal)) {
        panelButton->runAction(CCBlink::actionWithDuration(0.2, true));        
        return panel;
    }
    if (CCRect::CCRectContainsPoint(springButton->boundingBox(), pointLocal)) {
        springButton->runAction(CCBlink::actionWithDuration(0.2, true));        
        return spring;
    }
    
    
    return undefined;
}

//////////////////////////////////////////////////// 
// Creates object if tap location is on any button,
// null othewise
//////////////////////////////////////////////////// 
GameObject* InventoryLayer::getGameObjectForTapLocation(cocos2d::CCPoint location){
	CCPoint pointLocal = CCPoint(location.x - this->getPosition().x, location.y - this->getPosition().y);        
    if (CCRect::CCRectContainsPoint(boxButton->boundingBox(), pointLocal)) {
        boxButton->runAction(CCBlink::actionWithDuration(0.2, true));
		ObjectSimpleBox * ret = ObjectSimpleBox::node();
		ret->createBodyAtPosition(location);
        return ret;
    }
    if (CCRect::CCRectContainsPoint(pointButton->boundingBox(), pointLocal)) {
        pointButton->runAction(CCBlink::actionWithDuration(0.2, true));        
		ObjectFixedPoint * ret = ObjectFixedPoint::node();
		ret->createBodyAtPosition(location);
        return ret;
    }
    if (CCRect::CCRectContainsPoint(panelButton->boundingBox(), pointLocal)) {
        panelButton->runAction(CCBlink::actionWithDuration(0.2, true));        
		ObjectPanel * ret = ObjectPanel::node();
		ret->createBodyAtPosition(location);
        return ret;
    }
    if (CCRect::CCRectContainsPoint(springButton->boundingBox(), pointLocal)) {
        springButton->runAction(CCBlink::actionWithDuration(0.2, true));        
		ObjectSpring * ret = ObjectSpring::node();
		ret->createBodyAtPosition(location);
        return ret;

    }
    return NULL;
}


