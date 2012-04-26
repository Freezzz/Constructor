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
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>

#define USER_LVL_PREFIX "level_user_%d_name"

#define STORY_LVL_NAME "level_story_%d_name"
#define STORY_LVL_COMPLETE "level_story_%d_complete"

//////////////////////////////////////////////////// 
// LevelManager constructor
//////////////////////////////////////////////////// 
LevelManager::LevelManager( )
{
	// creating user level directory
	mkdir( CONSTRUCTOR_USER_LEVEL_PATH, 0755 );

	loadStoryLevelList();
	loadUserLevelList();
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
	DIR *dir = opendir( CCFileUtils::fullPathFromRelativePath( string(CONSTRUCTOR_STORY_LEVEL_PATH+chapter).c_str()) );
	if( ! dir ) {
		std::cout << "Couldn't find story level directory for chapter: " << chapter << std::endl;
		return ;
	}

	struct dirent *f = readdir( dir );
	while( f ) {
		if( f->d_type == DT_REG ) {
			std::cout << "  Level: " << f->d_name << std::endl;
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
	if( ! dir ) {
		std::cout << "Couldn't find story level directory: " << CONSTRUCTOR_STORY_LEVEL_PATH << std::endl;
		return ;
	}

	struct dirent *f = readdir( dir );
	while( f ) {
		if( f->d_type == DT_DIR && strcmp(f->d_name,".") && strcmp(f->d_name,"..") ) {
			std::cout << "Chapter: " << f->d_name << std::endl;
			loadStoryLevelList( f->d_name );
		}

		f = readdir( dir );
	}
}

//////////////////////////////////////////////////// 
// Loads user create level list from storage
//////////////////////////////////////////////////// 
void LevelManager::loadUserLevelList( )
{
	DIR *dir = opendir( CONSTRUCTOR_USER_LEVEL_PATH );
	if( ! dir ) {
		std::cout << "Couldn't find user directory: " << CONSTRUCTOR_USER_LEVEL_PATH << std::endl;
		return ;
	}
    
	struct dirent *f = readdir( dir );
	while( f ) {
		if( f->d_type != DT_DIR && strcmp(f->d_name,".") && strcmp(f->d_name,"..") ) {
			std::cout << " USER Level: " << f->d_name << std::endl;
			UserLevelDescribtion *lvl = new UserLevelDescribtion;
			lvl->name = f->d_name;
			m_userLevels.push_back( lvl );
		}
		
		f = readdir( dir );
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
	
	UserLevelDescribtion * dscr = new UserLevelDescribtion;
	dscr->name = fileName;
	
	if (level->saveToFile(dscr->getPath().c_str())) {
		m_userLevels.push_back(dscr);
		delete dscr;
		return true;
	}
	delete dscr;	
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