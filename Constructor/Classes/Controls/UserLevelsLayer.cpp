//
//  UserLevelsLayer.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/24/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "UserLevelsLayer.h"
#include "CreationLayer.h"
#include "GameLevelScene.h"

#define ROW_HEIGHT 40
#define HEADER_OFFSET 50

//////////////////////////////////////////////////// 
// UserLevelsLayer init
//////////////////////////////////////////////////// 
bool UserLevelsLayer::init(){
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	
	CCSprite * bg = CCSprite::spriteWithFile("user_levels_bg.png");
	addChild(bg);
	bg->setPosition(CCPoint(s.width * 0.5, s.height * 0.5 -25));
	
	setContentSize(bg->getContentSize());
	
    // Add header
	CCSprite * hdr = CCSprite::spriteWithFile("user_levels_header.png");
	addChild(hdr,3);
	hdr->setPosition(CCPoint(s.width / 2, s.height - 25));

    // Add close button
	CCMenuItemImage * closeButton = CCMenuItemImage::itemFromNormalImage("close_btn.png", "close_btn.png", this, menu_selector(UserLevelsLayer::closeButtonCallback));
	closeButton->setPosition(CCPoint(s.width * 0.5 + 200, s.height - 25));
	CCMenu * closeMenu = CCMenu::menuWithItem(closeButton);
	closeMenu->setPosition(CCPointZero);	
	addChild(closeMenu,4);

	m_menu = NULL;
	populateMenu();

    setIsTouchEnabled(true);
	
	return true;
}

//////////////////////////////////////////////////// 
// Populates level list
//////////////////////////////////////////////////// 
void UserLevelsLayer::populateMenu(){
	if (m_menu) {
		m_menu->removeFromParentAndCleanup(true);
	}
	
    CCSize s = CCDirector::sharedDirector()->getWinSize();	
	m_levels = LevelManager::sharedManager()->getUserLevelList();
	// Populate level list
    m_menu = CCMenu::menuWithItems(NULL);
    for (unsigned int i = 0; i < m_levels->size(); ++i)
    {
        CCLabelTTF* label = CCLabelTTF::labelWithString(m_levels->at(i)->name.c_str(), "Arial", 24);
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::itemWithLabel(label, this, menu_selector(UserLevelsLayer::levelTapCallback));
		label->setColor(ccc3(0, 55, 73));
        m_menu->addChild(pMenuItem, i + 100);
        pMenuItem->setPosition( CCPointMake( s.width / 2, (s.height - (i + 1) * ROW_HEIGHT) - HEADER_OFFSET ));
		pMenuItem->setTag(i);
    }
	
    m_menu->setContentSize(CCSizeMake(s.width, (m_levels->size() + 1) * (ROW_HEIGHT)));
	m_menu->setPosition(CCPointZero);
	
    addChild(m_menu,2);

}


//////////////////////////////////////////////////// 
// Force refresh on level list
//////////////////////////////////////////////////// 
void UserLevelsLayer::refreshList(){
	populateMenu();
}

void UserLevelsLayer::levelTapCallback( cocos2d::CCObject *sender )
{
	GameLevelScene::sharedGameScene()->loadFile( m_levels->at(((CCMenuItemLabel*)sender)->getTag()) );
	setIsVisible(false);	
	((CreationLayer*)getParent())->setMenuEnable(true);
}

void UserLevelsLayer::closeButtonCallback( cocos2d::CCObject *sender )
{
	setIsVisible(false);
	((CreationLayer*)getParent())->setMenuEnable(true);	
}

////////////////////////////////////////////////////
// Scrolling logic
///////////////////////////////////////////////////
void UserLevelsLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
    
	m_beginPos = touch->locationInView();
	m_beginPos = CCDirector::sharedDirector()->convertToGL( m_beginPos );
}

////////////////////////////////////////////////////
// Scrolling logic
///////////////////////////////////////////////////
void UserLevelsLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
	
    CCPoint touchLocation = touch->locationInView();	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    float nMoveY = touchLocation.y - m_beginPos.y;
	
    CCPoint curPos  = m_menu->getPosition();
    CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (nextPos.y < 0.0f)
    {
        m_menu->setPosition(CCPointZero);
        return;
    }
	
    if (nextPos.y  > ((m_levels->size() + 1)* ROW_HEIGHT - winSize.height))
    {
        m_menu->setPosition(ccp(0, ((m_levels->size() + 1)* ROW_HEIGHT - winSize.height + HEADER_OFFSET)));
        return;
    }
	
    m_menu->setPosition(nextPos);
    m_beginPos = touchLocation;
}
