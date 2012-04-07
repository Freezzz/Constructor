//
//  HelloWorldScene.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/6/12.
//  Copyright The Epic Bean 2012. All rights reserved.
//
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;


enum 
{
	kTagTileMap = 1,
	kTagSpriteManager = 1,
	kTagAnimation1 = 1,
}; 

/** Convert the given position into the box2d world. */
static inline float ptm(float d)
{
    return d / PTM_RATIO;
}

/** Convert the given position into the cocos2d world. */
static inline float mtp(float d)
{
    return d * PTM_RATIO;
}


HelloWorld::HelloWorld()
{
	setIsTouchEnabled( true );
	setIsAccelerometerEnabled( true );
    
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	//UXLOG(L"Screen width %0.2f screen height %0.2f",screenSize.width,screenSize.height);
    
	// Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	
	// Do we want to let bodies sleep?
	bool doSleep = true;
    
	// Construct a world object, which will hold and simulate the rigid bodies.
	world = new b2World(gravity);
    world->SetAllowSleeping(doSleep);    
	world->SetContinuousPhysics(true);
    

    m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    world->SetDebugDraw(m_debugDraw);
    
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	m_debugDraw->SetFlags(flags);
	
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(screenSize.width/2/PTM_RATIO, 
                               screenSize.height/2/PTM_RATIO); // bottom-left corner
	
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world->CreateBody(&groundBodyDef);
	
	// Define the ground box shape.
	b2PolygonShape groundBox;		
	
    // bottom
    groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, -screenSize.height/2/PTM_RATIO), 0);
 	groundBody->CreateFixture(&groundBox, 0);
	
    // top
    groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, screenSize.height/2/PTM_RATIO), 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    // left
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(-screenSize.width/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    // right
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(screenSize.width/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);
    
	
	//Set up sprite
	CCSpriteBatchNode *mgr = CCSpriteBatchNode::batchNodeWithFile("blocks.png", 150);
	addChild(mgr, 0, kTagSpriteManager);
	
	addNewSpriteWithCoords( CCPointMake(screenSize.width/2, screenSize.height/2) );
	
	CCLabelTTF *label = CCLabelTTF::labelWithString("Tap screen", "Marker Felt", 32);
	addChild(label, 0);
	label->setColor( ccc3(0,0,255) );
	label->setPosition( CCPointMake( screenSize.width/2, screenSize.height-50) );
	
    this->addNewRagDollAtPosition(CCPoint(screenSize.width*0.8, screenSize.height*0.1));
	schedule( schedule_selector(HelloWorld::tick) );
}

HelloWorld::~HelloWorld()
{
	delete world;
	world = NULL;
	
	//delete m_debugDraw;
}

void HelloWorld::draw()
{
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states:  GL_VERTEX_ARRAY, 
	// Unneeded states: GL_TEXTURE_2D, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	world->DrawDebugData();
	
	// restore default GL states
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
}

void HelloWorld::addNewSpriteWithCoords(CCPoint p)
{
	//UXLOG(L"Add sprite %0.2f x %02.f",p.x,p.y);
	CCSpriteBatchNode* sheet = (CCSpriteBatchNode*)getChildByTag(kTagSpriteManager);
	
	//We have a 64x64 sprite sheet with 4 different 32x32 images.  The following code is
	//just randomly picking one of the images
	int idx = (CCRANDOM_0_1() > .5 ? 0:1);
	int idy = (CCRANDOM_0_1() > .5 ? 0:1);
    
	CCSprite *sprite = CCSprite::spriteWithBatchNode(sheet, CCRectMake(32 * idx,32 * idy,32,32));
	sheet->addChild(sprite);
	
	sprite->setPosition( CCPointMake( p.x, p.y) );
	
	// Define the dynamic body.
	//Set up a 1m squared box in the physics world
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
	bodyDef.userData = sprite;
	b2Body *body = world->CreateBody(&bodyDef);
	
	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(.5f, .5f);//These are mid points for our 1m box
	
	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;	
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);
}


void HelloWorld::tick(ccTime dt)
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/
	
	int velocityIterations = 8;
	int positionIterations = 1;
    
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(dt, velocityIterations, positionIterations);
	
	//Iterate over the bodies in the physics world
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			CCSprite* myActor = (CCSprite*)b->GetUserData();
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
		}	
	}
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	//Add a new body/atlas sprite at the touched location
	CCSetIterator it;
	CCTouch* touch;
    
	for( it = touches->begin(); it != touches->end(); it++) 
	{
		touch = (CCTouch*)(*it);
        
		if(!touch)
			break;
        
		CCPoint location = touch->locationInView(touch->view());
		
		location = CCDirector::sharedDirector()->convertToGL(location);
        
		addNewSpriteWithCoords( location );
	}
}

void HelloWorld::addNewRagDollAtPosition(cocos2d::CCPoint _ragDollPosition)
{
    // -------------------------
    // Bodies ------------------
    // -------------------------
    
    // Set these to dynamics bodies
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    b2PolygonShape box;
    b2FixtureDef fixtureDef;
    
    // Head ------
    b2CircleShape headShape;
    headShape.m_radius = ptm(12.5f);
    fixtureDef.shape = &headShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.3f;
    bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y));
    b2Body *head = world->CreateBody(&bd);
    head->CreateFixture(&fixtureDef);
//    head->ApplyLinearImpulse(b2Vec2(random() % 100 - 50.0f, random() % 100 - 50.0f), head->GetWorldCenter());
    
    // -----------
    
    // Torso1 ----
    box.SetAsBox(ptm(15.0f), ptm(10.0f));
    fixtureDef.shape = &box;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.1f;
    bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 25.0f));
    b2Body *torso1 = world->CreateBody(&bd);
    torso1->CreateFixture(&fixtureDef);
    
    // -----------
    
    // Torso2 ----
    box.SetAsBox(ptm(15.0f), ptm(10.0f));
    fixtureDef.shape = &box;
    bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 43.0f));
    b2Body *torso2 = world->CreateBody(&bd);
    torso2->CreateFixture(&fixtureDef);
    
    // -----------
    
    // Torso3 ----
    box.SetAsBox(ptm(15.0f), ptm(10.0f));
    fixtureDef.shape = &box;
    bd.position.Set(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 58.0f));
    b2Body *torso3 = world->CreateBody(&bd);
    torso3->CreateFixture(&fixtureDef);
    
    // -----------
    
    // UpperArm --
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.1f;
    
    // Left
    box.SetAsBox(ptm(18.0f), ptm(6.5f));
    fixtureDef.shape = &box;
    bd.position.Set(ptm(_ragDollPosition.x - 30.0f), ptm(_ragDollPosition.y + 20.0f));
    b2Body *upperArmL = world->CreateBody(&bd);
    upperArmL->CreateFixture(&fixtureDef);
    
    // Right
    box.SetAsBox(ptm(18.0f), ptm(6.5f));
    fixtureDef.shape = &box;
    bd.position.Set(ptm(_ragDollPosition.x + 30.0f), ptm(_ragDollPosition.y + 20.0f));
    b2Body *upperArmR = world->CreateBody(&bd);
    upperArmR->CreateFixture(&fixtureDef);
    
    // -----------
    
    // Lower Arm
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.1f;
    
    // Left
    box.SetAsBox(ptm(17.0f), ptm(6.0f));
    fixtureDef.shape = &box;
    bd.position.Set(ptm(_ragDollPosition.x - 57.0f), ptm(_ragDollPosition.y + 20.0f));
    b2Body *lowerArmL = world->CreateBody(&bd);
    lowerArmL->CreateFixture(&fixtureDef);
    
    // Right
    box.SetAsBox(ptm(17.0f), ptm(6.0f));
    fixtureDef.shape = &box;
    bd.position.Set(ptm(_ragDollPosition.x + 57.0f), ptm(_ragDollPosition.y + 20.0f));
    b2Body *lowerArmR = world->CreateBody(&bd);
    lowerArmR->CreateFixture(&fixtureDef);
    
    // -----------
    
    // UpperLeg --
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.1f;
    
    // Left
    box.SetAsBox(ptm(7.5f), ptm(22.0f));
    fixtureDef.shape = &box;
    bd.position.Set(ptm(_ragDollPosition.x - 8.0f), ptm(_ragDollPosition.y + 85.0f));
    b2Body *upperLegL = world->CreateBody(&bd);
    upperLegL->CreateFixture(&fixtureDef);
    
    // Right
    box.SetAsBox(ptm(7.5f), ptm(22.0f));
    fixtureDef.shape = &box;
    bd.position.Set(ptm(_ragDollPosition.x + 8.0f), ptm(_ragDollPosition.y + 85.0f));
    b2Body *upperLegR = world->CreateBody(&bd);
    upperLegR->CreateFixture(&fixtureDef);
    
    // -----------
    
    // LowerLeg --
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.1f;
    
    // Left
    box.SetAsBox(ptm(6.0f), ptm(20.0f));
    fixtureDef.shape = &box;
    bd.position.Set(ptm(_ragDollPosition.x - 8.0f), ptm(_ragDollPosition.y + 120.0f));
    b2Body *lowerLegL = world->CreateBody(&bd);
    lowerLegL->CreateFixture(&fixtureDef);
    
    // Right
    box.SetAsBox(ptm(6.0f), ptm(20.0f));
    fixtureDef.shape = &box;
    bd.position.Set(ptm(_ragDollPosition.x + 8.0f), ptm(_ragDollPosition.y + 120.0f));
    b2Body *lowerLegR = world->CreateBody(&bd);
    lowerLegR->CreateFixture(&fixtureDef);
    // -----------
    
    // -------------------------
    // Joints ------------------
    // -------------------------
    
    b2RevoluteJointDef jd;
    jd.enableLimit = true;
    
    // Head to shoulders
    jd.lowerAngle = -40.0f / (180.0f / M_PI);
    jd.upperAngle = 40.0f / (180.0f / M_PI);
    jd.Initialize(torso1, head, b2Vec2(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 15.0f)));
    world->CreateJoint(&jd);
    
    // Upper arm to shoulders --
    // Left
    jd.lowerAngle = -85.0f / (180.0f / M_PI);
    jd.upperAngle = 130.0f / (180.0f / M_PI);
    jd.Initialize(torso1, upperArmL, b2Vec2(ptm(_ragDollPosition.x - 18.0f), ptm(_ragDollPosition.y + 20.0f)));
    world->CreateJoint(&jd);
    
    // Right
    jd.lowerAngle = -130.0f / (180.0f / M_PI);
    jd.upperAngle = 85.0f / (180.0f / M_PI);
    jd.Initialize(torso1, upperArmR, b2Vec2(ptm(_ragDollPosition.x + 18.0f), ptm(_ragDollPosition.y + 20.0f)));
    world->CreateJoint(&jd);
    
    // -------------------------
    
    // Lower arm to shoulders --
    // Left
    jd.lowerAngle = -130.0f / (180.0f / M_PI);
    jd.upperAngle = 10.0f / (180.0f / M_PI);
    jd.Initialize(upperArmL, lowerArmL, b2Vec2(ptm(_ragDollPosition.x - 45.0f), ptm(_ragDollPosition.y + 20.0f)));
    world->CreateJoint(&jd);
    
    // Right
    jd.lowerAngle = -10.0f / (180.0f / M_PI);
    jd.upperAngle = 130.0f / (180.0f / M_PI);
    jd.Initialize(upperArmR, lowerArmR, b2Vec2(ptm(_ragDollPosition.x + 45.0f), ptm(_ragDollPosition.y + 20.0f)));
    world->CreateJoint(&jd);
    
    // -------------------------
    
    // Shoulders / stomach -----
    jd.lowerAngle = -15.0f / (180.0f / M_PI);
    jd.upperAngle = 15.0f / (180.0f / M_PI);
    jd.Initialize(torso1, torso2, b2Vec2(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 35.0f)));
    world->CreateJoint(&jd);
    
    // Stomach / hips
    jd.Initialize(torso2, torso3, b2Vec2(ptm(_ragDollPosition.x), ptm(_ragDollPosition.y + 50.0f)));
    world->CreateJoint(&jd);
    
    // -------------------------
    
    // Torso to upper leg ------
    // Left
    jd.lowerAngle = -25.0f / (180.0f / M_PI);
    jd.upperAngle = 45.0f / (180.0f / M_PI);
    jd.Initialize(torso3, upperLegL, b2Vec2(ptm(_ragDollPosition.x - 8), ptm(_ragDollPosition.y + 72.0f)));
    world->CreateJoint(&jd);
    
    // Right
    jd.lowerAngle = -45.0f / (180.0f / M_PI);
    jd.upperAngle = 25.0f / (180.0f / M_PI);
    jd.Initialize(torso3, upperLegR, b2Vec2(ptm(_ragDollPosition.x + 8), ptm(_ragDollPosition.y + 72.0f)));
    world->CreateJoint(&jd);
    
    // -------------------------
    
    // Upper leg to lower leg --
    // Left
    jd.lowerAngle = -25.0f / (180.0f / M_PI);
    jd.upperAngle = 115.0f / (180.0f / M_PI);
    jd.Initialize(upperLegL, lowerLegL, b2Vec2(ptm(_ragDollPosition.x - 8), ptm(_ragDollPosition.y + 105.0f)));
    world->CreateJoint(&jd);
    
    // Right
    jd.lowerAngle = -115.0f / (180.0f / M_PI);
    jd.upperAngle = 25.0f / (180.0f / M_PI);
    jd.Initialize(upperLegR, lowerLegR, b2Vec2(ptm(_ragDollPosition.x + 8), ptm(_ragDollPosition.y + 105.0f)));
    world->CreateJoint(&jd);
    
    // -------------------------
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::node();
    
    // add layer as a child to scene
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}
