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

#define CONSTRUCTOR_LEVEL_PATH            "levels/"
#define CONSTRUCTOR_STORY_LEVEL_PATH      CONSTRUCTOR_LEVEL_PATH "story_levels/"
#define CONSTRUCTOR_SANDBOX_LEVEL_PATH    CONSTRUCTOR_LEVEL_PATH "sandboxes/"

#define CONSTRUCTOR_USER_LEVEL_PATH       ( ( ( CCFileUtils::getWriteablePath().empty() ? "." : CCFileUtils::getWriteablePath() ) + "/levels" ).c_str() )

struct StoryLevelDescribtion
{
	bool isComplete;
	string chapter;
	string name;
	string getPath( ) const
	{
		return string( string(CONSTRUCTOR_STORY_LEVEL_PATH) + chapter + "/" + name );
	}
};
struct UserLevelDescribtion
{
	string name;
	string getPath( ) const
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		return string( string(CCFileUtils::getWriteablePath() + "levels/" + name ));
#else
        return string( string(CCFileUtils::getWriteablePath() + "/" + name ));
#endif
	}
};

class LevelManager {
private:
	vector<UserLevelDescribtion*>  m_userLevels;
	vector<StoryLevelDescribtion*> m_storyLevels;
	int m_currentUserLevel;
	int m_lastComleteIndex;
	
	void loadUserLevelList();
	void loadStoryLevelList( string chapter );
	void loadStoryLevelList();
	
public:
	LevelManager( );
	~LevelManager( );

	LevelDef *loadUserLevel( const char *fileName );
	bool saveUserLevel(const char *fileName);
	
	vector<UserLevelDescribtion*> * getUserLevelList()
	{
		return & m_userLevels;
	}
	vector<StoryLevelDescribtion*> * getStoryLevelList( )
	{
		return & m_storyLevels;
	}

	int getCurrentUserLevelIndex(){
		return m_currentUserLevel;
	}
	
	void completeUserLevel(const char * levelName);
	void loadNextStoryLevel();
	
	//////////////////////////////////////////////////// 
    // Singleton pattern
    //////////////////////////////////////////////////// 
	static LevelManager* levelManagerInstance;
	static LevelManager* sharedManager();    

};

#endif
