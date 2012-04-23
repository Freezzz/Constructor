
#include "LevelDef.h"
#include "GameWorld.h"
#include <iostream>
#include <fstream>
#include "Serialization/json/json.h"
#include "Serialization/b2dJson.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/ObjectSimpleBox.h"
#include "GameObjects/ObjectGlue.h"
#include "GameObjects/ObjectPin.h"
#include "GameObjects/ObjectSpring.h"
#include "Constants.h"
#include <Box2D/Box2D.h>

std::ostream& operator<<( std::ostream &out, LevelDef &l )
{
	out << "     Level name: " << l.name << std::endl;
	out << "     Difficulty: " << l.difficulty << std::endl;
	out << "          Theme: " << l.theme << std::endl;
	out << "      GameWorld with " << l.gameWorld->physicsWorld->GetBodyCount() << " bodies" << std::endl;
	out << "      Inventory with " << l.inventoryItems.size() << " items" << std::endl;
	out << " Win conditions: " << l.winConditions << std::endl;
	out << "Lose conditions: " << l.loseConditions << std::endl;
	return out;
}

LevelDef::LevelDef( )
	: difficulty(0), gameWorld(0)
{
}
LevelDef::~LevelDef( )
{
	delete gameWorld;
}

LevelDef* LevelDef::loadFromFile( const char *fileName )
{
	std::string filePath = CCFileUtils::getWriteablePath() + fileName;
	
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
		Json::Value json;

		{
			if( ! reader.parse(ifs, json) )
			{
				std::cout  << "Failed to parse " << filePath << std::endl << reader.getFormattedErrorMessages();
				ifs.close();
				delete l;
				return NULL;
			}

			// level description
			l->name = json["name"].asString();
			l->difficulty = json["difficulty"].asFloat();

			// level style
			l->theme = json["theme"].asString();

			{
				b2dJson b2dj( true );
				
				// level physical world
				{
					Json::Value worldJson;
					worldJson = json["game world"];

					b2World *physicsWorld = b2dj.j2b2World( worldJson["physics world"] );
					b2Body *nullBody = b2dj.getBodyByName( "null body" );
					l->gameWorld = GameWorld::node( physicsWorld, nullBody );
				}

				// inventory items
				{
					int items = json["inventory items"].size();
					for( int i = 0; i < items; ++i ) {
						ObjectType type = static_cast<ObjectType>( json["inventory items"][i]["type"].asInt() );

						InventoryItem *item;
						switch( type ) {
							case SimpleBox:
							case FixedPoint:
							case Panel:
								{
									std::string itemSprite = json["inventory items"][i]["item sprite path"].asString();
									std::string objectSprite = json["inventory items"][i]["object sprite path"].asString();
									b2FixtureDef *fixtureDef = b2dj.j2b2FixtureDef( json["inventory items"][i]["fixtureDef"] );

									// l->inventoryItems.push_back( SimpleBoxInventoryItem::node(type) ); // TODO: load fixtureDef
									item = SimpleBoxInventoryItem::node( itemSprite, objectSprite, fixtureDef );
									break;
								}
							case Spring:
								item = SpringInventoryItem::node();
								break;
							case Pin:
								item = PinInventoryItem::node();
								break;
							case Glue:
								item = GlueInventoryItem::node();
								break;
							default:
								std::cout << "Invalid inventory item" << std::endl;
						}
						l->inventoryItems.push_back( item );

						item->isStatic = json["inventory items"][i]["isStatic"].asBool();
						item->isMovable = json["inventory items"][i]["isMovable"].asBool();
						item->isRotatable = json["inventory items"][i]["isRotatable"].asBool();
						item->isDeletable = json["inventory items"][i]["isDeletable"].asBool();
					}
				}
			}

			// win and lose conditions
			l->winConditions = static_cast<LevelDef::WinConditions>( json["win conditions"].asInt() );
			l->loseConditions = static_cast<LevelDef::LoseConditions>( json["lose conditions"].asInt() );
		}

		ifs.close();
	}

	std::cout << "Level loaded:" << std::endl;
	std::cout << *l << std::endl;

	return l;
}
bool LevelDef::saveToFile( const char *fileName )
{
	std::string filePath = CCFileUtils::getWriteablePath() + fileName;
	
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
		Json::Value json;

		{
			// level description
			json["name"] = name;
			json["difficulty"] = difficulty;

			// level style
			json["theme"] = theme;

			{
				b2dJson b2dj( true );
				
				// level physical world
				Json::Value worldJson;
				{
					b2dj.setBodyName( gameWorld->umbelicoDelMondo, "null body" );
					worldJson["physics world"] = b2dj.b2j( gameWorld->physicsWorld );
				}
				json["game world"] = worldJson;

				// inventory items
				for( unsigned int i = 0; i < inventoryItems.size(); i++ ) {
					InventoryItem *item = inventoryItems.at(i);
					json["inventory items"][i]["type"] = item->getObjectType();

					json["inventory items"][i]["isStatic"] = item->isStatic;
					json["inventory items"][i]["isMovable"] = item->isMovable;
					json["inventory items"][i]["isRotatable"] = item->isRotatable;
					json["inventory items"][i]["isDeletable"] = item->isDeletable;

					if( item->getObjectType() == SimpleBox || item->getObjectType() == FixedPoint || item->getObjectType() == Panel ) {
						SimpleBoxInventoryItem *sbitem = static_cast<SimpleBoxInventoryItem*>( item );
						json["inventory items"][i]["fixtureDef"] = b2dj.b2j( sbitem->m_fixtureDef );
						json["inventory items"][i]["item sprite path"] = sbitem->m_itemSpritePath;
						json["inventory items"][i]["object sprite path"] = sbitem->m_objectSpritePath;
					}
				}
			}

			// win and lose conditions
			json["win conditions"] = winConditions;
			json["lose conditions"] = loseConditions;
		}

		writer.write( ofs, json );
		ofs.close();
	}

	std::cout << "Level saved." << std::endl;
	return 1;
}
