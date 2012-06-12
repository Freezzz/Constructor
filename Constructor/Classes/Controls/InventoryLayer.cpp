//
//  InventoryLayer.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/14/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "InventoryLayer.h"
#include "../GameObjects/GameObject.h"
#include "../Serialization/LevelManager.h"
#include "../Serialization/ConstructorJSon.h"
#include <iostream>
#include <sstream>
#define BUTTON_SIZE 90


bool InventoryItem::init( )
{
	// sprite
	{
		m_sprite = CCSprite::spriteWithFile( m_spritePath.c_str() );
		setContentSize( m_sprite->getContentSize() );
		addChild( m_sprite );
	}

	// quantity label
	{
		m_quantityLabel = CCLabelTTF::labelWithString( "", "Arial", 22 );
		addChild( m_quantityLabel );
	}

	

	return 1;
}
GameObject *InventoryItem::spawnObject( CCPoint p )
{
	if( m_quantity >= m_maxQuantity ) {
		return 0;
	}

	std::cout << "Spawning " << m_prototypeName << "..." << std::endl;
	GameObject *obj = ConstructorJSon::j2cGameObject( m_prototypeName, p );

	if( obj ) {
		obj->m_inventoryItem = this;
		m_quantity ++;
		updateQuantityLabel( );
	}
	return obj;
}
void InventoryItem::updateQuantityLabel()
{
	string str;
	//int fontSize = 22;
	if( m_maxQuantity > 0 ) {
		stringstream q;
		q << ( m_maxQuantity - m_quantity );
		str = q.str();
	}
	else {
		str = "∞";
		//fontSize = 26;
	}

	m_quantityLabel->setString( str.c_str() );
	m_quantityLabel->setPosition( CCPoint(30, -30) );
	m_quantityLabel->setColor( ccc3(0, 255, 0) );
	m_quantityLabel->setAnchorPoint( CCPoint(0, 0) );
}

////////////////////////////////////////////////////
// InventoryLayer init
////////////////////////////////////////////////////
bool InventoryLayer::init( )
{
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
	for( it = m_inventoryItems.begin(); it != m_inventoryItems.end(); ++ it ) {
		InventoryItem *button = *it;

		/*
		std::cout << "  Item " << button->getName() << " at: " << CCRect::CCRectGetMidX( button->boundingBox() ) << "," << CCRect::CCRectGetMidY( button->boundingBox() )
				  << " : " << button->boundingBox().size.width << "x" << button->boundingBox().size.height << std::endl;
		*/

		CCPoint point = CCPoint( pointLocal.x - button->boundingBox().origin.x, pointLocal.y - button->boundingBox().origin.y );
		//std::cout << "  Fake click: " << point.x << "," << point.y << std::endl;

		if( CCRect::CCRectContainsPoint( button->m_sprite->boundingBox(), point ) ) {
			button->runAction( CCBlink::actionWithDuration(0.2, true) );
			GameObject *r = button->spawnObject( location );
			return r;
		}
	}
	
	return NULL;
}

void InventoryLayer::addInventoryItem( InventoryItem *item )
{
	m_inventoryItems.push_back( item );

	item->setPosition( CCPoint(35, BUTTON_SIZE * (4-(int)m_inventoryItems.size()) ) );
	item->updateQuantityLabel();
	addChild( item );
}
void InventoryLayer::removeInventoryItem( InventoryItem *item )
{
	// removing from m_inventoryItems
	m_inventoryItems.erase( std::remove( m_inventoryItems.begin(), m_inventoryItems.end(), item ), m_inventoryItems.end() );
	removeChild( item, 1 );
}
