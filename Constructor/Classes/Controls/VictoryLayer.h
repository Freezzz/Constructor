
#ifndef __VictoryLayer_H__
#define __VictoryLayer_H__

#include "cocos2d.h"
#include "Constants.h"
using namespace cocos2d;

class UIButton;


class VictoryLayer : public CCLayerColor
{
private:
	UIButton *m_restartButton, *m_nextButton, *m_menuButton;
	CCMenu *m_menu;

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


