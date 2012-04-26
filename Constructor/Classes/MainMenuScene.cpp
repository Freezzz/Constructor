//
//  MainMenuScene.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/23/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "MainMenuScene.h"
#include "GameLevelScene.h"
#include "StoryLevelsScene.h"

//////////////////////////////////////////////////// 
// MainMenuScene init
//////////////////////////////////////////////////// 
bool MainMenuScene::init(){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	CCSprite * bg = CCSprite::spriteWithFile("mainmenu.png");
	addChild(bg);
	bg->setPosition(CCPoint(winSize.width*0.5, winSize.height*0.5));
	
	CCLabelTTF * newGameLabel = CCLabelTTF::labelWithString("New Game", "Arial", 28);
	CCLabelTTF * sandboxLabel = CCLabelTTF::labelWithString("SandBox", "Arial", 28);
	
	CCMenuItemLabel * newGameItem = CCMenuItemLabel::itemWithLabel(newGameLabel, this, menu_selector(MainMenuScene::newGameTap));
	CCMenuItemLabel * sandboxItem = CCMenuItemLabel::itemWithLabel(sandboxLabel, this, menu_selector(MainMenuScene::sanboxtap));
	
	CCMenu * menu = CCMenu::menuWithItems(newGameItem, sandboxItem, NULL);
	addChild(menu);
	menu->setPosition(CCPointZero);
	
	newGameItem->setPosition(CCPoint(winSize.width*0.5 - 200, winSize.height*0.5-100));
	sandboxItem->setPosition(CCPoint(winSize.width*0.5 + 200, winSize.height*0.5-100));
	return true;
}


void MainMenuScene::newGameTap(cocos2d::CCObject *sender){
	CCDirector::sharedDirector()->replaceScene(StoryLevelsScene::scene());
}

void MainMenuScene::sanboxtap(cocos2d::CCObject *sender){
	CCDirector::sharedDirector()->replaceScene(GameLevelScene::scene());
}

//////////////////////////////////////////////////// 
// Static factory creation methods
//////////////////////////////////////////////////// 
CCScene* MainMenuScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	MainMenuScene *layer = MainMenuScene::node();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}