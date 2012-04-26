//
//  ObjectSimpleBox.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectSimpleBox_H__
#define __ObjectSimpleBox_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;

class SimpleBoxInventoryItem : public InventoryItem
{
private:
	ObjectType m_type;
	bool init( );
	bool init( std::string itemPath, std::string spritePath, b2FixtureDef *fixtureDef );

public:
	// the fixture
	b2FixtureDef *m_fixtureDef;

private:
	SimpleBoxInventoryItem( ) : InventoryItem(SimpleBox) { }
	SimpleBoxInventoryItem( ObjectType type ) : InventoryItem(type), m_type(type) { }

public:
	GameObject* gameObjectNode( b2Body *b );
	GameObject* gameObjectNode( CCPoint p );

	static SimpleBoxInventoryItem *node( std::string itemPath, std::string spritePath, b2FixtureDef *fixtureDef ) {
		SimpleBoxInventoryItem *r = new SimpleBoxInventoryItem();
		if( r && r->init( itemPath, spritePath, fixtureDef ) ) {
			r->autorelease();
			return r;
		}
		delete r;
		return NULL;
	}

};

////////////////////////////////////////////////////
// ObjectSimpleBox
///////////////////////////////////////////////////
class ObjectSimpleBox : public GameObject {
protected:
	bool init( std::string spritePath, b2FixtureDef *fixtureDef );
	b2FixtureDef *m_fixtureDef;

    void createBodyAtPosition( CCPoint position );
	
public:
	static ObjectSimpleBox *node( InventoryItem *item, b2Body *b, std::string spritePath, b2FixtureDef *fixtureDef ) {
		ObjectSimpleBox *r = new ObjectSimpleBox;
		if( r && r->init( spritePath, fixtureDef ) ) {
			r->m_inventoryItem = item;
			r->setBody( b );
			r->autorelease();
			return r;
		}
		delete r;
		return NULL;
	}
	static ObjectSimpleBox *node( InventoryItem *item, CCPoint p, std::string spritePath, b2FixtureDef *fixtureDef ) {
		ObjectSimpleBox *r = new ObjectSimpleBox;
		if( r && r->init( spritePath, fixtureDef ) ) {
			r->m_inventoryItem = item;
			r->createBodyAtPosition( p );
			r->autorelease();
			return r;
		}
		delete r;
		return NULL;
	}
};

#endif
