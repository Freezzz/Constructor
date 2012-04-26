
#include "UIButton.h"
#include <iostream>

bool UIButton::init( const char *str, CCObject *rec, SEL_MenuHandler selector )
{
	initWithTarget( rec, selector );

	// background image
	CCSprite * bg = CCSprite::spriteWithFile( "level_btn.png" );
	setContentSize( bg->getContentSize() );
	bg->setAnchorPoint( CCPoint(0,0) );
	addChild( bg );

	CCLabelTTF *label = CCLabelTTF::labelWithString( str, "Arial", 16 );
	label->setAnchorPoint( CCPoint(0.5,0) );
	label->setPosition( CCPoint(getContentSize().width/2, -20) );
	label->setColor(ccc3(255, 255, 255));
	addChild( label );

	return true;
}

void UIButton::setIsLocked(bool isLocked){
	CCSprite * sprite;
	if (isLocked) {
		sprite = CCSprite::spriteWithFile( "locked.png" );
		this->setTarget(this, NULL);
	}else {
		sprite = CCSprite::spriteWithFile( "complete.png" );
	}
	setContentSize( sprite->getContentSize() );
	sprite->setAnchorPoint( CCPoint(0,0) );
	addChild( sprite );
}

