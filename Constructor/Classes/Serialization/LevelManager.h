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

using namespace cocos2d;

class LevelDef;

#define CONSTRUCTOR_LEVEL_PATH            "levels/"
#define CONSTRUCTOR_STORY_LEVEL_PATH      CCFileUtils::fullPathFromRelativePath( CONSTRUCTOR_LEVEL_PATH "story_levels" ) + string("/")
#define CONSTRUCTOR_SANDBOX_LEVEL_PATH    CCFileUtils::fullPathFromRelativePath( CONSTRUCTOR_LEVEL_PATH "sandboxes/" ) + string("/")

#define CONSTRUCTOR_USER_LEVEL_PATH       ( ( CCFileUtils::getWriteablePath().empty() ? "." : CCFileUtils::getWriteablePath() ) + "/levels/" )

#define CONSTRUCTOR_INVITEMS_PATH         CCFileUtils::fullPathFromRelativePath( "items" )
#define CONSTRUCTOR_GAMEOBJECTS_PATH      CCFileUtils::fullPathFromRelativePath( "objects" )

inline std::string inventoryItemPath( const std::string &ii )
{
	return std::string( CONSTRUCTOR_INVITEMS_PATH ) + string("/") + ii + ".ciz";
}
inline std::string gameObjectPath( const std::string &ii )
{
	return std::string( CONSTRUCTOR_GAMEOBJECTS_PATH ) + string("/") + ii + ".coz";
}

struct LevelDescription
{
	virtual ~LevelDescription( ) { }
	virtual string getPath( ) const = 0;
};
struct StoryLevelDescription : public LevelDescription
{
	bool isComplete;
	string chapter;
	string name;

	string getPath( ) const
	{
		return string(CONSTRUCTOR_STORY_LEVEL_PATH) + chapter + "/" + name;
	}
};
struct SandboxLevelDescription : public LevelDescription
{
	string name;

	SandboxLevelDescription( const string &name ) : name(name) { }

	string getPath( ) const
	{
		return CONSTRUCTOR_SANDBOX_LEVEL_PATH + name;
	}
};
struct UserLevelDescription : public LevelDescription
{
	string name;

	UserLevelDescription( const string &name ) : name(name) { }

	string getPath( ) const
	{
		return CONSTRUCTOR_USER_LEVEL_PATH + name;
	}
};

class LevelManager {
private:
	vector<UserLevelDescription*>  m_userLevels;
	vector<StoryLevelDescription*> m_storyLevels;
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
	
	vector<UserLevelDescription*> * getUserLevelList()
	{
		return & m_userLevels;
	}
	vector<StoryLevelDescription*> * getStoryLevelList( )
	{
		return & m_storyLevels;
	}

	int getCurrentUserLevelIndex(){
		return m_currentUserLevel;
	}
	
	void completeUserLevel( LevelDescription *level );
	void loadNextStoryLevel();
	
	//////////////////////////////////////////////////// 
    // Singleton pattern
    //////////////////////////////////////////////////// 
	static LevelManager* levelManagerInstance;
	static LevelManager* sharedManager();    

};

#endif
