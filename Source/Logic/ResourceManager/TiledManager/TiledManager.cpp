#include "TiledManager.h"
#include "TileSet.h" 

TiledManager::TiledManager()
{
}

TiledManager::~TiledManager()
{
	for (auto tileSet : m_tileSets)
	{
		delete tileSet;
		tileSet = nullptr;
	}
	m_tileSets.clear();
}

void TiledManager::LoadMap(const char* mapName)
{
	XmlReader::LoadTiledMap(this, mapName);
}

void TiledManager::InsertTileSet(TileSet* pTileSet)
{
	m_tileSets.push_back(pTileSet);
}

TileSet* TiledManager::GetTileSetWithGid(int gid)
{
	//Find the correct tileset
	for (auto tileIt : m_tileSets)
	{
		if (tileIt->ContainsGid(gid))
		{
			return tileIt;
		}
	}
	return nullptr;
}
