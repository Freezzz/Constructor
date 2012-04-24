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

LevelManager::LevelManager(){
	m_userLevels = vector<string*>();
	m_storyLevels = vector<StoryLevelDescribtion*>();	
	m_userLevelCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("USER_LVL_COUNT", 0);	
	m_storyLevelCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("STORY_LVL_COUNT", 0);	
	loadStoryLevelList();
	loadUserLevelList();
}

LevelManager::~LevelManager(){

}

void LevelManager::loadStoryLevelList(){
	for (unsigned int i = 0; i< m_storyLevelCount; i++) {
		StoryLevelDescribtion * lvl = new StoryLevelDescribtion;

		char temp[10];
		sprintf(temp, STORY_LVL_NAME, i);
		lvl->name = CCUserDefault::sharedUserDefault()->getStringForKey(temp, "");		
		
		sprintf(temp, STORY_LVL_COMPLETE, i);
		lvl->name = CCUserDefault::sharedUserDefault()->getBoolForKey(temp, false);
		
		m_storyLevels.push_back(lvl);
	}
}

void LevelManager::loadUserLevelList(){
	for (unsigned int i = 0; i< m_userLevelCount; i++) {
		char temp[10];
		sprintf(temp, USER_LVL_PREFIX, i);
		string lvlName = CCUserDefault::sharedUserDefault()->getStringForKey(temp, "");
		if (lvlName != "") {
			m_userLevels.push_back(&lvlName);
		}
	}
}

LevelDef * LevelManager::loadUserLevel( const char *fileName ){
	return LevelDef::loadFromFile(fileName);
}

bool LevelManager::saveUserLevel( LevelDef * level, const char *fileName ){
	// TODO: overwrite
	if (level->saveToFile(fileName)) {
		m_userLevelCount++;
		char temp[10];
		sprintf(temp, USER_LVL_PREFIX, m_userLevelCount);
		CCUserDefault::sharedUserDefault()->setStringForKey(temp, fileName);
		m_userLevels.push_back(new string(fileName));
		
		CCUserDefault::sharedUserDefault()->setIntegerForKey("STORY_LVL_COUNT", m_userLevelCount);	
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