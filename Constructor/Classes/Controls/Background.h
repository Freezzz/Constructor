
#ifndef __Background_H__
#define __Background_H__

#include "cocos2d.h"
using namespace cocos2d;

namespace Json {
	class Value;
}

class ParticleEffect : public ARCH_OPTIMAL_PARTICLE_SYSTEM
{
public:
	ParticleEffect(){}
	virtual ~ParticleEffect(){}
	bool init( const Json::Value &def );
	static ParticleEffect * node( const Json::Value &def )
	{
		ParticleEffect *p = new ParticleEffect();
		if( p->init(def) ) {
			p->autorelease();
			return p;
		}
		CC_SAFE_DELETE( p );
		return NULL;
	}
};

class Background : public CCLayer
{
public:
	CCSprite *m_backgroundImage;

public:
	bool init( const Json::Value &def );
	static Background * node( const Json::Value &def )
	{
		Background *p = new Background();
		if( p->init(def) ) {
			p->autorelease();
			return p;
		}
		CC_SAFE_DELETE( p );
		return NULL;
	}

};

#endif
