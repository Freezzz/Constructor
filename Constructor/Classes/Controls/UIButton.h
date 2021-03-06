
#ifndef __UIButton_H__
#define __UIButton_H__

#include "cocos2d.h"
#include "Constants.h"
using namespace cocos2d;

// ALL UR BASE BELONG TO US! lol using it as level button, refacoring inc XD
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
	void setIsLocked(bool isLocked);
};

#endif
