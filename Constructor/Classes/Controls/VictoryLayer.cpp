
#include "VictoryLayer.h"

bool VictoryLayer::init( )
{
	// center it
	setAnchorPoint( CCPoint(0.5,0.5) );
	
	// background image
	CCSprite * bg = CCSprite::spriteWithFile( "victorywin.png" );
	setContentSize( bg->getContentSize() );
	addChild( bg );
	bg->setAnchorPoint( CCPoint(0.5,0.5) );
	
	return true;
}


