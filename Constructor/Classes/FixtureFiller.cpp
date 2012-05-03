//
//  FixtureFiller.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 5/3/12.
//  Copyright (c) 2012 The Epic Bean. All rights reserved.
//

#include "FixtureFiller.h"
#include "triangulate.h"
#include "Constants.h"


//////////////////////////////////////////////////// 
// Constructor for a textured filler, n.b. texture should 
// be added to the scene before draw is called 
//////////////////////////////////////////////////// 
FixtureFiller::FixtureFiller(b2Fixture * fixture, CCTexture2D * texture, ccColor4B borderColor){
	m_isTextureFiller = true;
	
	m_borderColor = borderColor;
	m_fillTexture = texture;
	
	populateGlVertixesFromShape(fixture->GetShape());
	
	Vector2dVector result;
	Vector2dVector areas;
	
	// Convert ccVertex2F to Vector2d 
	for (int i = 0; i< m_glVertexCount; i++) {
		areas.push_back(Vector2d(m_glVertixes[i].x, m_glVertixes[i].y));
	}

	Triangulate::Process(areas,result);
	m_areaPointsCount = result.size();

	// Convert  Vector2d to ccVertex2F
	m_areaTrianglePointsVertex = (ccVertex2F *) malloc(sizeof(ccVertex2F) * m_areaPointsCount);	
	for (int j = 0; j < m_areaPointsCount; j++) {
		m_areaTrianglePointsVertex[j] = (ccVertex2F){result[j].GetX(), result[j].GetY()};
	}

	// Calculate texture coordinates
	m_textureCoordinates = (ccVertex2F*)malloc(sizeof(ccVertex2F) * m_areaPointsCount);
	for (int j = 0; j < m_areaPointsCount; j++) {
		m_textureCoordinates[j].x = m_areaTrianglePointsVertex[j].x * 1.0f/m_fillTexture->getPixelsWide();
		m_textureCoordinates[j].y = m_areaTrianglePointsVertex[j].y * 1.0f/m_fillTexture->getPixelsWide();		
	}

}

//////////////////////////////////////////////////// 
// COnstructor for colored filler
//////////////////////////////////////////////////// 
FixtureFiller::FixtureFiller(b2Fixture * fixture, ccColor4B fillColor,ccColor4B borderColor){
	m_isTextureFiller = false;
	
	m_fillColor = fillColor;
	m_borderColor = borderColor;

	populateGlVertixesFromShape(fixture->GetShape());	
}


void FixtureFiller::drawColored(){
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
	glVertexPointer(2, GL_FLOAT, 0, m_glVertixes);
	
	glColor4f(m_fillColor.r, m_fillColor.g, m_fillColor.b, m_fillColor.a);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_glVertexCount);
	
	glColor4f(m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
	glDrawArrays(GL_LINE_LOOP, 0, m_glVertexCount);
	
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}

void FixtureFiller::drawTextured(){
	glDisableClientState(GL_COLOR_ARRAY);
	
	glBindTexture(GL_TEXTURE_2D, m_fillTexture->getName());
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
	glVertexPointer(2, GL_FLOAT, 0, m_areaTrianglePointsVertex);
	glTexCoordPointer(2, GL_FLOAT, 0, m_textureCoordinates);	
	glDrawArrays(GL_TRIANGLES, 0, m_areaPointsCount);
	
	
	glVertexPointer(2, GL_FLOAT, 0, m_glVertixes);	
	glColor4f(m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
	glDrawArrays(GL_LINE_LOOP, 0, m_glVertexCount);
	
	glEnableClientState(GL_COLOR_ARRAY);
}

//////////////////////////////////////////////////// 
// Convets fixture shape to opengl vertixes array
//////////////////////////////////////////////////// 
void FixtureFiller::populateGlVertixesFromShape(b2Shape *shape){
	if (shape->m_type == b2Shape::e_polygon) {
		b2PolygonShape* polyShape = (b2PolygonShape*)shape;
		m_glVertexCount = polyShape->GetVertexCount();
		
		m_glVertixes = (ccVertex2F*)malloc(m_glVertexCount * sizeof(ccVertex2F));

		for (int i = 0; i< m_glVertexCount; i++) {
			m_glVertixes[i] = (ccVertex2F){
				polyShape->GetVertex(i).x * PTM_RATIO * CC_CONTENT_SCALE_FACTOR(),
				polyShape->GetVertex(i).y * PTM_RATIO * CC_CONTENT_SCALE_FACTOR()
			};
		}
	}
}

void FixtureFiller::draw(){
	if (m_isTextureFiller) {
		drawTextured();
	}else {
		drawColored();
	}
}