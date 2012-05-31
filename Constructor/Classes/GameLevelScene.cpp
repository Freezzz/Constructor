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
#include "GameObjects/ObjectArea.h"

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
bool GameLevelScene::init( LevelDescription *level )
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

	m_winArea = NULL;
	m_looseArea = NULL;
    
	gameSceneInstance = this;

	// initializing the victory layer
	m_victoryLayer = VictoryLayer::node();
	m_victoryLayer->setPosition( CCPoint(winSize.width*0.5, winSize.height*0.5) );
	m_victoryLayer->setScale( 0 );
	addChild( m_victoryLayer, 1000 );

	// loading the level
	// not using loadFile because it also resets the current state
	// but that would give problems, since it's not initialized yet.
	LevelDef *ld = LevelDef::loadFromFile( level );
	m_levelFile = level;
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
	m_gameState = Defeat;
	m_inventoryLayer->setOnScreen( false );
	m_creationLayer->setOnScreen( false );
	m_victoryLayer->setOnScreen( true );
	setUtilityButtonsVisibleFoSelectedObject( false );
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

	// TODO: add win condition check
	if ( ! m_winArea ) {
		return 0;
	}
	
	b2ContactEdge * cont = m_winArea->m_objectBody->GetContactList();
	while( cont ) {
		if( ( cont->contact->GetFixtureA()->GetBody()->GetUserData() == m_target
			 || cont->contact->GetFixtureB()->GetBody()->GetUserData() == m_target )
		   && cont->contact->IsTouching() ) {
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
	
	// TODO: add loose condition check
	if ( ! m_looseArea ) {
		return 0;
	}
	
	b2ContactEdge * cont = m_looseArea->m_objectBody->GetContactList();
	while( cont ) {
		if( ( cont->contact->GetFixtureA()->GetBody()->GetUserData() == m_target
			 || cont->contact->GetFixtureB()->GetBody()->GetUserData() == m_target )
		   && cont->contact->IsTouching() ) {
			return 1;
		}
		cont = cont->next;
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
	ld->inventoryItems = m_inventoryLayer->m_inventoryItems;
	ld->gameObjects.addObjectsFromArray( m_gameObjects );
	ld->target = m_target;
	ld->winArea = m_winArea;
	ld->looseArea = m_looseArea;
	ld->winConditions = LevelDef::EnterAreaWin;
	ld->loseConditions = LevelDef::EnterAreaLose;
	return ld;
}
void GameLevelScene::saveFile( LevelDescription *level )
{
	getCurrentLevelDef()->saveToFile( level );
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
	if (m_winArea) {
		((ObjectArea*)m_winArea)->setAreaType(WinArea);
	}
	
	m_looseArea = ld->looseArea;
	if (m_looseArea) {
		((ObjectArea*)m_looseArea)->setAreaType(LooseArea);
	}

	
	enterEditing();
}
void GameLevelScene::loadFile( LevelDescription *level )
{
	m_levelFile = level;

	wipeWorld(); // removing former objects
	removeChild( gameWorld, 1 );
	
	// removing inventory items
	{
		while( ! m_inventoryLayer->m_inventoryItems.empty() ) {
			m_inventoryLayer->removeInventoryItem( m_inventoryLayer->m_inventoryItems.at(0) );
		}
	}

	LevelDef *ld = LevelDef::loadFromFile( level );
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

	CCPoint location = pTouch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	if (m_touchCount == 1) {
		m_firstTouchID = pTouch->m_uID;
	}else if( m_selectedObject->isRotatable){
		m_secondTouchID = pTouch->m_uID;
		m_selectedObject->setObjectState( GameObject::Idile );		
		m_selectedObject->setObjectState( GameObject::Rotating );
		m_initialTouchAngle=atan2(m_selectedObject->getPosition().x - location.x, m_selectedObject->getPosition().y -location.y);
		m_initialObjectAngle = m_selectedObject->m_objectBody->GetAngle();
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
	
	
	// Search for selected object taking in account z-order
	for (unsigned int i = 0; i < m_gameObjects->count(); i++) {
		GameObject * tmp = (GameObject*)m_gameObjects->getObjectAtIndex(i);
		if (tmp->containsPoint(location)) {
			if (m_selectedObject == NULL) {
				m_selectedObject = tmp;
				continue;
			}
			if (m_selectedObject->defaultZOrder < tmp->defaultZOrder) {
				m_selectedObject = tmp;
			}
		}
	}
	
	// If nothing was taped and there were no selected object ignore tap
	if (!m_selectedObject) {
		return true;
	}
	
	m_selectedObject->setSelected(true);
	if( m_selectedObject->isMovable ) {
		m_touchToCenterOffset = CCPoint(location.x - m_selectedObject->getPosition().x, location.y - m_selectedObject->getPosition().y);
		m_selectedObject->setObjectState( GameObject::Moving );
	}
	setUtilityButtonsVisibleFoSelectedObject(true);
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
	
	// Multitouch rotation
	if (m_selectedObject->m_state == GameObject::Rotating && m_touchCount == 2 && (int) pTouch->m_uID == m_secondTouchID) {
		double radians = atan2(m_selectedObject->getPosition().x - location.x, m_selectedObject->getPosition().y -location.y
							   ); //this grabs the radians for us
		
		m_selectedObject->rotate(-1*CC_RADIANS_TO_DEGREES(m_initialObjectAngle+(m_initialTouchAngle-radians)));
		return;
	}
	
	if (m_selectedObject->m_state == GameObject::Moving) {
		m_selectedObject->move(CCPoint(location.x - m_touchToCenterOffset.x, location.y - m_touchToCenterOffset.y));
	}else if (m_selectedObject->m_state == GameObject::Rotating && m_touchCount == 1) { // Button rotation
		double radians = atan2(m_selectedObject->getPosition().x - location.x, m_selectedObject->getPosition().y -location.y
							   ); //this grabs the radians for us
		
		m_selectedObject->rotate(-1*CC_RADIANS_TO_DEGREES(m_initialObjectAngle+(m_initialTouchAngle-radians)));
	}
}

//////////////////////////////////////////////////// 
// Screen Touch delegates - touch finished
//////////////////////////////////////////////////// 
void GameLevelScene::ccTouchEnded( CCTouch *pTouch, CCEvent* pEvent )
{
	m_touchCount--;	
	
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
    if( ! CCRect::CCRectContainsPoint(m_gameZoneRect, location) && m_selectedObject->m_state == GameObject::Moving && m_selectedObject->isDeletable ) {
		InventoryItem * item = m_selectedObject->m_inventoryItem;		
		m_gameObjects->removeObject(m_selectedObject);
        m_selectedObject->destroy();
		m_inventoryLayer->updateInventryItemQuantity(item);
        m_selectedObject=NULL;        
        return;
    }
    m_selectedObject->setObjectState( GameObject::Idile );
	
	m_selectedObject->startUnstuckPhase();
	runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.3),
								  CCCallFunc::actionWithTarget(m_selectedObject,
															   callfunc_selector(GameObject::unstuckPhaseFinished)), NULL));
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
		InventoryItem * item = m_selectedObject->m_inventoryItem;
		m_selectedObject->setSelected(false);
		m_gameObjects->removeObject(m_selectedObject);
		m_selectedObject->destroy();
		m_inventoryLayer->updateInventryItemQuantity(item);
		setUtilityButtonsVisibleFoSelectedObject(false);
		m_selectedObject = NULL;
		return true;	
	}
	if (m_selectedObject && m_rotareButton->getIsVisible() && CCRect::CCRectContainsPoint(m_rotareButton->boundingBox(), location)) {
		m_selectedObject->setSelected(true);
		m_selectedObject->setObjectState( GameObject::Rotating );
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
CCScene* GameLevelScene::scene( LevelDescription *level )
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node( );

	// 'layer' is an autorelease object
	GameLevelScene *layer = GameLevelScene::nodeWithLevel( level );

	// add layer as a child to scene
	scene->addChild( layer );

	// return the scene
	return scene;
}
GameLevelScene* GameLevelScene::nodeWithLevel( LevelDescription *level )
{
	GameLevelScene *r = new GameLevelScene;
	if( r && r->init( level ) ) {
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

