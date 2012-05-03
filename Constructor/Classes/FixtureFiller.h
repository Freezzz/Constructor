//
//  FixtureFiller.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 5/3/12.
//  Copyright (c) 2012 The Epic Bean. All rights reserved.
//

#ifndef Constructor_FixtureFiller_h
#define Constructor_FixtureFiller_h

#include "cocos2d.h"
#include "Box2D.h"


using namespace cocos2d;

//////////////////////////////////////////////////// 
// Draws a filled polygon from a fixture 
//////////////////////////////////////////////////// 
class FixtureFiller{
private:
	bool m_isTextureFiller;
	
	int m_glVertexCount;
	ccVertex2F *m_glVertixes;
	
	
	ccColor4B m_fillColor;
	ccColor4B m_borderColor;
	
	int m_areaPointsCount;
	ccVertex2F * m_areaTrianglePointsVertex;

	CCTexture2D * m_fillTexture;
	ccVertex2F * m_textureCoordinates;	
	
	void drawColored();
	void drawTextured();
	
	void populateGlVertixesFromShape(b2Shape * shape);
public:
	FixtureFiller(b2Fixture * fixture, CCTexture2D * texture, ccColor4B borderColor);
	FixtureFiller(b2Fixture * fixture, ccColor4B fillColor,ccColor4B borderColor);
	
	~FixtureFiller();
	
	void draw();
};

#endif
