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

////////////////////////////////////////////////////
// GameLevelScene - tat manages user interaction 
// with game world
///////////////////////////////////////////////////
class GameLevelScene : public CCLayer {
private:
    GameObject * _selectedObject;
	
	CCMutableArray<GameObject*>* _gameObjects;
    CCRect _gameZoneRect;
    
	InventoryLayer * _inventoryLayer;
    bool _isInEditMode;
    
    virtual void registerWithTouchDispatcher();
	virtual bool init();    
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
    
    bool isInEditMode(){return _isInEditMode;}

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

