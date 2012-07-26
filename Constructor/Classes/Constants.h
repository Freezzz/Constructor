//
//  Constants.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/14/12.
//  Copyright (c) 2012 The Epic Bean. All rights reserved.
//

#ifndef Constructor_Constants_h
#define Constructor_Constants_h

#define PTM_RATIO 32.0f

typedef enum ObjectType{
    Undefined   = -1,
    SimpleBox   = 0,
	Area		= 1,
    Spring      = 3,
	Pin			= 4,
	Glue		= 5,
	Fatty		= 6,
	Water		= 7,
	Oven		= 8,
	Crumb		= 9
}ObjectType;


#endif
