
#ifndef Constructor_Common_h
#define Constructor_Common_h

#include "cocos2d.h"
#include "Box2D.h"

inline b2Vec2 v( cocos2d::CCPoint p )
{
	return b2Vec2( p.x, p.y );
}
inline cocos2d::CCPoint v( b2Vec2 v )
{
	return cocos2d::CCPoint( v.x, v.y );
}

#endif
