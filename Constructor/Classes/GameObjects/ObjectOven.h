//
//  ObjectOven.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectOven_H__
#define __ObjectOven_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;


////////////////////////////////////////////////////
// ObjectOven
///////////////////////////////////////////////////
class ObjectOven : public GameObject {
private:
	std::vector<GameObject*> m_affectedObjects;
protected:
	CCSprite *m_sprite;
	CCSprite *m_flameSprite;
	
	b2Body *m_body;
	b2Body *m_flameSensor;
	b2Body *m_buttonSensor;
	
	CCPoint m_flameSensorOffset;
	CCPoint m_buttonSensorOffset;
	
	bool m_isOn;

    ObjectOven(const string& fileName, const Json::Value& prototype)
	: GameObject(fileName, prototype) { }
	bool init( );
    bool createBodyAtPosition( CCPoint position );
	
public:
	GAMEOBJECT_NODE_DEF( ObjectOven )
	
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
	// Callback on simulation ended
	//////////////////////////////////////////////////// 
	virtual void onSimulationEnded();
	
	////////////////////////////////////////////////////
	// Moves object to new location, if state is idile
	// than it is a simple translation, if moving than
	// creates a move joint to move object around
	////////////////////////////////////////////////////
	virtual void move(CCPoint newPostion);
		
	bool getIsOn(){return m_isOn;}
	void setIsOn( bool value );
};

#endif
