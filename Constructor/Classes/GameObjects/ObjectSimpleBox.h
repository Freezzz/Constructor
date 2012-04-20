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

	SimpleBoxInventoryItem( ObjectType type ) : m_type(type) { }

public:
	GameObject* gameObjectNode( );

	// TODO: instead of type, let node be fed with the physical
	//       and graphical properties of the inventory item
	//       and of the relative game object.
	//       ObjectType will have "SimpleObject" then for any
	//       instance of this class...
	static SimpleBoxInventoryItem *node( ObjectType type ) {
		SimpleBoxInventoryItem *r = new SimpleBoxInventoryItem(type);
		if( r && r->init() ) {
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
	virtual bool init();

	ObjectType m_simpleType;
	ObjectSimpleBox( ObjectType type ) {
		m_simpleType = type;
	}
	
public:    
    void createBodyAtPosition(CCPoint position);
    
	static ObjectSimpleBox *node( ObjectType type ) {
		ObjectSimpleBox *r = new ObjectSimpleBox(type);
		if( r && r->init() ) {
			r->autorelease();
			return r;
		}
		delete r;
		return NULL;
	}
};

#endif
