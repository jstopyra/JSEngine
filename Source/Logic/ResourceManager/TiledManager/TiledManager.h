//TiledManager.h

#ifndef __TILEDMANAGER_H__
#define __TILEDMANAGER_H__

#include "../Xml/xmlreader.h"
#include <vector>

class TileSet;

class TiledManager
{
	std::vector<TileSet*> m_tileSets;


public:
	TiledManager();
	~TiledManager();
	void InsertTileSet(TileSet* pTileSet);

	TileSet* GetTileSetWithGid(int gid);
	void TiledManager::Init() {};
	void LoadMap(const char* mapName);

private:

};

#endif // !__TILEDMANAGER_H__
