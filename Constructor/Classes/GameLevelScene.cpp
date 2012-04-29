//
//  GameLevelScene.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/6/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "GameLevelScene.h"
#include "GameWorld.h"

#include "GameObjects/ObjectSimpleBox.h"
#include "GameObjects/ObjectSpring.h"

#include "Controls/InventoryLayer.h"
#include "Controls/VictoryLayer.h"
#include "Controls/CreationLayer.h"
#include "Constants.h"

#include "Serialization/LevelManager.h"

#include "LevelDef.h"

#include <iostream>

#define TOUCH_TRESHOLD 20

//////////////////////////////////////////////////// 
// GameLevelScene init
//////////////////////////////////////////////////// 
bool GameLevelScene::init( const char *file )
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	setIsTouchEnabled( true );
	setIsAccelerometerEnabled( true );

	m_touchCount = 0;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize( winSize );

	// Background
	CCSprite * bg = CCSprite::spriteWithFile("blueprints_bg.png");
	bg->setPosition(CCPoint(winSize.width*0.5, winSize.height*0.5));
	addChild(bg);

	// Invetory
	m_inventoryLayer = InventoryLayer::node();
	m_inventoryLayer->setPosition(CCPoint(0, winSize.height*0.5));
	addChild( m_inventoryLayer );

	// Game Menu
	m_creationLayer = CreationLayer::node();
	addChild( m_creationLayer, 10 );

	m_deleteButton = CCSprite::spriteWithFile("delete_btn.png");
	this->addChild(m_deleteButton, 100);
	m_deleteButton->setIsVisible(false);


	m_rotareButton = CCSprite::spriteWithFile("rotate_btn.png");
	this->addChild(m_rotareButton, 100);
	m_rotareButton->setIsVisible(false);


	m_gameObjects  = new CCMutableArray<GameObject*>();
	m_gameZoneRect = CCRect(100, 30, winSize.width-100, winSize.height-70);
	m_isInEditMode = true;

	gameSceneInstance = this;

	// initializing the victory layer
	m_victoryLayer = VictoryLayer::node();
	m_victoryLayer->setPosition( CCPoint(winSize.width*0.5, winSize.height*0.5) );
	m_victoryLayer->setScale( 0 );
	addChild( m_victoryLayer, 1000 );

	// loading the level
	// not using loadFile because it also resets the current state
	// but that would give problems, since it's not initialized yet.
	LevelDef *ld = LevelDef::loadFromFile( file );
	m_levelFile = strdup( file );
	loadLevel( ld );

	scheduleUpdate();
	return true;
}

void GameLevelScene::enterEditing( )
{
	m_gameState = Editing;
	m_inventoryLayer->setOnScreen( true );
	m_creationLayer->setOnScreen( true );
	m_creationLayer->enterEditing();
	m_victoryLayer->setOnScreen( false );
	setUtilityButtonsVisibleFoSelectedObject( true );
}
void GameLevelScene::enterSimulating( )
{
	m_gameState = Simulating;
	m_inventoryLayer->setOnScreen( false );
	m_creationLayer->setOnScreen( true );
	m_creationLayer->enterSimulating();
	m_victoryLayer->setOnScreen( false );
	setUtilityButtonsVisibleFoSelectedObject( false );
}
void GameLevelScene::enterVictory( )
{
	LevelManager::sharedManager()->completeUserLevel(m_levelFile);

	m_gameState = Victory;
	m_inventoryLayer->setOnScreen( false );
	m_creationLayer->setOnScreen( false );
	m_victoryLayer->setOnScreen( true );
	setUtilityButtonsVisibleFoSelectedObject( false );
}
void GameLevelScene::enterDefeat( )
{
	CCAssert( false, "not implemented..." );
}

//////////////////////////////////////////////////// 
// Starts world simulation
////////////////////////////////////////////////////
void GameLevelScene::runWorld( )
{
	enterSimulating();

	for (unsigned int i = 0; i < m_gameObjects->count(); i++) {
		m_gameObjects->getObjectAtIndex(i)->setObjectState( GameObject::Simulating );
	}
	if (m_selectedObject) {
		m_selectedObject->setSelected(false);
	}
}

//////////////////////////////////////////////////// 
// Pause world simulation without reseting initial state
//////////////////////////////////////////////////// 
void GameLevelScene::pauseWorld( )
{
	enterEditing();

	for (unsigned int i = 0; i < m_gameObjects->count(); i++) {
		m_gameObjects->getObjectAtIndex(i)->setObjectState( GameObject::Idile );
	}

	// If before simulation objec was selecteed restore selction and buttons
	if (m_selectedObject) {
		m_selectedObject->setSelected(true);
	}
}


//////////////////////////////////////////////////// 
// Restore original GameObjects postions before simulation
//////////////////////////////////////////////////// 
void GameLevelScene::resetWorld( )
{    
	gameWorld->physicsWorld->ClearForces();    
	pauseWorld();
}

//////////////////////////////////////////////////// 
// Deletes all GameObjects from world
//////////////////////////////////////////////////// 
void GameLevelScene::wipeWorld( )
{
	pauseWorld();
	gameWorld->physicsWorld->ClearForces();
	for (unsigned int i = 0; i < m_gameObjects->count(); i++) {
		m_gameObjects->getObjectAtIndex(i)->destroy();
	}
	m_gameObjects->removeAllObjects();
	m_selectedObject = NULL;
}
void GameLevelScene::reloadLevel( )
{
	// reinitializing the level
	loadFile( m_levelFile );
}

bool GameLevelScene::checkVictory()
{
	if( ! isSimulating() ) {
		return 0;
	}

	b2ContactEdge * cont = m_winArea->m_objectBody->GetContactList();
	while( cont ) {
		if( cont->contact->GetFixtureA()->GetBody()->GetUserData() == m_target || cont->contact->GetFixtureB()->GetBody()->GetUserData() == m_target ) {
			return 1;
		}
		cont = cont->next;
	}
	return 0;
}
bool GameLevelScene::checkDefeat()
{
	if( ! isSimulating() ) {
		return 0;
	}

	return 0;
}
void GameLevelScene::update( ccTime dt )
{
	if( isSimulating() ) {
		if( checkVictory() ) {
			enterVictory();
		}
		if( checkDefeat() ) {
			enterDefeat();
		}
	}
}

//////////////////////////////////////////////////// 
// Returns current level defenition
//////////////////////////////////////////////////// 
LevelDef* GameLevelScene::getCurrentLevelDef( )
{
	LevelDef *ld = new LevelDef;
	ld->name = "test level";
	ld->difficulty = 1;
	ld->theme = "test theme";
	ld->gameWorld = gameWorld;
	ld->inventoryItems = m_inventoryLayer->m_buttons;
	ld->gameObjects.addObjectsFromArray( m_gameObjects );
	ld->target = m_target;
	ld->winArea = m_winArea;
	ld->winConditions = LevelDef::EnterAreaWin;
	ld->loseConditions = LevelDef::EnterAreaLose;
	return ld;
}
void GameLevelScene::saveFile( const char *file )
{
	getCurrentLevelDef()->saveToFile( file );
}
void GameLevelScene::loadLevel( LevelDef *ld )
{
	gameWorld = ld->gameWorld;
	addChild( gameWorld );

	// inventory items
	{
		// adding new ones
		{
			vector<InventoryItem*> invItems = ld->inventoryItems;
			for( unsigned int i = 0; i < invItems.size(); ++i ) {
				m_inventoryLayer->addInventoryItem( invItems.at(i) );
			}
		}
	}

	// game objects
	{
		// adding new ones
		{
			m_gameObjects->addObjectsFromArray( & ld->gameObjects );
			for( unsigned int i = 0; i < m_gameObjects->count(); ++i ) {
				GameObject *object = m_gameObjects->getObjectAtIndex(i);
				addChild( object, object->defaultZOrder );
			}
		}
	}
	m_target = ld->target;
	m_winArea = ld->winArea;

	enterEditing();
}
void GameLevelScene::loadFile( const char *file )
{
	m_levelFile = strdup( file );

	// removing inventory items
	{
		while( ! m_inventoryLayer->m_buttons.empty() ) {
			m_inventoryLayer->removeInventoryItem( m_inventoryLayer->m_buttons.at(0) );
		}
	}
	wipeWorld(); // removing former objects
	removeChild( gameWorld, 1 );

	LevelDef *ld = LevelDef::loadFromFile( file );
	loadLevel( ld );
}


//////////////////////////////////////////////////// 
// Screen Touch delegates - touch started
//////////////////////////////////////////////////// 
bool GameLevelScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if ( ! isEditing() ) {
        return false;
    }
	
	if(!(
	   (m_touchCount == 0 ) || // FIRST TOUCH
	   (m_touchCount == 1 && m_selectedObject && (m_selectedObject->m_state == GameObject::Moving ||
	    m_selectedObject->m_state == GameObject::Rotating) && m_selectedObject->isRotatable)// Object selected and rotatable
	   )){
		CCLog("ALREADY TOUCHING skiping touch! %d", m_touchCount);
		return false;
	}
	m_touchCount++;
	CCLog("NEW TOUCH! %d", m_touchCount);

	CCPoint location = pTouch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

	if (m_touchCount == 1) {
		m_firstTouchID = pTouch->m_uID;
	}else if( m_selectedObject->isRotatable){
		m_secondTouchID = pTouch->m_uID;
		m_selectedObject->setObjectState( GameObject::Rotating );
		m_selectedObject->rotate(location);
		return true;
	}
	

	// If user taped on utility buttons
	if (tapUtilityButtons(location)) {
		return true;
	}
	
	if (m_selectedObject != NULL) {
		setUtilityButtonsVisibleFoSelectedObject(false);
		m_selectedObject->setSelected(false);
		m_selectedObject = NULL;
	}
	
	// Check if need to create new object
	GameObject * newObject = m_inventoryLayer->getGameObjectForTapLocation(location);
    if (newObject != NULL) {
		m_gameObjects->addObject(newObject);		
		addChild(newObject, newObject->defaultZOrder);		
        newObject->setObjectState( GameObject::Moving );
        newObject->setSelected(true);     
		newObject->move(location);		
		m_selectedObject = newObject;
        return true;
    }

	
    // If touch is in game zone look for touched object
    if (CCRect::CCRectContainsPoint(m_gameZoneRect, location)) {
        
		// Search for selected object taking in account z-order
        for (unsigned int i = 0; i < m_gameObjects->count(); i++) {
            GameObject * tmp = (GameObject*)m_gameObjects->getObjectAtIndex(i);
			if (CCRect::CCRectContainsPoint(tmp->boundingBox(), location)) {
				if (m_selectedObject == NULL) {
					m_selectedObject = tmp;
					continue;
				}
				if (m_selectedObject->defaultZOrder < tmp->defaultZOrder) {
					m_selectedObject = tmp;
				}
            }
        }
		if (!m_selectedObject) {
			return true;
		}
		m_selectedObject->setSelected(true);
		if( m_selectedObject->isMovable ) {
			m_selectedObject->setObjectState( GameObject::Moving );
			m_selectedObject->move(location);
		}
		setUtilityButtonsVisibleFoSelectedObject(true);
		return true;
    }
    return true;
}

//////////////////////////////////////////////////// 
// Screen Touch delegates - touch moved
//////////////////////////////////////////////////// 
void GameLevelScene::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    if (m_selectedObject == NULL) {
        return;
    }
    CCPoint location = pTouch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
	setUtilityButtonsVisibleFoSelectedObject(false);
	
	if (m_selectedObject->m_state == GameObject::Rotating && m_touchCount == 2 && (int) pTouch->m_uID == m_secondTouchID) {
		double radians = atan2(m_selectedObject->getPosition().x - location.x, m_selectedObject->getPosition().y -location.y
						); //this grabs the radians for us
		
		m_selectedObject->rotate(CC_RADIANS_TO_DEGREES(radians));
		return;
	}
	
	if (m_selectedObject->m_state == GameObject::Moving) {
		m_selectedObject->move(location);
	}else if (m_selectedObject->m_state == GameObject::Rotating) {
		m_selectedObject->rotate(location);
	}
}

//////////////////////////////////////////////////// 
// Screen Touch delegates - touch finished
//////////////////////////////////////////////////// 
void GameLevelScene::ccTouchEnded( CCTouch *pTouch, CCEvent* pEvent )
{
	m_touchCount--;	
	
	CCLog("TOUCH ENDED! %d id: %d", m_touchCount, pTouch->m_uID);
	// Initial finger is still taped
	if (m_touchCount == 1 && (int) pTouch->m_uID == m_secondTouchID) {
		m_selectedObject->setObjectState( GameObject::Idile);
		m_selectedObject->setObjectState( GameObject::Moving );
		return;
	}

    if (m_selectedObject == NULL) {
        return;
    }
    
    CCPoint location = pTouch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    // If touch is not in game zone
    if (!CCRect::CCRectContainsPoint(m_gameZoneRect, location) && m_selectedObject->m_state == GameObject::Moving) {
		m_gameObjects->removeObject(m_selectedObject);
        m_selectedObject->destroy();
        m_selectedObject=NULL;        
        return;
    }
    m_selectedObject->setObjectState( GameObject::Idile );
	setUtilityButtonsVisibleFoSelectedObject(true);
}

void GameLevelScene::ccTouchCancelled( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCLog("TOUCH CANCELLED!");
}

//////////////////////////////////////////////////// 
// Checks if user taped on one of ultility bottons
// and performs tapped button operation
// returns true if button was activated, false otherwise 
//////////////////////////////////////////////////// 
bool GameLevelScene::tapUtilityButtons( cocos2d::CCPoint location )
{
	if( ! isEditing() ) {
		return false;
	}

	if (m_selectedObject && m_deleteButton->getIsVisible() && CCRect::CCRectContainsPoint(m_deleteButton->boundingBox(), location)) {
		m_selectedObject->setSelected(false);
		m_gameObjects->removeObject(m_selectedObject);
		m_selectedObject->destroy();
		setUtilityButtonsVisibleFoSelectedObject(false);
		m_selectedObject = NULL;
		return true;	
	}
	if (m_selectedObject && m_rotareButton->getIsVisible() && CCRect::CCRectContainsPoint(m_rotareButton->boundingBox(), location)) {
		m_selectedObject->setSelected(true);
		m_selectedObject->setObjectState( GameObject::Rotating );
		m_selectedObject->rotate(location);
		return true;	
	}	
	return false;
}

//////////////////////////////////////////////////// 
// Shows and hides utility buttons near selected object
//////////////////////////////////////////////////// 
void GameLevelScene::setUtilityButtonsVisibleFoSelectedObject( bool visibility )
{
	if (!visibility || !m_selectedObject) {
		m_deleteButton->setIsVisible(false);
		m_rotareButton->setIsVisible(false);
		return;
	}
	if (m_selectedObject && m_selectedObject->m_state == GameObject::Idile) {
		if (m_selectedObject->isRotatable) {
			m_rotareButton->setIsVisible(true);
			m_rotareButton->setPosition(CCPoint(m_selectedObject->getPosition().x + m_selectedObject->rotateButtonOffset.x, m_selectedObject->getPosition().y + m_selectedObject->rotateButtonOffset.y));							
		}				
		if (m_selectedObject->isDeletable) {
			m_deleteButton->setIsVisible(true);
			m_deleteButton->setPosition(CCPoint(m_selectedObject->getPosition().x + m_selectedObject->deleteButtonOffset.x, m_selectedObject->getPosition().y + m_selectedObject->deleteButtonOffset.y));							
		}
	}
}


void GameLevelScene::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

//////////////////////////////////////////////////// 
// Static factory creation methods
//////////////////////////////////////////////////// 
CCScene* GameLevelScene::scene( const char *file )
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node( );

	// 'layer' is an autorelease object
	GameLevelScene *layer = GameLevelScene::nodeWithLevel( file );

	// add layer as a child to scene
	scene->addChild( layer );

	// return the scene
	return scene;
}
GameLevelScene* GameLevelScene::nodeWithLevel( const char *file )
{
	GameLevelScene *r = new GameLevelScene;
	if( r && r->init( file ) ) {
		r->autorelease();
		return r;
	}

	delete r;
	return NULL;
}

//////////////////////////////////////////////////// 
// Singleton pattern
////////////////////////////////////////////////////     
GameLevelScene* GameLevelScene::gameSceneInstance = NULL;
GameLevelScene* GameLevelScene::sharedGameScene( )
{
	CCAssert(gameSceneInstance!=NULL, "Game world not yet initialized");
	return gameSceneInstance;
}

