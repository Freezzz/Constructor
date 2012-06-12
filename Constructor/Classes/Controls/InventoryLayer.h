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
#include "Serialization/json/json.h" // for the prototype

class GameObject;
using namespace cocos2d;


// a button in the inventory
class InventoryItem : public CCNode
{
public:
	std::string m_fileName; // the file where this button is defined
	std::string m_name;

	std::string m_spritePath, m_prototypeName;

	CCSprite *m_sprite;

	int m_quantity, m_maxQuantity;
	CCLabelTTF *m_quantityLabel;

protected:
	InventoryItem( const std::string &fileName, const std::string &name, const std::string &prototypeName, const std::string &spritePath )
	: m_fileName(fileName), m_name(name), m_spritePath(spritePath), m_prototypeName(prototypeName), m_quantity(0) { }

public:
	GameObject *spawnObject( CCPoint p );
	std::string getName() const { return m_name; }
	bool init( );
	void updateQuantityLabel( );

	static InventoryItem *node( const std::string &fileName, const std::string &name, const std::string &prototypeName, const std::string &spritePath ) {
		InventoryItem *r = new InventoryItem( fileName, name, prototypeName, spritePath );
		if( r && r->init() ) {
			r->autorelease();
			return r;
		}
		delete r;
		return NULL;
	}
};

////////////////////////////////////////////////////
// InventoryLayer - layer that represents 
// inventrory of player
///////////////////////////////////////////////////
class InventoryLayer : public CCLayer {
private:
	virtual bool init();

public:
	vector<InventoryItem*> m_inventoryItems;
	
public:
	////////////////////////////////////////////////////
	// Shows and hides inventory
	////////////////////////////////////////////////////
	void setOnScreen(bool isOnscreen);

	////////////////////////////////////////////////////
	// Creates object if tap location is on any button,
	// null othewise
	////////////////////////////////////////////////////
	GameObject* getGameObjectForTapLocation(CCPoint location);

	void addInventoryItem( InventoryItem *item );
	void removeInventoryItem( InventoryItem *item );

	LAYER_NODE_FUNC(InventoryLayer);
};

#endif
