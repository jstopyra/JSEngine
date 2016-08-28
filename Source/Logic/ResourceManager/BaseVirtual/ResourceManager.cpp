//ResourceManager.cpp

#include "ResourceManager.h"
#include "Resource.h"
#include "../Xml/XmlReader.h"
#include "../../GameObject/GameObjectManager.h"
#include "../Sdl/SdlResourceManager.h"
#include <vector>

Resource* ResourceManager::LoadResource(const char* fileName)
{
	FileExtention currentFileExt = CheckFileExtention(fileName);
	if (currentFileExt == FileExtention::k_xml)
	{
		//use the xmlLoader
	}
	else if (currentFileExt == FileExtention::k_png)
	{
		//use the png loader
		return SdlResourceManager::LoadSdlTexture(fileName);
	}
	return nullptr;
}
#include <SDL.h>
Resource* ResourceManager::GetResource(const char* fileName)
{
	//Check if the resource exists:
	std::string fileNameStr(fileName);
	auto resource = m_resourceMap.find(fileNameStr);
	if(resource != m_resourceMap.end())
	{
		//we found it
		SDL_Log("Returning texture: %s", fileName);
		return resource->second;
	}

	//Resource doesnt exist, lets load it.
	std::string name(fileName);
	//const char* name = string.c_str();

	Resource* pNewResource = LoadResource(name.c_str());

	//Add the resource to the Map of Resources
	m_resourceMap[name] = pNewResource;
	return pNewResource;
}


ResourceManager::~ResourceManager()
{
	for (auto it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it)
	{
		delete it->second;
	}
	m_resourceMap.clear();
}






