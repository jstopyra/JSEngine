#include "XmlReader.h"

#include "../TiledManager/TiledObjectLayer.h"
#include "../../../Rendering/SDL/SdlRenderDevice.h"
#include "../../Components/ComponentIncludes.h"

#include "../TiledManager/TiledManager.h"
#include <string>
#include <utility>

#include "../TiledManager/TileSet.h"
#include "../../LuaPlus/LuaPlusSingleton.h"
#include "../../Components/MovementComponent.h"
#include "../../Components/LuaScriptComponent.h"
#include "../../Components/ButtonComponent.h"
#include "Utilities\DebugLog.h"
extern RenderDevice* g_pRenderDevice;


//TODO: This function is too masive!!! needs to be broken down into smaller pieces
void XmlReader::LoadTiledMap(TiledManager* pTiledManager, const char* mapName)
{

	//This is for debuggin. Check what value is the element.
	//std::string elementName3 = pElement->Value();

	//---------------------------------------------------------------------------------------------
	//File Setup
	//---------------------------------------------------------------------------------------------
	//Error handle
	tinyxml2::XMLError err;
	//our xml document
	tinyxml2::XMLDocument xmlDoc;
	//Loading the xml file and storing any errors

	//create a path to the map file
	std::string mapNameString("Media/Maps/");
	mapNameString.append(mapName);
	mapNameString.append(".tmx");

	err = xmlDoc.LoadFile(mapNameString.c_str());
	//find the root node
	tinyxml2::XMLNode* pRoot = xmlDoc.FirstChild();
	if (err != tinyxml2::XMLError::XML_NO_ERROR)
	{
		DEBUGLOG("Loading Tiled Map failed: " << err);
		return;
	}
	//this is the map element
	tinyxml2::XMLElement* pElement = pRoot->NextSiblingElement();

	//---------------------------------------------------------------------------------------------
	//Check the size of the map and the tiles
	//---------------------------------------------------------------------------------------------

	int mapWidth = 0;
	int mapHeight = 0;
	int tileWidth = 0;
	int tileHeigh = 0;

	pElement->QueryIntAttribute("width", &mapWidth);
	pElement->QueryIntAttribute("height", &mapHeight);
	pElement->QueryIntAttribute("tilewidth", &tileWidth);
	pElement->QueryIntAttribute("tileheight", &tileHeigh);

	//---------------------------------------------------------------------------------------------
	//Get all the tilesets
	//---------------------------------------------------------------------------------------------

	//Get the tileset element
	tinyxml2::XMLElement * pTileSetElement = pElement->FirstChildElement("tileset");

	//loop untill we read all the tilesets
	while (pTileSetElement != nullptr)
	{
		int imageWidth = 0;
		int imageHeight = 0;
		int firstGid = 0;
		const char* tilesetName = nullptr;
		int tilesetTileWidth = 0;
		int tilesetTileHeight = 0;

		tinyxml2::XMLElement* imgElement = pTileSetElement->FirstChildElement();
		std::string elementName2 = imgElement->Value();
		imgElement->QueryIntAttribute("width", &imageWidth);
		imgElement->QueryIntAttribute("height", &imageHeight);

		pTileSetElement->QueryIntAttribute("firstgid", &firstGid);
		tilesetName = pTileSetElement->Attribute("name");
		pTileSetElement->QueryIntAttribute("tilewidth", &tilesetTileWidth);
		pTileSetElement->QueryIntAttribute("tileheight", &tilesetTileHeight);

		//Set up the path to the image:
		std::string imagePath("Media/Textures/");
		imagePath.append(tilesetName);
		imagePath.append(".png");

		//Create and store the tileset
		SDL_Log("Trying to load map: %s", imagePath.c_str());
		//DEBUGLOG(fprintf("Trying to load map: %s", imagePath));
		TileSet* pTileSet = new TileSet(imageWidth, firstGid, imageHeight, tilesetName, tilesetTileWidth, tilesetTileHeight, imagePath.c_str());
		pTiledManager->InsertTileSet(pTileSet);

		//Check if there are any animations.
		tinyxml2::XMLElement* pTile = pTileSetElement->FirstChildElement("tile");
		while (pTile)
		{
			//Get the animation id
			int animationId = 0;
			pTile->QueryIntAttribute("id", &animationId);

			//Get the name of the animation
			const char* animationName = nullptr;
			tinyxml2::XMLElement* pProperty = pTile->FirstChildElement("properties");
			if (pProperty)
			{
				pProperty = pProperty->FirstChildElement("property");
				if (pProperty)
				{
					animationName = pProperty->Attribute("name");
				}
			}
			if (!animationName)
			{
				char animationStr[10];
				memset(animationStr, 0, 10);
				_itoa_s(animationId, animationStr, 10);
				animationName = animationStr;
			}
			//Get the animation frames and duration // on pAnimation / tileid duration
			tinyxml2::XMLElement* pFrameElement = pTile->FirstChildElement("animation");
			if (pFrameElement)
			{
				pFrameElement = pFrameElement->FirstChildElement("frame");
			}
			std::vector<std::pair<int, int>> frames;
			
			while (pFrameElement)
			{
				int frameId = 0;
				int frameDuration = 0;
				pFrameElement->QueryIntAttribute("tileid", &frameId);
				pFrameElement->QueryIntAttribute("duration", &frameDuration);
				frames.push_back(std::make_pair(frameId, frameDuration));
				pFrameElement = pFrameElement->NextSiblingElement("frame");
			}

			pTileSet->CreateAnimation(animationName, frames);
			//ChecK if there are any other animations
			pTile = pTile->NextSiblingElement("tile");
		}
		//push the animation to the tileset

		pTileSetElement = pTileSetElement->NextSiblingElement("tileset");
	}

	//Load all the layers

	//Get the first layer element
	int layerIndex = 0;
	static_cast<SdlRenderDevice*>(g_pRenderDevice)->SetRenderLayers(layerIndex + 1);
	tinyxml2::XMLElement* pLayerElement = pElement->FirstChildElement("layer");

	while (pLayerElement != nullptr)
	{
		const char* layerName = nullptr;
		int layerWidth = 0;
		int layerHeight = 0;


		layerName = pLayerElement->Attribute("name");
		pLayerElement->QueryIntAttribute("width", &layerWidth);
		pLayerElement->QueryIntAttribute("height", &layerHeight);

		//load in all the tiles(data)
		tinyxml2::XMLElement* pData = pLayerElement->FirstChildElement("data");
		if(pData)
		{
			int tileX = 0; //x position of the tile
			int tileY = 0; //y position of the tile
			int lastIndex = 1; //last index of an int. used to parse through 2d array

			//This is the entire list of the tiles. we need to parse it
			std::string myMap = pData->FirstChild()->Value();

			//This is where the next tile begins.
			int currIndex = (int)myMap.find(",", lastIndex);
			//Tiles are saved in an array.
			int* tileGids = new int[layerWidth * layerHeight]();
			int currentIndex = 0;
			std::string temp;
			while (lastIndex <= currIndex)
			{
				while (lastIndex < currIndex)
				{
					temp += (char)myMap[lastIndex];
					++lastIndex;
				}
				++lastIndex;
				currIndex = (int)myMap.find(",", lastIndex);
				tileGids[currentIndex] = std::stoi(temp);
				++currentIndex;

				temp.clear();
			}
			currIndex = (int)myMap.find("\n", lastIndex);
			while (lastIndex < currIndex)
			{
				temp += (char)myMap[lastIndex];
				++lastIndex;
			}
			tileGids[currentIndex] = std::stoi(temp);

			//Now we have all the tileGids, lets create actual tiles for them
			for (int i = 0; i < layerWidth * layerHeight; ++i)
			{
				//the tile is empty, lets skip it.
				if (tileGids[i] == 0)
				{
					continue;
				}

				tileX = (i % layerWidth) * tileWidth;
				tileY = (i / layerWidth) * tileHeigh;
				tileX += tileWidth/2;
				tileY += tileHeigh/2;

				Vector3<float> objectPos;
				objectPos.SetXYZ((float)tileX, (float)tileY, (float)layerIndex);

				Vector3<float> objectRotation;
				objectRotation.SetAll(0);

				Vector3<float> objectScale;
				objectScale.SetXYZ((float)tileWidth, (float)tileHeigh, 0.f);

				GameObjectManager::CreateObject("Tile", objectPos, objectRotation, objectScale, tileGids[i]);
			}

			delete tileGids;

		}

		++layerIndex;
		static_cast<SdlRenderDevice*>(g_pRenderDevice)->SetRenderLayers(layerIndex + 1);
		//keep looping the layer elements
		pLayerElement = pLayerElement->NextSiblingElement("layer");
	}

	//read the object groups 
	tinyxml2::XMLElement* pObjectGroup = pElement->FirstChildElement("objectgroup");

	while(pObjectGroup)
	{
		tinyxml2::XMLElement* pObject = pObjectGroup->FirstChildElement("object");
		while (pObject)
		{
			const char* objectName = nullptr;
			int objectGid = -1;
			float objectPosX = 0;
			float objectPosY = 0;
			float objectWidth = 0;
			float objectHeight = 0;
			float objectRotation = 0;

			objectName = pObject->Attribute("name");
			pObject->QueryIntAttribute("gid", &objectGid);
			pObject->QueryFloatAttribute("x", &objectPosX);
			pObject->QueryFloatAttribute("y", &objectPosY);
			pObject->QueryFloatAttribute("width", &objectWidth);
			pObject->QueryFloatAttribute("height", &objectHeight);
			pObject->QueryFloatAttribute("rotation", &objectRotation);

			//We can determine if the Gameobject has a Texture on it or not by checking the GID.
				//The GID is -1 at the initiation. If after reading the object gid is anything other than -1 then the object has a texture
				//If the object has a texture we need to offset the position the object down by half of its height.
				//If the object doesnt have a texture we need to offset the position the object up by half if its height.
				//In both cases we need to offset the position the object left by half of its width.

			

			//position of the object
			//Set up the position of the object
			Vector3<float> objectPos;
			Vector3<float> colliderPos;
			objectPos.SetXYZ((float)objectPosX, (float)objectPosY, (float)layerIndex);

			//The object has a texture, lets offset it down by half of its height
			if (objectGid >= 0)
			{
				objectPos.SetY(objectPosY - objectHeight/2);
			//	colliderPos.SetY(objectPosY - objectHeight);
			}
			//the object doesnt have the texture so lets offset it up by half of its height
			else
			{
				objectPos.SetY(objectPosY + objectHeight / 2);
			}
				objectPos.SetX(objectPosX + (objectWidth / 2));
				//colliderPos.SetX(objectPosX - objectWidth);
				//Offset width left by half of the width

				//objectPos.SetY(objectPosY - objectHeight / 2);

			Vector3<float> vecRot;
			vecRot.SetXYZ(objectRotation, 0, 0);

			Vector3<float> objectScale;
			objectScale.SetXYZ(objectWidth, objectHeight, 0);

			GameObject* pNewObject = GameObjectManager::CreateObject(objectName, objectPos, vecRot, objectScale, objectGid);

			//Each gameobject will have its own collision
			ColliderComponent* pCollision = new ColliderComponent(pNewObject, objectPos.GetX(), objectPos.GetY(), objectWidth, objectHeight);
			pNewObject->AddComponent(pCollision);

			//Check if there are any components to add to the object.
			tinyxml2::XMLElement* pProperties = pObject->FirstChildElement("properties");
			if (pProperties)
			{
				pProperties = pProperties->FirstChildElement("property");
				while (pProperties)
				{
					const char* propertyType = nullptr;
					const char* propertyName = nullptr;

					propertyType = pProperties->Attribute("value");
					propertyName = pProperties->Attribute("name");

					if (std::string(propertyType) == "Component")
					{
						//we have a component that we have to add to the component list
						std::string componentName(propertyName);
						
						componentName.append("Component");

						pNewObject->AddComponent(componentName.c_str());
					}
					else if (std::string(propertyType) == "Script")
					{
						LuaScriptComponent* pLua = pNewObject->GetComponent<LuaScriptComponent>();
						if (!pLua)
						{
							pLua = new LuaScriptComponent(pNewObject);
							pNewObject->AddComponent(pLua);
						}
						std::string scriptName("Media/Scripts/");
						scriptName.append(propertyName);
						scriptName.append(".lua");
						pLua->Init(scriptName);
					}
					pProperties = pProperties->NextSiblingElement("property");
				}
			}

			pObject = pObject->NextSiblingElement("object");
		}
		pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		//Load all the collision objects
	}
}