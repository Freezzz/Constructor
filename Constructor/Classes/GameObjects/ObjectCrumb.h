//
//  ObjectCrumb.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectCrumb_H__
#define __ObjectCrumb_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;


////////////////////////////////////////////////////
// ObjectCrumb
///////////////////////////////////////////////////
class ObjectCrumb : public GameObject {
private:
	bool m_canSee;
	
	bool m_isEaten;
	
	void setIsEaten( bool value );
protected:
	CCSprite *m_sprite;
	b2Body *m_body;

    ObjectCrumb(const string& fileName, const Json::Value& prototype)
	: GameObject(fileName, prototype) { }
	bool init( );
    bool createBodyAtPosition( CCPoint position );
	
	void update( ccTime dt );
public:
	GAMEOBJECT_NODE_DEF( ObjectCrumb )
	
	bool isEaten(){return m_isEaten;}
	
	//////////////////////////////////////////////////// 
	// Object collided notification
	// @pararm: other Object of the collision
	//////////////////////////////////////////////////// 
	virtual void objectCollided(GameObject * otherObject, b2Contact * contact);
	
	//////////////////////////////////////////////////// 
	// Object collision ended notification
	// @pararm: other Object of the collision
	//////////////////////////////////////////////////// 
	virtual void objectCollisionEnded(GameObject * otherObject, b2Contact * contact);
	
	//////////////////////////////////////////////////// 
	// Callback on simulation started
	//////////////////////////////////////////////////// 	
	virtual void onSimulationStarted();
	
	//////////////////////////////////////////////////// 
	// Callback on simulation ended
	//////////////////////////////////////////////////// 
	virtual void onSimulationEnded();
	
	void draw();
};

#endif
