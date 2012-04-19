//
//  GameLevelScene.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/6/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "cocos2d.h"
using namespace cocos2d;

class GameWorld;
class GameObject;
class InventoryLayer;
class b2Contact;
////////////////////////////////////////////////////
// GameLevelScene - tat manages user interaction 
// with game world
///////////////////////////////////////////////////
class GameLevelScene : public CCLayer {
private:
    GameObject * m_selectedObject;
	
	CCMutableArray<GameObject*> * m_gameObjects;
    CCRect m_gameZoneRect;
    
	InventoryLayer * m_inventoryLayer;
    bool m_isInEditMode;
    
    virtual void registerWithTouchDispatcher();
	virtual bool init();
	
	CCSprite * m_moveButton;
	CCSprite * m_deleteButton;
	CCSprite * m_rotareButton;	
	
public:
    GameWorld * gameWorld;
    
    //////////////////////////////////////////////////// 
    // Starts world simulation
    //////////////////////////////////////////////////// 
    void runWorld();

    //////////////////////////////////////////////////// 
    // Pause world simulation without reseting initial state
    //////////////////////////////////////////////////// 
    void pauseWorld();
    
    //////////////////////////////////////////////////// 
    // Restore original GameObjects postions before simulation
    //////////////////////////////////////////////////// 
    void resetWorld();
    
    //////////////////////////////////////////////////// 
    // Deletes all GameObjects from world
    //////////////////////////////////////////////////// 
    void wipeWorld();
    
    
    //////////////////////////////////////////////////// 
    // Screen Touch delegates
    //////////////////////////////////////////////////// 
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent* pEvent);  
    
	//////////////////////////////////////////////////// 
	// Checks if user taped on one of ultility bottons
	// and performs tapped button operation
	// returns true if button was activated, false otherwise 
	//////////////////////////////////////////////////// 
	bool tapUtilityButtons(CCPoint location);
	
	//////////////////////////////////////////////////// 
	// Shows and hides utility buttons near selected object
	//////////////////////////////////////////////////// 
	void setUtilityButtonsVisibleFoSelectedObject(bool visibility);
	
	
    bool isInEditMode(){return m_isInEditMode;}

    //////////////////////////////////////////////////// 
    // Static factory creation methods
    //////////////////////////////////////////////////// 
	static cocos2d::CCScene* scene();    
	LAYER_NODE_FUNC(GameLevelScene);
    
    //////////////////////////////////////////////////// 
    // Singleton pattern
    ////////////////////////////////////////////////////     
	static GameLevelScene* gameSceneInstance;
	static GameLevelScene* sharedGameScene();
    
};

