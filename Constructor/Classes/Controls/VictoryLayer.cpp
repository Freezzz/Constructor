
#include "VictoryLayer.h"
#include "UIButton.h"
#include "GameLevelScene.h"
#include <iostream>

bool VictoryLayer::init( )
{
	// center it
	setAnchorPoint( CCPoint(0,0) );
	
	// background image
	CCSprite * bg = CCSprite::spriteWithFile( "victorywin.png" );
	setContentSize( bg->getContentSize() );
	addChild( bg );

	m_nextButton = UIButton::node( "Next", this, menu_selector(VictoryLayer::next) );
	m_nextButton->setPosition( CCPoint( -getContentSize().width/4, -getContentSize().height/4 ) );

	m_restartButton = UIButton::node( "Restart", this, menu_selector(VictoryLayer::restart) );
	m_restartButton->setPosition( CCPoint( 0, -getContentSize().height/4 ) );

	m_menuButton = UIButton::node( "Menu", this, menu_selector(VictoryLayer::menu) );
	m_menuButton->setPosition( CCPoint( getContentSize().width/4, -getContentSize().height/4 ) );

	m_menu = CCMenu::menuWithItems(m_nextButton, m_restartButton, m_menuButton, NULL);
	m_menu->setIsRelativeAnchorPoint( true );
	addChild( m_menu );

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
	std::cout << "TODO: go back to menu" << std::endl;
}
void VictoryLayer::restart( CCObject * sender )
{
	GameLevelScene::sharedGameScene()->wipeWorld();
}

