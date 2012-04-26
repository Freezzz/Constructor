//
//  LevelManager.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/24/12.
//  Copyright (c) 2012 The Epic Bean. All rights reserved.
//

#include "LevelManager.h"
#include "GameLevelScene.h"
#include <dirent.h>
#include <iostream>

#define USER_LVL_PREFIX "level_user_%d_name"

#define STORY_LVL_NAME "level_story_%d_name"
#define STORY_LVL_COMPLETE "level_story_%d_complete"

//////////////////////////////////////////////////// 
// LevelManager constructor
//////////////////////////////////////////////////// 
LevelManager::LevelManager( )
{
	m_userLevels = new vector<string*>();
	m_userLevelCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("USER_LVL_COUNT", 0);
	loadStoryLevelList();
	loadUserLevelList();
	
	CCLog("LevelManager::m_userLevelCount: %d", m_userLevelCount);
}

//////////////////////////////////////////////////// 
// LevelManagerDestructor
//////////////////////////////////////////////////// 
LevelManager::~LevelManager(){

}

//////////////////////////////////////////////////// 
// Loads story mode level describtions list from storage
////////////////////////////////////////////////////
void LevelManager::loadStoryLevelList( string chapter )
{
	DIR *dir = opendir( ( string(CCFileUtils::fullPathFromRelativePath(CONSTRUCTOR_STORY_LEVEL_PATH))+chapter ).c_str() );

	struct dirent *f = readdir( dir );
	while( f ) {
		if( f->d_type == DT_REG ) {
			//std::cout << "  Level: " << f->d_name << std::endl;
			StoryLevelDescribtion *lvl = new StoryLevelDescribtion;
			lvl->isConmplete = 0;
			lvl->chapter = chapter;
			lvl->name = f->d_name;
			m_storyLevels.push_back( lvl );
		}

		f = readdir( dir );
	}
}
void LevelManager::loadStoryLevelList( )
{
	DIR *dir = opendir( CCFileUtils::fullPathFromRelativePath(CONSTRUCTOR_STORY_LEVEL_PATH) );

	struct dirent *f = readdir( dir );
	while( f ) {
		if( f->d_type == DT_DIR ) {
			//std::cout << "Chapter: " << f->d_name << std::endl;
			loadStoryLevelList( f->d_name );
		}

		f = readdir( dir );
	}
}

//////////////////////////////////////////////////// 
// Loads user create level list from storage
//////////////////////////////////////////////////// 
void LevelManager::loadUserLevelList(){
	for (int i = 1; i< m_userLevelCount+1; i++) {
		char temp[20];
		sprintf(temp, USER_LVL_PREFIX, i);
		string lvlName = CCUserDefault::sharedUserDefault()->getStringForKey(temp, "");
		if (lvlName != "") {
			m_userLevels->push_back(new string(lvlName));
		}
	}
}

//////////////////////////////////////////////////// 
// Load level defenition by level name
//////////////////////////////////////////////////// 
LevelDef * LevelManager::loadUserLevel( const char *fileName ){
	return LevelDef::loadFromFile(fileName);
}

//////////////////////////////////////////////////// 
// Saves user level and updates list of user created levels
//////////////////////////////////////////////////// 
bool LevelManager::saveUserLevel(const char *fileName){
	// TODO: overwrite
	
	LevelDef * level = GameLevelScene::sharedGameScene()->getCurrentLevelDef();
	level->name = string(fileName);
	
	if (level->saveToFile(fileName)) {
		m_userLevelCount++;
		char temp[10];
		sprintf(temp, USER_LVL_PREFIX, m_userLevelCount);

		m_userLevels->push_back(new string(level->name));
		
		CCUserDefault::sharedUserDefault()->setStringForKey(temp, level->name);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("USER_LVL_COUNT", m_userLevelCount);
		CCUserDefault::sharedUserDefault()->flush();
		delete level;
		return true;
	}
	delete level;	
	return false;
}


//////////////////////////////////////////////////// 
// Singleton pattern
////////////////////////////////////////////////////
LevelManager* LevelManager::levelManagerInstance = NULL;
LevelManager* LevelManager::sharedManager(){
	if (!levelManagerInstance) {
		levelManagerInstance = new LevelManager();
	}
	return levelManagerInstance;
}