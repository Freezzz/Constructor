
#ifndef __VictoryLayer_H__
#define __VictoryLayer_H__

#include "cocos2d.h"
#include "Constants.h"
using namespace cocos2d;

class VictoryLayer : public CCLayerColor
{
private:
	CCMenu *m_menu;
	CCSprite * m_bg;
	CCMenuItemImage * m_nextItem;
private:
	bool init();

public:
	void next( CCObject * sender );
	void menu( CCObject * sender );
	void restart( CCObject * sender );
	void setOnScreen( bool isOnscreen );

public:
	LAYER_NODE_FUNC( VictoryLayer );
	
};


#endif


