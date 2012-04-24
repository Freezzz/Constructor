//
//  LevelManager.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/24/12.
//  Copyright (c) 2012 The Epic Bean. All rights reserved.
//

#ifndef Constructor_LevelManager_h
#define Constructor_LevelManager_h
#include "cocos2d.h"
#include "LevelDef.h"

using namespace cocos2d;

typedef struct StoryLevelDescribtion{
	bool isConmplete;
	string name;
}StoryLevelDescribtion;

class LevelManager {
private:
	vector<string*> * m_userLevels;
	vector<StoryLevelDescribtion*> * m_storyLevels;
	
	int m_userLevelCount;
	int m_storyLevelCount;
	
	void loadUserLevelList();
	void loadStoryLevelList();
	
public:
	LevelManager( );
	~LevelManager( );

	LevelDef *loadUserLevel( const char *fileName );
	bool saveUserLevel( LevelDef * level, const char *fileName );
	
	vector<string*> * getUserLevelList(){return m_userLevels;}
	vector<StoryLevelDescribtion*> * getStoryLevelList(){return m_storyLevels;}
	
	//////////////////////////////////////////////////// 
    // Singleton pattern
    //////////////////////////////////////////////////// 
	static LevelManager* levelManagerInstance;
	static LevelManager* sharedManager();    

};

#endif
