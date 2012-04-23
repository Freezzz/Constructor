
#ifndef __UIButton_H__
#define __UIButton_H__

#include "cocos2d.h"
#include "Constants.h"
using namespace cocos2d;

class UIButton : public CCMenuItemLabel
{
private:
	bool init( const char *str, CCObject *rec, SEL_MenuHandler selector );

public:
	static UIButton *node( const char *str, CCObject *rec, SEL_MenuHandler selector ) {
		UIButton *r = new UIButton( );
		if( r && r->init(str, rec, selector) ) {
			r->autorelease();
			return r;
		}
		delete r;
		return NULL;
	}

};

#endif
