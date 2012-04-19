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
#include <iostream>

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
    addInventoryItem( SimpleBoxInventoryItem::node() );
    addInventoryItem( FixedPointInventoryItem::node() );
    addInventoryItem( PanelInventoryItem::node() );
    addInventoryItem( SpringInventoryItem::node() );
    
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
// Creates object if tap location is on any button,
// null othewise
//////////////////////////////////////////////////// 
GameObject* InventoryLayer::getGameObjectForTapLocation(CCPoint location){
	CCPoint pointLocal = CCPoint(location.x - getPosition().x, location.y - getPosition().y);
	
	//std::cout << "Clicked: " << pointLocal.x << "," << pointLocal.y << std::endl;
	
	vector<InventoryItem*>::iterator it;
	for( it = m_buttons.begin(); it != m_buttons.end(); ++ it ) {
		InventoryItem *button = *it;
		
		/*
		std::cout << "  Item at: " << CCRect::CCRectGetMidX( button->boundingBox() ) << "," << CCRect::CCRectGetMidY( button->boundingBox() )
				  << " : " << button->m_objectSprite->boundingBox().size.width << "x" << button->m_objectSprite->boundingBox().size.height << std::endl;
		*/
				  
		CCPoint point = CCPoint( pointLocal.x - button->boundingBox().origin.x, pointLocal.y - button->boundingBox().origin.y );
		// std::cout << "  Fake click: " << point.x << "," << point.y << std::endl;
		
		if( CCRect::CCRectContainsPoint( button->m_objectSprite->boundingBox(), point ) ) {
			button->runAction( CCBlink::actionWithDuration(0.2, true) );
			GameObject *r = button->gameObjectNodeV();
			r->createBodyAtPosition( location );
			return r;
		}
	}
	
    return NULL;
}

void InventoryLayer::addInventoryItem( InventoryItem *item )
{
    item->setPosition( CCPoint(35, BUTTON_SIZE * (1-(int)m_buttons.size()) ) );
    m_buttons.push_back( item );
    addChild( item );
}


