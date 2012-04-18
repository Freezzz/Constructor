//
//  CreationLayer.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "CreationLayer.h"
#include <GameLevelScene.h>

//////////////////////////////////////////////////// 
// CreationLayer init
//////////////////////////////////////////////////// 
bool CreationLayer::init(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // Buttons
    m_labelPause = CCLabelTTF::labelWithString("Edit", "Arial", 30);
	CCMenuItemLabel * menuItemPause = CCMenuItemLabel::itemWithLabel(m_labelPause, this, menu_selector(CreationLayer::onPauseButton));
    m_labelPause->setColor(ccc3(0, 254, 30));
    
    m_labelPlay = CCLabelTTF::labelWithString("Simulate", "Arial", 30);
	CCMenuItemLabel * menuItemPlay = CCMenuItemLabel::itemWithLabel(m_labelPlay, this, menu_selector(CreationLayer::onPlayButton));

    
    CCLabelTTF * labelDelete = CCLabelTTF::labelWithString("Delete", "Arial", 30);
	CCMenuItemLabel * menuItemDelete = CCMenuItemLabel::itemWithLabel(labelDelete, this, menu_selector(CreationLayer::onDeleteButton));

    CCLabelTTF * labelReset = CCLabelTTF::labelWithString("Reset", "Arial", 30);
	CCMenuItemLabel * menuItemReset = CCMenuItemLabel::itemWithLabel(labelReset, this, menu_selector(CreationLayer::onResetButton));
        
	CCMenu* pMenu = CCMenu::menuWithItems(menuItemPause,menuItemPlay,menuItemDelete,menuItemReset, NULL);
	pMenu->setPosition(CCPointZero);
    
    menuItemPlay->setPosition(CCPoint(80, screenSize.height-30));
	menuItemPause->setPosition(CCPoint(180, screenSize.height-30));

    menuItemDelete->setPosition(CCPoint(screenSize.width-200, screenSize.height-30));
    menuItemDelete->setColor(ccc3(255, 0, 0));

    menuItemReset->setPosition(CCPoint(screenSize.width-70, screenSize.height-30));
    
	addChild(pMenu, 1);

    
	return true;
}

void CreationLayer::onPlayButton(CCObject *sender){
    GameLevelScene::sharedGameScene()->runWorld();
    m_labelPlay->setColor(ccc3(0, 254, 30));
    m_labelPause->setColor(ccc3(255, 255, 255));
}

void CreationLayer::onPauseButton(CCObject *sender){
    GameLevelScene::sharedGameScene()->pauseWorld();
    m_labelPause->setColor(ccc3(0, 254, 30));
    m_labelPlay->setColor(ccc3(255, 255, 255));    
}

void CreationLayer::onDeleteButton(CCObject *sender){
    GameLevelScene::sharedGameScene()->wipeWorld();  
    m_labelPause->setColor(ccc3(0, 254, 30));
    m_labelPlay->setColor(ccc3(255, 255, 255));       
}

void CreationLayer::onResetButton(CCObject *sender){
    GameLevelScene::sharedGameScene()->resetWorld();  
    m_labelPause->setColor(ccc3(0, 254, 30));
    m_labelPlay->setColor(ccc3(255, 255, 255));       
}