//
//  MainMenuScene.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/23/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "StoryLevelsScene.h"
#include "GameLevelScene.h"
#include "MainMenuScene.h"
#include "LevelManager.h"
#include "UIButton.h"

//////////////////////////////////////////////////// 
// MainMenuScene init
//////////////////////////////////////////////////// 
bool StoryLevelsScene::init(){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	CCSprite * bg = CCSprite::spriteWithFile("mainmenu.png");
	addChild(bg);
	bg->setPosition(CCPoint(winSize.width*0.5, winSize.height*0.5));
	
	CCLabelTTF * maiMenuLabel = CCLabelTTF::labelWithString("MainMenu", "Arial", 28);
	CCMenuItemLabel * newGameItem = CCMenuItemLabel::itemWithLabel(maiMenuLabel, this, menu_selector(StoryLevelsScene::mainMenuTap));
	newGameItem->setPosition(CCPoint(120, 50));
	
	CCMenu * menu = CCMenu::menuWithItems(newGameItem, NULL);
	addChild(menu);
	menu->setPosition(CCPointZero);

	
	vector<StoryLevelDescribtion*> * levels = LevelManager::sharedManager()->getStoryLevelList();
	for (int i = 0; i < 20; i++) {
		UIButton * btn = UIButton::node("1-1", this, menu_selector(StoryLevelsScene::levelSelectedTap));
		menu->addChild(btn,i);
		btn->setPosition(CCPoint(200+(i%8* 100), 500 - (int(i/8) * 100)));
	}
	

	return true;
}


void StoryLevelsScene::levelSelectedTap(cocos2d::CCObject *sender){
	CCDirector::sharedDirector()->replaceScene(GameLevelScene::scene());	
}

void StoryLevelsScene::mainMenuTap(cocos2d::CCObject *sender){
	CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
}

//////////////////////////////////////////////////// 
// Static factory creation methods
//////////////////////////////////////////////////// 
CCScene* StoryLevelsScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	StoryLevelsScene *layer = StoryLevelsScene::node();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}