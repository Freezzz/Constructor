
#include "ConstructorJSon.h"

#include "../GameObjects/GameObject.h"
#include "../GameObjects/ObjectSimpleBox.h"
#include "../GameObjects/ObjectArea.h"
#include "../GameObjects/ObjectGlue.h"
#include "../GameObjects/ObjectPin.h"
#include "../GameObjects/ObjectSpring.h"

#include "LevelDef.h"

#include "GameWorld.h"

ConstructorJSon::ConstructorJSon( )
	: b2dJson(1)
{
}

Json::Value ConstructorJSon::cj( LevelDef* levelDef )
{
	m_inventoryItemIndexMap.clear();
	m_gameObjectIndexMap.clear();
	
	Json::Value value;
	
	// level description
	value["name"] = levelDef->name;
	value["difficulty"] = levelDef->difficulty;

	// level style
	value["theme"] = levelDef->theme;

	// level physical world
	Json::Value worldJson;
	{
		setBodyName( levelDef->gameWorld->umbelicoDelMondo, "null body" );
		worldJson["physics world"] = b2j( levelDef->gameWorld->physicsWorld );
	}
	value["game world"] = worldJson;

	// inventory items
	for( unsigned int i = 0; i < levelDef->inventoryItems.size(); i++ ) {
		InventoryItem *item = levelDef->inventoryItems.at(i);
		m_inventoryItemIndexMap[item] = i;
		value["inventory items"][i] = cj( item );
	}

	// game objects
	for( unsigned int i = 0; i < levelDef->gameObjects.count(); i++ ) {
		GameObject *object = levelDef->gameObjects.getObjectAtIndex(i);
		m_gameObjectIndexMap[object] = i;
		value["game objects"][i] = cj( object );
	}
	{
		value["target"] = this->lookupGameObjectIndex( levelDef->target );
	}

	// win and lose conditions
	value["win conditions"] = levelDef->winConditions;
	value["lose conditions"] = levelDef->loseConditions;

	return value;
}
Json::Value ConstructorJSon::b2j( const b2Shape* shape )
{
	Json::Value shapeValue;
	switch (shape->GetType())
	{
	case b2Shape::e_circle:
		{
			b2CircleShape* circle = (b2CircleShape*)shape;
			floatToJson("radius", circle->m_radius, shapeValue["circle"]);
			vecToJson("center", circle->m_p, shapeValue["circle"]);
		}
		break;
	case b2Shape::e_edge:
		{
			b2EdgeShape* edge = (b2EdgeShape*)shape;
			vecToJson("vertex1", edge->m_vertex1, shapeValue["edge"]);
			vecToJson("vertex2", edge->m_vertex2, shapeValue["edge"]);
			if ( edge->m_hasVertex0 )
				shapeValue["edge"]["hasVertex0"] = true;
			if ( edge->m_hasVertex3 )
				shapeValue["edge"]["hasVertex3"] = true;
			if ( edge->m_hasVertex0 )
				vecToJson("vertex0", edge->m_vertex0, shapeValue["edge"]);
			if ( edge->m_hasVertex3 )
				vecToJson("vertex3", edge->m_vertex3, shapeValue["edge"]);
		}
		break;
	/* r197
	case b2Shape::e_loop:
		{
			b2LoopShape* loop = (b2LoopShape*)shape;
			int32 count = loop->GetCount();
			const b2Vec2* vertices = loop->GetVertices();
			for (int32 i = 0; i < count; ++i)
				vecToJson("vertices", vertices[i], shapeValue["loop"], i);
		}
		break;
	*/
	case b2Shape::e_chain:
		{
			b2ChainShape* chain = (b2ChainShape*)shape;
			int32 count = chain->m_count;
			const b2Vec2* vertices = chain->m_vertices;
			for (int32 i = 0; i < count; ++i)
				vecToJson("vertices", vertices[i], shapeValue["chain"], i);
			if ( chain->m_hasPrevVertex )
				shapeValue["chain"]["hasPrevVertex"] = true;
			if ( chain->m_hasNextVertex )
				shapeValue["chain"]["hasNextVertex"] = true;
			if ( chain->m_hasPrevVertex )
				vecToJson("prevVertex", chain->m_prevVertex, shapeValue["chain"]);
			if ( chain->m_hasNextVertex )
				vecToJson("nextVertex", chain->m_nextVertex, shapeValue["chain"]);
		}
		break;
	case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)shape;
			int32 vertexCount = poly->m_vertexCount;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			for (int32 i = 0; i < vertexCount; ++i)
				vecToJson("vertices", poly->m_vertices[i], shapeValue["polygon"], i);
		}
		break;
	default:
		std::cout << "Unknown shape type : " << shape->GetType() << std::endl;
	}
	return shapeValue;
}
Json::Value ConstructorJSon::b2j( const b2FixtureDef* fixtureDef )
{
	Json::Value fixtureDefValue;

	fixtureDefValue["shape"] = b2j( fixtureDef->shape );
	floatToJson( "friction", fixtureDef->friction, fixtureDefValue );
	floatToJson( "restitution", fixtureDef->restitution, fixtureDefValue );
	floatToJson( "density", fixtureDef->density, fixtureDefValue );
	fixtureDefValue["isSensor"] = fixtureDef->isSensor;

	b2Filter filter = fixtureDef->filter;
	if ( filter.categoryBits != 0x0001 )
		fixtureDefValue["filter-categoryBits"] = filter.categoryBits;
	if ( filter.maskBits != 0xffff )
		fixtureDefValue["filter-maskBits"] = filter.maskBits;
	if ( filter.groupIndex != 0 )
		fixtureDefValue["filter-groupIndex"] = filter.groupIndex;

	return fixtureDefValue;
}
Json::Value ConstructorJSon::cj( InventoryItem* item )
{
	Json::Value itemValue;

	itemValue["type"] = item->getObjectType();

	itemValue["isStatic"] = item->isStatic;
	itemValue["isMovable"] = item->isMovable;
	itemValue["isRotatable"] = item->isRotatable;
	itemValue["isDeletable"] = item->isDeletable;

	itemValue["item sprite path"] = item->m_itemSpritePath;
	itemValue["object sprite path"] = item->m_objectSpritePath;

	if( item->getObjectType() == SimpleBox ) {
		SimpleBoxInventoryItem *sbitem = static_cast<SimpleBoxInventoryItem*>( item );
		itemValue["fixtureDef"] = b2j( sbitem->m_fixtureDef );
	}

	return itemValue;
}
Json::Value ConstructorJSon::cj( GameObject* gameObject )
{
	Json::Value objectValue;

	objectValue["body"] = lookupBodyIndex( gameObject->getObjectBody() );
	objectValue["inventory item"] = lookupInventoryItemIndex( gameObject->getInventoryItem() );

	objectValue["isStatic"] = gameObject->isStatic;
	objectValue["isMovable"] = gameObject->isMovable;
	objectValue["isRotatable"] = gameObject->isRotatable;
	objectValue["isDeletable"] = gameObject->isDeletable;

	return objectValue;
}



LevelDef* ConstructorJSon::j2cLevelDef( Json::Value value )
{
	LevelDef *l = new LevelDef;
	
	// level description
	l->name = value["name"].asString();
	l->difficulty = value["difficulty"].asFloat();

	// level style
	l->theme = value["theme"].asString();

	// level physical world
	{
		Json::Value worldJson;
		worldJson = value["game world"];

		b2World *physicsWorld = j2b2World( worldJson["physics world"] );
		b2Body *nullBody = getBodyByName( "null body" );
		l->gameWorld = GameWorld::node( physicsWorld, nullBody );
	}

	// inventory items
	{
		int items = value["inventory items"].size();
		for( int i = 0; i < items; ++i ) {
			InventoryItem *item = j2cInventoryItem( value["inventory items"][i] );
			l->inventoryItems.push_back( item );
			m_inventoryItems.push_back( item );
		}
	}

	// game objects
	{
		int items = value["game objects"].size();
		for( int i = 0; i < items; ++i ) {
			GameObject *object = j2cGameObject( value["game objects"][i] );
			l->gameObjects.addObject( object );
			m_gameObjects.push_back( object );
		}
	}
	{
		int targetIndex = value["target"].asInt();
		if( targetIndex < (int) m_bodies.size() ) {
			l->target = m_gameObjects[targetIndex];
		}
	}

	// win and lose conditions
	l->winConditions = static_cast<LevelDef::WinConditions>( value["win conditions"].asInt() );
	l->loseConditions = static_cast<LevelDef::LoseConditions>( value["lose conditions"].asInt() );

	return l;
}
b2Shape* ConstructorJSon::j2b2Shape( Json::Value shapeValue )
{
	if ( !shapeValue["circle"].isNull() ) {
		b2CircleShape *circleShape = new b2CircleShape;
		circleShape->m_radius = jsonToFloat("radius", shapeValue["circle"]);
		circleShape->m_p = jsonToVec("center", shapeValue["circle"]);
		return circleShape;
	}
	else if ( !shapeValue["edge"].isNull() ) {
		b2EdgeShape *edgeShape = new b2EdgeShape;
		edgeShape->m_vertex1 = jsonToVec("vertex1", shapeValue["edge"]);
		edgeShape->m_vertex2 = jsonToVec("vertex2", shapeValue["edge"]);
		edgeShape->m_hasVertex0 = shapeValue["edge"].get("hasVertex0",false).asBool();
		edgeShape->m_hasVertex3 = shapeValue["edge"].get("hasVertex3",false).asBool();
		if ( edgeShape->m_hasVertex0 )
			edgeShape->m_vertex0 = jsonToVec("vertex0", shapeValue["edge"]);
		if ( edgeShape->m_hasVertex3 )
			edgeShape->m_vertex3 = jsonToVec("vertex3", shapeValue["edge"]);
		return edgeShape;
	}
	else if ( !shapeValue["loop"].isNull() ) { //support old format (r197)
		b2ChainShape *chainShape = new b2ChainShape;
		int numVertices = shapeValue["loop"]["vertices"]["x"].size();
		b2Vec2* vertices = new b2Vec2[numVertices];
		for (int i = 0; i < numVertices; i++)
			vertices[i] = jsonToVec("vertices", shapeValue["loop"], i);
		chainShape->CreateLoop(vertices, numVertices);
		delete vertices;
		return chainShape;
	}
	else if ( !shapeValue["chain"].isNull() ) {
		b2ChainShape *chainShape = new b2ChainShape;
		int numVertices = shapeValue["chain"]["vertices"]["x"].size();
		b2Vec2* vertices = new b2Vec2[numVertices];
		for (int i = 0; i < numVertices; i++)
			vertices[i] = jsonToVec("vertices", shapeValue["chain"], i);
		chainShape->CreateChain(vertices, numVertices);
		chainShape->m_hasPrevVertex = shapeValue["chain"].get("hasPrevVertex",false).asBool();
		chainShape->m_hasNextVertex = shapeValue["chain"].get("hasNextVertex",false).asBool();
		if ( chainShape->m_hasPrevVertex )
			chainShape->m_prevVertex = jsonToVec("prevVertex", shapeValue["chain"]);
		if ( chainShape->m_hasNextVertex )
			chainShape->m_nextVertex = jsonToVec("nextVertex", shapeValue["chain"]);
		delete vertices;
		return chainShape;
	}
	else if ( !shapeValue["polygon"].isNull() ) {
		b2PolygonShape *polygonShape = new b2PolygonShape;
		b2Vec2 vertices[b2_maxPolygonVertices];
		int numVertices = shapeValue["polygon"]["vertices"]["x"].size();
		for (int i = 0; i < numVertices; i++) {
			vertices[i] = jsonToVec("vertices", shapeValue["polygon"], i);
		}
		polygonShape->Set(vertices, numVertices);
		return polygonShape;
	}
	else {
		std::cout << "Unknown shape type not stored in json" << std::endl;
		return NULL;
	}
}
b2FixtureDef* ConstructorJSon::j2b2FixtureDef( Json::Value fixtureDefValue )
{
	b2FixtureDef *fixtureDef = new b2FixtureDef;

	fixtureDef->shape = j2b2Shape( fixtureDefValue["shape"] );
	if( ! fixtureDef->shape ) {
		delete fixtureDef;
		return NULL;
	}

	fixtureDef->friction = jsonToFloat("friction", fixtureDefValue);
	fixtureDef->restitution = jsonToFloat("restitution", fixtureDefValue);
	fixtureDef->density = jsonToFloat("density", fixtureDefValue);
	fixtureDef->isSensor = fixtureDefValue["isSensor"].asBool();

	fixtureDef->filter.categoryBits = fixtureDefValue.get("filter-categoryBits",0x0001).asInt();
	fixtureDef->filter.maskBits = fixtureDefValue.get("filter-maskBits",0xffff).asInt();
	fixtureDef->filter.groupIndex = fixtureDefValue.get("filter-groupIndex",0).asInt();

	return fixtureDef;
}
InventoryItem* ConstructorJSon::j2cInventoryItem( Json::Value itemValue )
{
	ObjectType type = static_cast<ObjectType>( itemValue["type"].asInt() );
	std::string itemSprite = itemValue["item sprite path"].asString();
	std::string objectSprite = itemValue["object sprite path"].asString();

	InventoryItem *item;
	switch( type ) {
		case SimpleBox:
			{
				b2FixtureDef *fixtureDef = j2b2FixtureDef( itemValue["fixtureDef"] );

				// l->inventoryItems.push_back( SimpleBoxInventoryItem::node(type) ); // TODO: load fixtureDef
				item = SimpleBoxInventoryItem::node( itemSprite, objectSprite, fixtureDef );
				break;
			}
		case Area:
			item = AreaInventoryItem::node( itemSprite, objectSprite );
			break;
		case Spring:
			item = SpringInventoryItem::node( itemSprite, objectSprite );
			break;
		case Pin:
			item = PinInventoryItem::node( itemSprite, objectSprite );
			break;
		case Glue:
			item = GlueInventoryItem::node( itemSprite, objectSprite );
			break;
		default:
			CCAssert( false, "Invalid inventory item" );
	}

	item->isStatic = itemValue["isStatic"].asBool();
	item->isMovable = itemValue["isMovable"].asBool();
	item->isRotatable = itemValue["isRotatable"].asBool();
	item->isDeletable = itemValue["isDeletable"].asBool();

	return item;
}
GameObject* ConstructorJSon::j2cGameObject( Json::Value objectValue )
{
	int itemIndex = objectValue["inventory item"].asInt();
	int bodyIndex = objectValue["body"].asInt();

	if( bodyIndex >= (int) m_bodies.size() ) {
		return NULL;
	}
	if( itemIndex >= (int) m_inventoryItems.size() ) {
		return NULL;
	}

	InventoryItem *item = m_inventoryItems[itemIndex];
	b2Body *body = m_bodies[bodyIndex];
	
	GameObject *object = item->gameObjectNode( body );

	object->isStatic = objectValue["isStatic"].asBool();
	object->isMovable = objectValue["isMovable"].asBool();
	object->isRotatable = objectValue["isRotatable"].asBool();
	object->isDeletable = objectValue["isDeletable"].asBool();
	
	return object;
}


void ConstructorJSon::setInventoryItemName( InventoryItem* item, const char* name )
{
    m_inventoryItemToNameMap[item] = name;
}
void ConstructorJSon::setGameObjectName( GameObject* object, const char* name )
{
    m_gameObjectToNameMap[object] = name;
}


InventoryItem* ConstructorJSon::getInventoryItemByName( std::string name )
{
	std::map<InventoryItem*,std::string>::iterator it = m_inventoryItemToNameMap.begin();
	while( it != m_inventoryItemToNameMap.end() ) {
		if( it->second == name )
			return it->first;
		++it;
	}
	return 0;
}
GameObject* ConstructorJSon::getGameObjectByName( std::string name )
{
	std::map<GameObject*,std::string>::iterator it = m_gameObjectToNameMap.begin();
	while( it != m_gameObjectToNameMap.end() ) {
		if( it->second == name )
			return it->first;
		++it;
	}
	return 0;
}


int ConstructorJSon::lookupInventoryItemIndex( InventoryItem* item )
{
	std::map<InventoryItem*,int>::iterator it = m_inventoryItemIndexMap.find( item );
	if( it != m_inventoryItemIndexMap.end() ) {
		return it->second;
	}
	return 0;
}
int ConstructorJSon::lookupGameObjectIndex( GameObject* object )
{
	std::map<GameObject*,int>::iterator it = m_gameObjectIndexMap.find( object );
	if( it != m_gameObjectIndexMap.end() ) {
		return it->second;
	}
	return 0;
}


std::string ConstructorJSon::getInventoryItemName( InventoryItem* item )
{
	std::map<InventoryItem*,std::string>::iterator it = m_inventoryItemToNameMap.find( item );
	if ( it == m_inventoryItemToNameMap.end() ) {
		return "";
	}
	return it->second;
}
std::string ConstructorJSon::getGameObjectName( GameObject* object )
{
	std::map<GameObject*,std::string>::iterator it = m_gameObjectToNameMap.find( object );
	if ( it == m_gameObjectToNameMap.end() ) {
		return "";
	}
	return it->second;
}
