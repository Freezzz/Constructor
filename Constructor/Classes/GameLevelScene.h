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
class CreationLayer;
class VictoryLayer;
class b2Contact;
class LevelDef;

////////////////////////////////////////////////////
// GameLevelScene - tat manages user interaction 
// with game world
///////////////////////////////////////////////////
class GameLevelScene : public CCLayer {
private:
    GameObject * m_selectedObject;
	
	CCMutableArray<GameObject*> * m_gameObjects;
	GameObject * m_target;
	
    CCRect m_gameZoneRect;
    
	InventoryLayer * m_inventoryLayer;
	CreationLayer * m_creationLayer;
	VictoryLayer * m_victoryLayer;
    bool m_isInEditMode;
    
    virtual void registerWithTouchDispatcher();
	virtual bool init();
	
	CCSprite * m_moveButton;
	CCSprite * m_deleteButton;
	CCSprite * m_rotareButton;
	bool m_gameOver;

	LevelDef *m_levelDef;

public:
	GameWorld * gameWorld;

	// initializing the level
	bool initLevel( );

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
	// Checks whether the user won or lost
	////////////////////////////////////////////////////
	bool checkVictory();
	bool checkDefeat();
	void update(ccTime dt);

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

