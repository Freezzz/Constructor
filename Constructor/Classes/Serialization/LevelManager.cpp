//
//  LevelManager.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/24/12.
//  Copyright (c) 2012 The Epic Bean. All rights reserved.
//

#include "LevelManager.h"
#define USER_LVL_PREFIX "level_user_%d"

#define STORY_LVL_NAME "level_story_%d_name"
#define STORY_LVL_COMPLETE "level_story_%d_complete"

//////////////////////////////////////////////////// 
// LevelManager constructor
//////////////////////////////////////////////////// 
LevelManager::LevelManager(){
	m_userLevels = new vector<string*>();
	m_storyLevels = new vector<StoryLevelDescribtion*>();	
	m_userLevelCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("USER_LVL_COUNT", 0);	
	m_storyLevelCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("STORY_LVL_COUNT", 0);	
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
void LevelManager::loadStoryLevelList(){
	for (unsigned int i = 0; i< m_storyLevelCount; i++) {
		StoryLevelDescribtion * lvl = new StoryLevelDescribtion;

		char temp[10];
		sprintf(temp, STORY_LVL_NAME, i);
		lvl->name = CCUserDefault::sharedUserDefault()->getStringForKey(temp, "");		
		
		sprintf(temp, STORY_LVL_COMPLETE, i);
		lvl->name = CCUserDefault::sharedUserDefault()->getBoolForKey(temp, false);
		
		m_storyLevels->push_back(lvl);
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
bool LevelManager::saveUserLevel( LevelDef * level, const char *fileName ){
	// TODO: overwrite
	if (level->saveToFile(fileName)) {
		m_userLevelCount++;
		char temp[10];
		sprintf(temp, USER_LVL_PREFIX, m_userLevelCount);

		m_userLevels->push_back(new string(level->name));
		
		CCUserDefault::sharedUserDefault()->setStringForKey(temp, level->name);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("USER_LVL_COUNT", m_userLevelCount);
		CCUserDefault::sharedUserDefault()->flush();		
		return true;
	}
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