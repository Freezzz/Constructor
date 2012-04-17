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
    this->addChild(bg);
    
    // Game World
    gameWorld = GameWorld::node();
    this->addChild(gameWorld);
    
    // Invetory
    _inventoryLayer = InventoryLayer::node();
    _inventoryLayer->setPosition(CCPoint(0, winSize.height*0.5));
    this->addChild(_inventoryLayer);
    
    // Game Menu
    CreationLayer * creationLayer = CreationLayer::node();
    this->addChild(creationLayer, 10);
    
    this->_gameObjects  = new CCMutableArray<GameObject*>();
    this->_gameZoneRect = CCRect(100, 30, winSize.width-100, winSize.height-70);
    this->_isInEditMode = true;
    
    gameSceneInstance = this;
	return true;
}

//////////////////////////////////////////////////// 
// Starts world simulation
////////////////////////////////////////////////////
void GameLevelScene::runWorld(){
    _isInEditMode = false;
    _inventoryLayer->setOnScreen(false);    
    for (int i = 0; i < _gameObjects->count(); i++) {
		_gameObjects->getObjectAtIndex(i)->setObjectState(simulating);
    }
}

//////////////////////////////////////////////////// 
// Pause world simulation without reseting initial state
//////////////////////////////////////////////////// 
void GameLevelScene::pauseWorld(){
    _isInEditMode = true;
    _inventoryLayer->setOnScreen(true);
    for (int i = 0; i < _gameObjects->count(); i++) {
		_gameObjects->getObjectAtIndex(i)->setObjectState(idile);
    }
}


//////////////////////////////////////////////////// 
// Restore original GameObjects postions before simulation
//////////////////////////////////////////////////// 
void GameLevelScene::resetWorld(){    
    gameWorld->physicsWorld->ClearForces();    
    this->pauseWorld();
}

//////////////////////////////////////////////////// 
// Deletes all GameObjects from world
//////////////////////////////////////////////////// 
void GameLevelScene::wipeWorld(){
    this->pauseWorld();  
    gameWorld->physicsWorld->ClearForces();    
    for (int i = 0; i < _gameObjects->count(); i++) {
		_gameObjects->getObjectAtIndex(i)->destroy();
    }
    _gameObjects->removeAllObjects();
}



//////////////////////////////////////////////////// 
// Screen Touch delegates - touch started
//////////////////////////////////////////////////// 
bool GameLevelScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (!_isInEditMode ) {
        return false;
    }
    CCPoint location = pTouch->locationInView(pTouch->view());
    location = CCDirector::sharedDirector()->convertToGL(location);
    
	// Check if need to create new object
	GameObject * newObject = _inventoryLayer->getGameObjectForTapLocation(location);
    if (newObject != NULL) {
		_gameObjects->addObject(newObject);		
		this->addChild(newObject);		
        newObject->setObjectState(moving);
        newObject->setSelected(true);     
		newObject->move(location);		
		_selectedObject = newObject;		
        return true;
    }

	
    // If touch is in game zone look for touched object
    if (CCRect::CCRectContainsPoint(_gameZoneRect, location)) {
        
		// Take in account threshold, to make easier touch selection
//        CCRect touchZone = CCRect(location.x - TOUCH_TRESHOLD*0.5, location.y- TOUCH_TRESHOLD*0.5, TOUCH_TRESHOLD, TOUCH_TRESHOLD);
        for (int i = 0; i < _gameObjects->count(); i++) {
            GameObject * tmp = (GameObject*)_gameObjects->getObjectAtIndex(i);
//			if (CCRect::CCRectIntersectsRect(touchZone, tmp->boundingBox())) {				
			if (CCRect::CCRectContainsPoint(tmp->boundingBox(), location)) {			
                _selectedObject = tmp;
                _selectedObject->setSelected(true);
                _selectedObject->setObjectState(moving);
				_selectedObject->move(location);				
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
    if (_selectedObject == NULL) {
        return;
    }
    CCPoint location = pTouch->locationInView(pTouch->view());
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    _selectedObject->move(location);
}

//////////////////////////////////////////////////// 
// Screen Touch delegates - touch finished
//////////////////////////////////////////////////// 
void GameLevelScene::ccTouchEnded(CCTouch *pTouch, CCEvent* pEvent){
    if (_selectedObject == NULL) {
        return;
    }
    
    CCPoint location = pTouch->locationInView(pTouch->view());
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    // If touch is not in game zone
    if (!CCRect::CCRectContainsPoint(_gameZoneRect, location)) {
		_gameObjects->removeObject(_selectedObject);
        _selectedObject->destroy();
        _selectedObject=NULL;        
        return;
    }
    
    _selectedObject->move(location);
    _selectedObject->setSelected(false);
    _selectedObject->setObjectState(idile);
	CCLog("GameLevelScene::ccTouchEnded SEL Size: %f, %f", _selectedObject->getContentSize().width, _selectedObject->getContentSize().height);					
	
//    _selectedObject = NULL;
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

