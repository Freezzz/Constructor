//
//  UserLevelSaveLayer.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/24/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "UserLevelSaveLayer.h"
#include "GameLevelScene.h"
#include "CreationLayer.h"
#include "LevelManager.h"
#include "LevelDef.h"

//////////////////////////////////////////////////// 
// UserLevelSaveLayer init
//////////////////////////////////////////////////// 
bool UserLevelSaveLayer::init(){
	CCSize s = CCDirector::sharedDirector()->getWinSize();

	CCSprite* bgImage = CCSprite::spriteWithFile("user_level_save.png"); 
	addChild(bgImage);
	bgImage->setPosition(CCPoint(s.width*0.5, s.height-73));
	setContentSize(bgImage->getContentSize());
	
	// Name selection textfield
	m_textField = CCTextFieldTTF::textFieldWithPlaceHolder("Nome", "Arial", 30);
	addChild(m_textField,2);
	m_textField->setDelegate(this);
	m_textField->setPosition(CCPoint(s.width * 0.5 + 150, s.height - 30));
    m_textField->setColor(ccc3(0, 0, 0));
	m_textField->setContentSize(CCSize(300, 40));
	
	// Menu items
	CCLabelTTF * closeLabel = CCLabelTTF::labelWithString("[X]Close", "Arial", 28);
	CCMenuItemLabel * closeButton = CCMenuItemLabel::itemWithLabel(closeLabel, this, menu_selector(UserLevelSaveLayer::menuCloseCallback));
	closeButton->setPosition(CCPoint(s.width * 0.5 + 120, s.height - 95));
	closeLabel->setColor(ccc3(0, 55, 73));
	
	CCLabelTTF * saveLabel = CCLabelTTF::labelWithString("[V]Save", "Arial", 28);
	CCMenuItemLabel * saveButton = CCMenuItemLabel::itemWithLabel(saveLabel, this, menu_selector(UserLevelSaveLayer::menuSaveCallback));
	saveButton->setPosition(CCPoint(s.width * 0.5 - 120, s.height - 95));
	saveLabel->setColor(ccc3(0, 55, 73));
	
	CCMenu * closeMenu = CCMenu::menuWithItems(closeButton,saveButton, NULL);
	closeMenu->setPosition(CCPointZero);	
	addChild(closeMenu,5);

	setIsTouchEnabled(true);
	return true;
}

void UserLevelSaveLayer::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

bool UserLevelSaveLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	return getIsVisible();
}

void UserLevelSaveLayer::ccTouchEnded(CCTouch *pTouch, CCEvent* pEvent){
	CCPoint location = pTouch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    // if touch is on textbox
    if (CCRect::CCRectContainsPoint(m_textField->boundingBox(), location)) {
		m_textField->attachWithIME();
	}
}

////////////////////////////////////////////////////
// TextInput delegate handler, called when text editing is done
///////////////////////////////////////////////////
bool UserLevelSaveLayer::onTextFieldDetachWithIME(CCTextFieldTTF * pSender) {
    // Resize user input to 10 chars
	CCLog("NEW NAME: %s",pSender->getString());
    return false;
}

void UserLevelSaveLayer::menuSaveCallback(cocos2d::CCObject *sender){
	CCLog("SAVNG TO LEVEL NAME: %s", 	m_textField->getString());

	// TODO: empty check, filename sanitize
	LevelDef * lvl = GameLevelScene::sharedGameScene()->getCurrentLevelDef();
	lvl->name = m_textField->getString();
	LevelManager::sharedManager()->saveUserLevel(lvl, m_textField->getString());
	
	m_textField->detachWithIME();	
	setIsVisible(false);
	((CreationLayer*)getParent())->setMenuEnable(true);	
}

void UserLevelSaveLayer::menuCloseCallback(cocos2d::CCObject *sender){
	m_textField->detachWithIME();
	setIsVisible(false);
	((CreationLayer*)getParent())->setMenuEnable(true);
}

////////////////////////////////////////////////////
// TextInput delegate handler, using for char limit
// @return true if text should be swalowed
///////////////////////////////////////////////////
bool UserLevelSaveLayer::onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen) {
    // Return carret should be passed always
    if (*text == '\n') {
        return false;
    }
	// Char limit
	if (pSender->getCharCount() > 10)
		return true;
	return false;
    
}