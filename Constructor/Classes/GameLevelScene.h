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
class CreationLayer;
class GameObject;
////////////////////////////////////////////////////
// GameLevelScene
///////////////////////////////////////////////////
class GameLevelScene : public CCLayer {
private:
	virtual bool init();
	CreationLayer * creationLayer;
    bool _isInEditMode;
    vector<GameObject*> * _gameObjects;
    
    GameObject * selectedObject;
    virtual void registerWithTouchDispatcher();
public:
    GameWorld * gameWorld;
    
    void runWorld();
    void pauseWorld();
    void resetWorld();
    void wipeWorld();
    
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent* pEvent);    
    
    bool isInEditMode(){return _isInEditMode;}
    
    static CCPoint alignPointToGrid(CCPoint point);
    
	static cocos2d::CCScene* scene();    
	LAYER_NODE_FUNC(GameLevelScene);
	static GameLevelScene* gameSceneInstance;
	static GameLevelScene* sharedGameScene();
    
};

