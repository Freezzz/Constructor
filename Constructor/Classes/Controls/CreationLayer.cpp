//
//  CreationLayer.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "CreationLayer.h"
#include "UserLevelsLayer.h"
#include "UserLevelSaveLayer.h"
#include <GameLevelScene.h>

//////////////////////////////////////////////////// 
// CreationLayer init
//////////////////////////////////////////////////// 
bool CreationLayer::init(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
	// Set achor point to top
	setAnchorPoint( CCPoint(0.5,0.5) );
    
    // Buttons
    m_labelPause = CCLabelTTF::labelWithString("Edit", "Arial", 30);
	CCMenuItemLabel * menuItemPause = CCMenuItemLabel::itemWithLabel(m_labelPause, this, menu_selector(CreationLayer::onPauseButton));
    m_labelPause->setColor(ccc3(0, 254, 30));
    
    m_labelPlay = CCLabelTTF::labelWithString("Simulate", "Arial", 30);
	CCMenuItemLabel * menuItemPlay = CCMenuItemLabel::itemWithLabel(m_labelPlay, this, menu_selector(CreationLayer::onPlayButton));

    CCLabelTTF * labelSave = CCLabelTTF::labelWithString("Save", "Arial", 30);
	CCMenuItemLabel * menuItemSave = CCMenuItemLabel::itemWithLabel(labelSave, this, menu_selector(CreationLayer::onSaveButton));
    CCLabelTTF * labelLoad = CCLabelTTF::labelWithString("Load", "Arial", 30);
	CCMenuItemLabel * menuItemLoad = CCMenuItemLabel::itemWithLabel(labelLoad, this, menu_selector(CreationLayer::onLoadButton));
    
    CCLabelTTF * labelDelete = CCLabelTTF::labelWithString("Delete", "Arial", 30);
	CCMenuItemLabel * menuItemDelete = CCMenuItemLabel::itemWithLabel(labelDelete, this, menu_selector(CreationLayer::onDeleteButton));

    CCLabelTTF * labelReset = CCLabelTTF::labelWithString("Reset", "Arial", 30);
	CCMenuItemLabel * menuItemReset = CCMenuItemLabel::itemWithLabel(labelReset, this, menu_selector(CreationLayer::onResetButton));
        
	m_menu = CCMenu::menuWithItems(menuItemPause,menuItemPlay,menuItemSave,menuItemLoad,menuItemDelete,menuItemReset, NULL);
	m_menu->setPosition( CCPoint(0,screenSize.height-30) );
	m_menu->setContentSize( CCSize(screenSize.width, 60) );

    menuItemPlay->setPosition( CCPoint(80, 0) );
	menuItemPause->setPosition( CCPoint(180, 0) );

    menuItemSave->setPosition( CCPoint(screenSize.width/2-50, 0) );
    menuItemLoad->setPosition( CCPoint(screenSize.width/2+50, 0) );

    menuItemDelete->setPosition( CCPoint(screenSize.width-200, 0) );
    menuItemDelete->setColor(ccc3(255, 0, 0) );

    menuItemReset->setPosition( CCPoint(screenSize.width-70, 0) );

	addChild( m_menu, 1 );
	setContentSize( m_menu->getContentSize() );
    
    // init user level save dialog
	m_userLevelSaveLayer = UserLevelSaveLayer::node();
	addChild(m_userLevelSaveLayer, 2);
	m_userLevelSaveLayer->setPosition(CCPointZero);
	m_userLevelSaveLayer->setIsVisible(false);
	
	// init user level list
	m_userLevelsLayer = UserLevelsLayer::node();
	addChild(m_userLevelsLayer, 2);
	m_userLevelsLayer->setPosition(CCPointZero);
	m_userLevelsLayer->setIsVisible(false);
	return true;
}

//////////////////////////////////////////////////// 
// Shows and hides the buttons 
//////////////////////////////////////////////////// 
void CreationLayer::setOnScreen( bool isOnscreen ){
	CCPoint location = this->getPosition();
	location.y = 0; // assuming it will be always on the top of the screen
	if ( ! isOnscreen) {
		location.y += this->getContentSize().height;
		m_menu->setIsTouchEnabled( 0 );
	}
	else {
		m_menu->setIsTouchEnabled( 1 );
	}
	this->runAction( CCMoveTo::actionWithDuration(0.5, location) );
}

//////////////////////////////////////////////////// 
// Disables/Enables menu interaction
//////////////////////////////////////////////////// 
void CreationLayer::setMenuEnable(bool isEnabled){
	m_menu->setIsTouchEnabled(isEnabled);
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

void CreationLayer::onSaveButton(CCObject * sender)
{
	setMenuEnable(false);
	m_userLevelSaveLayer->setIsVisible(true);
}

void CreationLayer::onLoadButton(CCObject * sender)
{
	setMenuEnable(false);
	m_userLevelsLayer->refreshList();
	m_userLevelsLayer->setIsVisible(true);
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
