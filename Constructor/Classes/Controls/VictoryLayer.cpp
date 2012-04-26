
#include "VictoryLayer.h"
#include "GameLevelScene.h"
#include "MainMenuScene.h"
#include <iostream>

bool VictoryLayer::init( )
{
	// center it
	setAnchorPoint( CCPoint(0,0) );
	CCSize  winSize = CCDirector::sharedDirector()->getWinSize();
	
	// background image
	CCSprite * bg = CCSprite::spriteWithFile( "victorywin.png" );
	setContentSize( bg->getContentSize() );
	addChild( bg );

	CCMenuItemImage * homeItem = CCMenuItemImage::itemFromNormalImage("menu_btn.png", "menu_btn.png", this, menu_selector(VictoryLayer::menu));
	homeItem->setPosition(CCPoint(winSize.width*0.5- 150, winSize.height*0.5));
	
	CCMenuItemImage * restartItem = CCMenuItemImage::itemFromNormalImage("restart_btn.png", "restart_btn.png", this, menu_selector(VictoryLayer::restart));
	restartItem->setPosition(CCPoint(winSize.width*0.5, winSize.height*0.5));
	
	CCMenuItemImage * nextItem = CCMenuItemImage::itemFromNormalImage("next_btn.png", "next_btn.png", this, menu_selector(VictoryLayer::next));
	nextItem->setPosition(CCPoint(winSize.width*0.5 + 150, winSize.height*0.5));
	
	m_menu = CCMenu::menuWithItems(homeItem, restartItem, nextItem, NULL);
	addChild( m_menu );
	m_menu->setPosition(CCPointZero);
    m_menu->setIsRelativeAnchorPoint( true );

	setIsTouchEnabled( true );

	return true;
}

void VictoryLayer::setOnScreen( bool isOnscreen )
{
	if ( ! isOnscreen) {
		m_menu->setIsTouchEnabled( 0 );
		runAction( CCScaleTo::actionWithDuration(0.5, 0) );
	}
	else {
		m_menu->setIsTouchEnabled( 1 );
		runAction( CCScaleTo::actionWithDuration(0.5, 1) );
	}
}

void VictoryLayer::next( CCObject * sender )
{
	std::cout << "TODO: go to next level" << std::endl;
}
void VictoryLayer::menu( CCObject * sender )
{
	CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
}
void VictoryLayer::restart( CCObject * sender )
{
	GameLevelScene::sharedGameScene()->reloadLevel();
}

