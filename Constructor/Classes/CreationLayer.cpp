//
//  CreationLayer.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "CreationLayer.h"
#include "GameLevelScene.h"

//////////////////////////////////////////////////// 
// CreationLayer init
//////////////////////////////////////////////////// 
bool CreationLayer::init(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    labelPause = CCLabelTTF::labelWithString("Edit", "Arial", 30);
	CCMenuItemLabel * menuItemPause = CCMenuItemLabel::itemWithLabel(labelPause, this, menu_selector(CreationLayer::onPauseButton));
    labelPause->setColor(ccc3(0, 254, 30));
    
    labelPlay = CCLabelTTF::labelWithString("Simulate", "Arial", 30);
	CCMenuItemLabel * menuItemPlay = CCMenuItemLabel::itemWithLabel(labelPlay, this, menu_selector(CreationLayer::onPlayButton));

    
    CCLabelTTF * labelDelete = CCLabelTTF::labelWithString("Delete", "Arial", 30);
	CCMenuItemLabel * menuItemDelete = CCMenuItemLabel::itemWithLabel(labelDelete, this, menu_selector(CreationLayer::onDeleteButton));

    CCLabelTTF * labelReset = CCLabelTTF::labelWithString("Reset", "Arial", 30);
	CCMenuItemLabel * menuItemReset = CCMenuItemLabel::itemWithLabel(labelReset, this, menu_selector(CreationLayer::onResetButton));
    
    CCLabelTTF * labelBox = CCLabelTTF::labelWithString("BOX", "Arial", 30);
	CCMenuItemLabel * menuItemBox = CCMenuItemLabel::itemWithLabel(labelBox, this, menu_selector(CreationLayer::onBoxButton));
    
	CCMenu* pMenu = CCMenu::menuWithItems(menuItemPause,menuItemPlay,menuItemDelete,menuItemReset, menuItemBox, NULL);
	pMenu->setPosition(CCPointZero);
    
    menuItemPlay->setPosition(CCPoint(80, 30));
	menuItemPause->setPosition(CCPoint(160, 30));

    menuItemDelete->setPosition(CCPoint(screenSize.width-200, 30));
    menuItemDelete->setColor(ccc3(255, 0, 0));

    menuItemReset->setPosition(CCPoint(screenSize.width-70, 30));
    
    menuItemBox->setPosition(CCPoint(screenSize.width/2, screenSize.height - 30));
    
	this->addChild(pMenu, 1);

    
	return true;
}

void CreationLayer::onPlayButton(cocos2d::CCObject *sender){
    GameLevelScene::sharedGameScene()->runWorld();
    labelPlay->setColor(ccc3(0, 254, 30));
    labelPause->setColor(ccc3(255, 255, 255));
}

void CreationLayer::onPauseButton(cocos2d::CCObject *sender){
    GameLevelScene::sharedGameScene()->pauseWorld();
    labelPause->setColor(ccc3(0, 254, 30));
    labelPlay->setColor(ccc3(255, 255, 255));    
}

void CreationLayer::onDeleteButton(cocos2d::CCObject *sender){
    GameLevelScene::sharedGameScene()->wipeWorld();  
    labelPause->setColor(ccc3(0, 254, 30));
    labelPlay->setColor(ccc3(255, 255, 255));       
}

void CreationLayer::onResetButton(cocos2d::CCObject *sender){
    GameLevelScene::sharedGameScene()->resetWorld();  
    labelPause->setColor(ccc3(0, 254, 30));
    labelPlay->setColor(ccc3(255, 255, 255));       
}

void CreationLayer::onBoxButton(cocos2d::CCObject *sender){
}