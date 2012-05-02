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
class LevelDescription;

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
	CreationLayer * m_creationLayer;
	VictoryLayer * m_victoryLayer;
	bool m_isInEditMode;

	CCSprite * m_deleteButton;
	CCSprite * m_rotareButton;

	LevelDescription *m_levelFile;

	int m_touchCount;
	int m_firstTouchID;
	int m_secondTouchID;
	
	// Multitouch rotation vars
	float m_initialTouchAngle;
	float m_initialObjectAngle;

public:
	enum GameState
	{
		Editing,
		Simulating,
		Victory,
		Defeat
	} m_gameState;

	GameWorld * gameWorld;
	GameObject * m_target;
	GameObject * m_winArea;

public:
	void enterEditing();
	void enterSimulating();
	void enterVictory();
	void enterDefeat();

	bool isEditing( ) const { return m_gameState == Editing; }
	bool isSimulating( ) const { return m_gameState == Simulating; }
	bool isVictory( ) const { return m_gameState == Victory; }
	bool isDefeat( ) const { return m_gameState == Defeat; }
	bool isGameOver( ) const { return isVictory() || isDefeat(); }

	GameLevelScene( )
		: m_selectedObject(0), m_levelFile(0)
	{
	}
	bool init( LevelDescription *level );
	void registerWithTouchDispatcher( );

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

protected:
	////////////////////////////////////////////////////
	// Deletes all GameObjects from world
	////////////////////////////////////////////////////
	void wipeWorld();

public:
	////////////////////////////////////////////////////
	// Checks whether the user won or lost
	////////////////////////////////////////////////////
	bool checkVictory();
	bool checkDefeat();
	void update(ccTime dt);

	void reloadLevel();
	void saveFile( LevelDescription *level );
	void loadLevel( LevelDef *ld );
	void loadFile( LevelDescription *level );
	LevelDef* getCurrentLevelDef();

	////////////////////////////////////////////////////
	// Screen Touch delegates
	////////////////////////////////////////////////////
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent* pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent* pEvent);
	
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

	////////////////////////////////////////////////////
	// Static factory creation methods
	////////////////////////////////////////////////////
	static cocos2d::CCScene* scene( LevelDescription * level );
	static GameLevelScene* nodeWithLevel( LevelDescription *level );

	////////////////////////////////////////////////////
	// Singleton pattern
	////////////////////////////////////////////////////
	static GameLevelScene* gameSceneInstance;
	static GameLevelScene* sharedGameScene();
    
};

