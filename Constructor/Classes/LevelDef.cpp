
#include "LevelDef.h"
#include "GameWorld.h"
#include "GameObjects/GameObject.h"
#include <iostream>
#include <fstream>
#include "Serialization/ConstructorJSon.h"
#include "Constants.h"
#include <Box2D/Box2D.h>

std::ostream& operator<<( std::ostream &out, LevelDef &l )
{
	out << "     Level name: " << l.name << std::endl;
	out << "     Difficulty: " << l.difficulty << std::endl;
	out << "          Theme: " << l.theme << std::endl;
	out << "      GameWorld with " << l.gameWorld->physicsWorld->GetBodyCount() << " bodies" << std::endl;
	out << "      Inventory with " << l.inventoryItems.size() << " items" << std::endl;
	out << "              " << l.gameObjects.count() << " game objects" << std::endl;
	out << " Win conditions: " << l.winConditions << std::endl;
	out << "Lose conditions: " << l.loseConditions << std::endl;
	return out;
}

LevelDef::LevelDef( )
: difficulty(0), gameWorld(0), target(0)
{
}
LevelDef::~LevelDef( )
{
}

LevelDef* LevelDef::loadFromFile( const char *fileName )
{
	std::string filePath = CCFileUtils::fullPathFromRelativePath(fileName);
	
	LevelDef *l = new LevelDef;
	std::cout << "Loading level " << filePath << "..." << std::endl;
    
	{
		std::ifstream ifs;
		ifs.open( filePath.c_str(), std::ios::in );
		if (!ifs) {
			std::cout << "Could not open file " << filePath << " for reading" << std::endl;
			return NULL;
		}
        
		Json::Reader reader;
		ConstructorJSon cjs;
		Json::Value json;
        
		if( ! reader.parse(ifs, json) )
		{
			std::cout  << "Failed to parse " << filePath << std::endl << reader.getFormattedErrorMessages();
			ifs.close();
			delete l;
			return NULL;
		}
        
		l = cjs.j2cLevelDef( json );
        
		ifs.close();
	}
    
	std::cout << "Level loaded:" << std::endl;
	std::cout << *l << std::endl;
    
	return l;
}
bool LevelDef::saveToFile( const char *fileName )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	std::string filePath = fileName;
#else
    std::string filePath = CCFileUtils::getWriteablePath() + fileName;
#endif  
    
    
	
	std::cout << "Saving level " << filePath << "..." << std::endl;
	std::cout << (*this) << std::endl;
    
	{
		std::ofstream ofs;
		ofs.open( filePath.c_str(), std::ios::out );
		if( ! ofs ) {
			std::cout << "Could not open file " << filePath << " for writing" << std::endl;
			return 0;
		}
        
		Json::StyledStreamWriter writer( "   " );
		ConstructorJSon cjs;
		Json::Value json = cjs.cj( this );
        
		writer.write( ofs, json );
		ofs.close();
	}
    
	std::cout << "Level saved." << std::endl;
	return 1;
}
