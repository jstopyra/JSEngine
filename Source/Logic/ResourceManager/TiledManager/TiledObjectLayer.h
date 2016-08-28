//TiledObjectLayer.h

#ifndef __TILEDOBJECTLAYER_H__
#define __TILEDOBJECTLAYER_H__

#include <vector>

typedef std::vector<int> TileVector;

class TiledObjectLayer
{
	TileVector m_tiles;



public:
	TiledObjectLayer(TileVector tiles);
private:

};

TiledObjectLayer::TiledObjectLayer(TileVector tiles)
{
	m_tiles = tiles;
}


#endif // !__TILEDOBJECTLAYER_H__
