//
//  InventoryLayer.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/14/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "InventoryLayer.h"
#include "../GameObjects/GameObject.h"
#include <iostream>
#include <sstream>
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
	
	return true;
}

//////////////////////////////////////////////////// 
// Shows and hides inventory 
//////////////////////////////////////////////////// 
void InventoryLayer::setOnScreen(bool isOnscreen){
	CCPoint location = getPosition();
	location.x = 0; // assuming it will be always on the left side of the screen
	if (!isOnscreen) {
		location.x -= getContentSize().width;
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
			GameObject *r = button->gameObjectNode( location );
			if (r) {
				updateInventryItemQuantity(button);
			}
			return r;
		}
	}
	
	return NULL;
}

void InventoryLayer::addInventoryItem( InventoryItem *item, int )
{
	item->setPosition( CCPoint(35, BUTTON_SIZE * (2-(int)m_buttons.size()) ) );
	m_buttons.push_back( item );
	addChild( item );

	
	// Quantity counters
	string str;
	int fontSize = 22;
	if (item->m_maxQuantity > 0) {
		stringstream q;
		q << item->m_maxQuantity;
		str = q.str();
	}else {
		str = "∞";
		fontSize = 26;		
	}
	
	CCLabelTTF * quantityLabel = CCLabelTTF::labelWithString(str.c_str(), "Arial", fontSize);
	quantityLabel->setPosition(CCPoint(45, BUTTON_SIZE * (3-(int)m_buttons.size())-25) );
    quantityLabel->setColor(ccc3(0, 255, 0));
	quantityLabel->setAnchorPoint(CCPoint(0, 0));
	m_quantityLabels.push_back(quantityLabel);
	addChild(quantityLabel);
}
void InventoryLayer::removeInventoryItem( InventoryItem *item )
{
	int itemIndex = -1;
	for( unsigned int i = 0; i < m_buttons.size(); ++i ) {
		if (item == m_buttons.at(i)) {
			itemIndex = i;
			break;
		}
	}
	m_buttons.erase( m_buttons.begin()+itemIndex );
	removeChild( item, 1 );
	
	removeChild(m_quantityLabels.at(itemIndex), true);
	m_quantityLabels.erase(m_quantityLabels.begin()+itemIndex);
}


void InventoryLayer::updateInventryItemQuantity(InventoryItem *item){
	if (item->m_maxQuantity <= 0) {
		return;
	}
	int itemIndex = -1;
	for( unsigned int i = 0; i< m_buttons.size(); ++i ) {
		if (item == m_buttons.at(i)) {
			itemIndex = i;
			break;
		}
	}
	stringstream q;
	q << item->m_maxQuantity - item->m_quantity;
	m_quantityLabels.at(itemIndex)->setString(q.str().c_str());
    if (item->m_maxQuantity - item->m_quantity == 0) {
        m_quantityLabels.at(itemIndex)->setColor(ccc3(255, 0, 0));
    }else {
        m_quantityLabels.at(itemIndex)->setColor(ccc3(0, 255, 0));
    }

}


