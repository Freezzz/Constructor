//
//  RaysCastCallback.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 7/25/12.
//  Copyright (c) 2012 The Epic Bean. All rights reserved.
//

#ifndef Constructor_RaysCastCallback_h
#define Constructor_RaysCastCallback_h
#include <Box2D/Box2D.h>

class RaysCastCallback : public b2RayCastCallback{
public:
    RaysCastCallback() : m_fixture(NULL) {
    }
	
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {        
        m_fixture = fixture;        
        m_point = point;        
        m_normal = normal;        
        m_fraction = fraction;        
        return fraction;     
    }    
	
    b2Fixture* m_fixture;    
    b2Vec2 m_point;    
    b2Vec2 m_normal;    
    float32 m_fraction;
	
};


#endif
