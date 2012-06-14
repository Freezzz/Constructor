
#include "Background.h"
#include "Serialization/json/json.h"

bool ParticleEffect::init( const Json::Value &def )
{
	if( ARCH_OPTIMAL_PARTICLE_SYSTEM::initWithTotalParticles( def["number"].asInt() ) )
	{
		// emitter duration
		m_fDuration = kCCParticleDurationInfinity;

		// set gravity mode.
		m_nEmitterMode = kCCParticleModeGravity;

		// Gravity Mode: gravity
		modeA.gravity = ccp(0,0);

		// Gravity Mode: speed of particles
		modeA.speed = def["speed"].asFloat();
		modeA.speedVar = def["speed var"].asFloat();

		// particle texture
		setTexture( CCTextureCache::sharedTextureCache()->addImage( def["texture"].asCString() ) );

		// emitter position
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		setPosition( ccp( winSize.width*def["position"]["x"].asFloat(), winSize.height*def["position"]["y"].asFloat() ) );
		m_tPosVar = ccp( winSize.width*def["position var"]["x"].asFloat(), winSize.height*def["position var"]["y"].asFloat() );

		// angle
		m_fAngle = def["angle"].asFloat();
		m_fAngleVar = def["angle var"].asFloat();

		// life of particles
		m_fLife = def["life"].asFloat();
		m_fLifeVar = def["life var"].asFloat();

		// size, in pixels
		m_fStartSize = def["size"].asFloat();
		m_fStartSizeVar = def["size var"].asFloat();
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per second
		m_fEmissionRate = def["emission rate"].asFloat();

		// color of particles
		m_tStartColor.r = 1.0f;
		m_tStartColor.g = 1.0f;
		m_tStartColor.b = 1.0f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.0f;
		m_tStartColorVar.g = 0.0f;
		m_tStartColorVar.b = 0.0f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 1.0f;
		m_tEndColor.g = 1.0f;
		m_tEndColor.b = 1.0f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		// additive
		this->setIsBlendAdditive(false);
		return true;
	}
	return false;
}

bool Background::init( const Json::Value &def )
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	setAnchorPoint( CCPoint(0.5,0.5) );
	setContentSize( winSize );

	// bg
	CCSprite * bg = CCSprite::spriteWithFile( def["background image"].asCString() );
	bg->setPosition( CCPoint(winSize.width*0.5, winSize.height*0.5) );
	bg->setScaleX( winSize.width / bg->getContentSize().width );
	bg->setScaleY( winSize.height / bg->getContentSize().height );
	addChild( bg );

	// particle systems
	int items = def["particle effects"].size();
	for( int i = 0; i < items; ++i ) {
		addChild( ParticleEffect::node( def["particle effects"][i] ) );
	}

	return true;
}
