
#include "UIButton.h"
#include <iostream>

bool UIButton::init( const char *str, CCObject *rec, SEL_MenuHandler selector )
{
	initWithTarget( rec, selector );

	// background image
	CCSprite * bg = CCSprite::spriteWithFile( "uibutton.png" );
	setContentSize( bg->getContentSize() );
	bg->setAnchorPoint( CCPoint(0,0) );
	addChild( bg );

	// adding label
	CCLabelTTF *label = CCLabelTTF::labelWithString( str, "Arial", 16 );
	label->setAnchorPoint( CCPoint(0,0) );
	label->setPosition( CCPoint(getContentSize().width/2-label->getContentSize().width/2,
								getContentSize().height/2-label->getContentSize().height/2) );
	addChild( label );

	return true;
}


