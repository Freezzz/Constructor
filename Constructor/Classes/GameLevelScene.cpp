//
//  GameLevelScene.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/6/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "GameLevelScene.h"
#include "GameWorld.h"
#include "CreationLayer.h"
#include "ObjectSimpleBox.h"

#define GRID_SIZE 50
#define TOUCH_TRESHOLD GRID_SIZE * 1.7

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
    
    gameWorld = GameWorld::node();
    this->addChild(gameWorld);
    
    creationLayer = CreationLayer::node();
    this->addChild(creationLayer);
    
    _gameObjects = new vector<GameObject*>();
    _isInEditMode = true;
    
    gameSceneInstance = this;
	return true;
}

void GameLevelScene::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameLevelScene::runWorld(){
    _isInEditMode = false;
    for (int i = 0; i < _gameObjects->size(); i++) {
        GameObject * tmp = _gameObjects->at(i);
        tmp->originalPosition = tmp->getPosition();
        tmp->originalRotation = tmp->getRotation();
        tmp->objectBody->SetAwake(true);
    }
    gameWorld->runWorld();
}

void GameLevelScene::pauseWorld(){
    _isInEditMode = true;
    gameWorld->pauseWorld();
}


void GameLevelScene::resetWorld(){
    this->pauseWorld();
    gameWorld->physicsWorld->ClearForces();
    for (int i = 0; i < _gameObjects->size(); i++) {
        GameObject * tmp = _gameObjects->at(i);
        tmp->move(tmp->originalPosition);
        tmp->rotate(tmp->originalRotation);
    }
}

void GameLevelScene::wipeWorld(){
    this->pauseWorld();  
    gameWorld->physicsWorld->ClearForces();    
    for (int i = 0; i < _gameObjects->size(); i++) {
        GameObject * tmp = _gameObjects->at(i);
        tmp->destroy();
    }
    _gameObjects->clear();
}

CCPoint GameLevelScene::alignPointToGrid(cocos2d::CCPoint point){
    int x =  ((int)(point.x / GRID_SIZE)) * GRID_SIZE;
    int y =  ((int)(point.y / GRID_SIZE)) * GRID_SIZE;    
    return CCPoint(x, y);
}



bool GameLevelScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (!_isInEditMode) {
        return false;
    }
    CCPoint location = pTouch->locationInView(pTouch->view());
    
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    CCPoint locationAligned = alignPointToGrid(location);
    
    CCRect touchZone = CCRect(locationAligned.x, locationAligned.y, TOUCH_TRESHOLD, TOUCH_TRESHOLD);
    for (int i = 0; i < _gameObjects->size(); i++) {
        GameObject * tmp = _gameObjects->at(i);
        if (CCRect::CCRectContainsPoint(touchZone, tmp->getPosition())) {
            selectedObject = tmp;
            selectedObject->unscheduleUpdate();   
            selectedObject->setSelected(true);
            return true;
        }
    }
    
    selectedObject = (ObjectSimpleBox*)ObjectSimpleBox::node();
    ((ObjectSimpleBox*)selectedObject)->createBodyAtPosition(location);
    this->addChild(selectedObject);
    _gameObjects->push_back(selectedObject);
    selectedObject->unscheduleUpdate();
    selectedObject->setSelected(true);    
    return true;
}

void GameLevelScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if (selectedObject == NULL) {
        return;
    }
    CCPoint location = pTouch->locationInView(pTouch->view());
    location = CCDirector::sharedDirector()->convertToGL(location);

    selectedObject->move(location);
}

void GameLevelScene::ccTouchEnded(CCTouch *pTouch, CCEvent* pEvent){
    if (selectedObject == NULL) {
        return;
    }
    CCPoint location = pTouch->locationInView(pTouch->view());
    location = CCDirector::sharedDirector()->convertToGL(location);

	location = alignPointToGrid(location);
    selectedObject->scheduleUpdate();
    selectedObject->move(location);
    selectedObject->setSelected(false);    
    selectedObject = NULL;
}

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

GameLevelScene* GameLevelScene::gameSceneInstance = NULL;
GameLevelScene* GameLevelScene::sharedGameScene(){
	CCAssert(gameSceneInstance!=NULL, "Game world not yet initialized");
	return gameSceneInstance;
}

