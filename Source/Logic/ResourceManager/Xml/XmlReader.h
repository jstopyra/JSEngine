#ifndef __XMLREADER_H__
#define __XMLREADER_H__

#include <tinyxml2.h>
#include <string>
#include "../../Components/TransformComponent.h"
#include "../../Components/RenderComponent.h"
#include "../../../Utilities/DebugLog.h"
#include "../BaseVirtual/Resource.h"
#include "../../GameObject/GameObjectManager.h"
#include <unordered_map>

class TiledObjectLayer;
class Resource;

class XmlReader
{

public:
	//Loads the game map tiles from a xml file created with a Tiled tool
	static void LoadTiledMap(TiledManager* pTiledManager, const char* mapName);
	
};

#endif // !__XMLREADER_H__
