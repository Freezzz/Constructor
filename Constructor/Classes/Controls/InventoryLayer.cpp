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
	setAnchorPoint(CCPoint(0,0.5));

    // Background images
    CCSprite * bg = CCSprite::spriteWithFile("inventory_bg.png");
	setContentSize(bg->getContentSize());
	addChild(bg);
    bg->setAnchorPoint(CCPoint(0,0.5));
    
    
    // Buttons
    m_boxButton = CCSprite::spriteWithFile("box.png");
    m_boxButton->setPosition(CCPoint(35, BUTTON_SIZE * 1));
    addChild(m_boxButton);
    
    
    m_pointButton = CCSprite::spriteWithFile("fixed.png");
    m_pointButton->setPosition(CCPoint(35, BUTTON_SIZE * 0));
    addChild(m_pointButton);
    
    
    m_panelButton= CCSprite::spriteWithFile("panel_btn.png");
    m_panelButton->setPosition(CCPoint(35, BUTTON_SIZE * -1));
    addChild(m_panelButton);
    
    m_springButton = CCSprite::spriteWithFile("spring_btn.png");
    m_springButton->setPosition(CCPoint(35, BUTTON_SIZE * -2));
    addChild(m_springButton);
    
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
ObjectType InventoryLayer::getObjectTypeForTapCoordinates(CCPoint location){
    CCPoint pointLocal = CCPoint(location.x - getPosition().x, location.y - getPosition().y);        
    if (CCRect::CCRectContainsPoint(m_boxButton->boundingBox(), pointLocal)) {
        m_boxButton->runAction(CCBlink::actionWithDuration(0.2, true));
        return SimpleBox;
    }
    if (CCRect::CCRectContainsPoint(m_pointButton->boundingBox(), pointLocal)) {
        m_pointButton->runAction(CCBlink::actionWithDuration(0.2, true));        
        return FixedPoint;
    }
    if (CCRect::CCRectContainsPoint(m_panelButton->boundingBox(), pointLocal)) {
        m_panelButton->runAction(CCBlink::actionWithDuration(0.2, true));        
        return Panel;
    }
    if (CCRect::CCRectContainsPoint(m_springButton->boundingBox(), pointLocal)) {
        m_springButton->runAction(CCBlink::actionWithDuration(0.2, true));        
        return Spring;
    }
    
    
    return Undefined;
}

//////////////////////////////////////////////////// 
// Creates object if tap location is on any button,
// null othewise
//////////////////////////////////////////////////// 
GameObject* InventoryLayer::getGameObjectForTapLocation(CCPoint location){
	CCPoint pointLocal = CCPoint(location.x - getPosition().x, location.y - getPosition().y);        
    if (CCRect::CCRectContainsPoint(m_boxButton->boundingBox(), pointLocal)) {
        m_boxButton->runAction(CCBlink::actionWithDuration(0.2, true));
		ObjectSimpleBox * ret = ObjectSimpleBox::node();
		ret->createBodyAtPosition(location);
        return ret;
    }
    if (CCRect::CCRectContainsPoint(m_pointButton->boundingBox(), pointLocal)) {
        m_pointButton->runAction(CCBlink::actionWithDuration(0.2, true));        
		ObjectFixedPoint * ret = ObjectFixedPoint::node();
		ret->createBodyAtPosition(location);
        return ret;
    }
    if (CCRect::CCRectContainsPoint(m_panelButton->boundingBox(), pointLocal)) {
        m_panelButton->runAction(CCBlink::actionWithDuration(0.2, true));        
		ObjectPanel * ret = ObjectPanel::node();
		ret->createBodyAtPosition(location);
        return ret;
    }
    if (CCRect::CCRectContainsPoint(m_springButton->boundingBox(), pointLocal)) {
        m_springButton->runAction(CCBlink::actionWithDuration(0.2, true));        
		ObjectSpring * ret = ObjectSpring::node();
		ret->createBodyAtPosition(location);
        return ret;

    }
    return NULL;
}


