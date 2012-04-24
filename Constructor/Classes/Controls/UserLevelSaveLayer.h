//
//  UserLevelSaveLayer.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/24/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __UserLevelSaveLayer_H__
#define __UserLevelSaveLayer_H__

#include "cocos2d.h"


using namespace cocos2d;


////////////////////////////////////////////////////
// UserLevelSaveLayer, pop up dialog for level saving
///////////////////////////////////////////////////
class UserLevelSaveLayer : public CCLayer , public CCTextFieldDelegate {
private:
	CCTextFieldTTF * m_textField;
	
	virtual bool init();
	
	void menuCloseCallback(CCObject* sender);
    void menuSaveCallback(CCObject* sender);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);	
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent* pEvent);
	virtual void registerWithTouchDispatcher();
	
    // Text input delegates
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
	
public:
	LAYER_NODE_FUNC(UserLevelSaveLayer);
};

#endif
