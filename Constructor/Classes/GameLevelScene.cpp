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
#include "GameObjects/ObjectFixedPoint.h"
#include "GameObjects/ObjectPanel.h"
#include "GameObjects/ObjectSpring.h"

#include "Controls/InventoryLayer.h"
#include "Controls/CreationLayer.h"
#include "Constants.h"

#define TOUCH_TRESHOLD 20

//////////////////////////////////////////////////// 
// GameLevelScene init
//////////////////////////////////////////////////// 
bool GameLevelScene::init(){
    if ( !CCLayer::init() )
	{
		return false;
	}
	setIsTouchEnabled( true );
	setIsAccelerometerEnabled( true );
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // Background
    CCSprite * bg = CCSprite::spriteWithFile("blueprints_bg.png");
    bg->setPosition(CCPoint(winSize.width*0.5, winSize.height*0.5));
	addChild(bg);
    
    // Game World
    gameWorld = GameWorld::node();
	addChild(gameWorld);
    
    // Invetory
    m_inventoryLayer = InventoryLayer::node();
    m_inventoryLayer->setPosition(CCPoint(0, winSize.height*0.5));
	addChild(m_inventoryLayer);
    
    // Game Menu
    CreationLayer * creationLayer = CreationLayer::node();
	addChild(creationLayer, 10);
    
	m_gameObjects  = new CCMutableArray<GameObject*>();
	m_gameZoneRect = CCRect(100, 30, winSize.width-100, winSize.height-70);
	m_isInEditMode = true;
    
    gameSceneInstance = this;
	return true;
}

//////////////////////////////////////////////////// 
// Starts world simulation
////////////////////////////////////////////////////
void GameLevelScene::runWorld(){
    m_isInEditMode = false;
    m_inventoryLayer->setOnScreen(false);    
    for (int i = 0; i < m_gameObjects->count(); i++) {
		m_gameObjects->getObjectAtIndex(i)->setObjectState(Simulating);
    }
}

//////////////////////////////////////////////////// 
// Pause world simulation without reseting initial state
//////////////////////////////////////////////////// 
void GameLevelScene::pauseWorld(){
    m_isInEditMode = true;
    m_inventoryLayer->setOnScreen(true);
    for (int i = 0; i < m_gameObjects->count(); i++) {
		m_gameObjects->getObjectAtIndex(i)->setObjectState(Idile);
    }
}


//////////////////////////////////////////////////// 
// Restore original GameObjects postions before simulation
//////////////////////////////////////////////////// 
void GameLevelScene::resetWorld(){    
    gameWorld->physicsWorld->ClearForces();    
	pauseWorld();
}

//////////////////////////////////////////////////// 
// Deletes all GameObjects from world
//////////////////////////////////////////////////// 
void GameLevelScene::wipeWorld(){
	pauseWorld();  
    gameWorld->physicsWorld->ClearForces();    
    for (int i = 0; i < m_gameObjects->count(); i++) {
		m_gameObjects->getObjectAtIndex(i)->destroy();
    }
    m_gameObjects->removeAllObjects();
}



//////////////////////////////////////////////////// 
// Screen Touch delegates - touch started
//////////////////////////////////////////////////// 
bool GameLevelScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (!m_isInEditMode ) {
        return false;
    }
    CCPoint location = pTouch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
	// Check if need to create new object
	GameObject * newObject = m_inventoryLayer->getGameObjectForTapLocation(location);
    if (newObject != NULL) {
		m_gameObjects->addObject(newObject);		
		addChild(newObject);		
        newObject->setObjectState(Moving);
        newObject->setSelected(true);     
		newObject->move(location);		
		m_selectedObject = newObject;		
        return true;
    }

	
    // If touch is in game zone look for touched object
    if (CCRect::CCRectContainsPoint(m_gameZoneRect, location)) {
        
		// Take in account threshold, to make easier touch selection
//        CCRect touchZone = CCRect(location.x - TOUCH_TRESHOLD*0.5, location.y- TOUCH_TRESHOLD*0.5, TOUCH_TRESHOLD, TOUCH_TRESHOLD);
        for (int i = 0; i < m_gameObjects->count(); i++) {
            GameObject * tmp = (GameObject*)m_gameObjects->getObjectAtIndex(i);
//			if (CCRect::CCRectIntersectsRect(touchZone, tmp->boundingBox())) {				
			if (CCRect::CCRectContainsPoint(tmp->boundingBox(), location)) {			
                m_selectedObject = tmp;
                m_selectedObject->setSelected(true);
                m_selectedObject->setObjectState(Moving);
				m_selectedObject->move(location);				
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////// 
// Screen Touch delegates - touch moved
//////////////////////////////////////////////////// 
void GameLevelScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if (m_selectedObject == NULL) {
        return;
    }
    CCPoint location = pTouch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    m_selectedObject->move(location);
}

//////////////////////////////////////////////////// 
// Screen Touch delegates - touch finished
//////////////////////////////////////////////////// 
void GameLevelScene::ccTouchEnded(CCTouch *pTouch, CCEvent* pEvent){
    if (m_selectedObject == NULL) {
        return;
    }
    
    CCPoint location = pTouch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    // If touch is not in game zone
    if (!CCRect::CCRectContainsPoint(m_gameZoneRect, location)) {
		m_gameObjects->removeObject(m_selectedObject);
        m_selectedObject->destroy();
        m_selectedObject=NULL;        
        return;
    }
    
    m_selectedObject->move(location);
    m_selectedObject->setSelected(false);
    m_selectedObject->setObjectState(Idile);
	
//    m_selectedObject = NULL;
}

void GameLevelScene::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

//////////////////////////////////////////////////// 
// Static factory creation methods
//////////////////////////////////////////////////// 
CCScene* GameLevelScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	GameLevelScene *layer = GameLevelScene::node();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

//////////////////////////////////////////////////// 
// Singleton pattern
////////////////////////////////////////////////////     
GameLevelScene* GameLevelScene::gameSceneInstance = NULL;
GameLevelScene* GameLevelScene::sharedGameScene(){
	CCAssert(gameSceneInstance!=NULL, "Game world not yet initialized");
	return gameSceneInstance;
}

