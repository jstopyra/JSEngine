//TileSet.h

#ifndef __TILESET_H__
#define __TILESET_H__

#include "../BaseVirtual/ResourceManager.h"
#include "../../../Rendering/Texture.h"
#include <utility>
#include <unordered_map>
#include <vector>
#include <string>

typedef std::vector<std::pair<int, int>> FrameIdDuration;
extern ResourceManager g_resourceManager;

class TileSet
{

	//--------------------------------------------------------------------------------------------
	//private member variables
	//--------------------------------------------------------------------------------------------
	 int m_firstGid;
	 int m_lastGid;
	 const char* m_name;
	 int m_tileWidth;
	 const char* m_source;
	 int m_tileHeight;
	 int m_imageWidth;
	 int m_imageHeight;
	 int m_tileAmountWidth;
	 Texture* m_pTexture;
	 //Animation stuff
	 std::unordered_map<std::string, FrameIdDuration> animationMap;
	//--------------------------------------------------------------------------------------------
	//END private member variables
	//--------------------------------------------------------------------------------------------



public:
	TileSet(
		int imageWidth
		, int firstGid
		, int imageHeight
		, const char* tilesetName
		, int tilesetTileWidth
		, int tilesetTileHeight
		, const char* tilesetImagePath
		)
		: m_firstGid(firstGid)
		, m_name(tilesetName)
		, m_tileWidth(tilesetTileWidth)
		, m_source(tilesetImagePath)
		, m_tileHeight(tilesetTileHeight)
		, m_imageWidth(imageWidth)
		, m_imageHeight(imageHeight)
	{
		
		m_tileAmountWidth = imageWidth / tilesetTileWidth;
		m_lastGid = m_tileAmountWidth * (imageHeight / tilesetTileHeight) + firstGid - 1;
		m_pTexture = static_cast<Texture*>(g_resourceManager.GetResource(tilesetImagePath));
		
	};

	const int GetGID() const { return m_firstGid; }
	const char* GetTexturePath() const { return m_name; }
	const int GetWidth() const { return m_imageWidth / m_tileWidth; }
	Texture* GetTexture() const { return m_pTexture; }
	const bool ContainsGid(int gid) { return (gid >= m_firstGid && gid <= m_lastGid);}

	//Check if we have animation with a given name
	//Return the animation if we have it.
	//Return nullptr if the animation doesnt exist
	FrameIdDuration* GetAnimationByName(std::string animationName)
	{
		auto findIt = animationMap.find(animationName);
		if (findIt != animationMap.end())
		{
			return (&findIt->second);
		}
		return nullptr;
	}
	void CreateAnimation(const char* animationName, FrameIdDuration& frames)
	{
		//set the animation name
		std::string newString = "";
		if (animationName)
			newString = animationName;

		//add the animation name and the frames that come with it into a map of animations for that tileset
		animationMap.emplace(std::make_pair(newString, frames));
	}


	FrameIdDuration* PlayAnimationWithGid(int gid)
	{
		auto findIt = animationMap.begin();
		while (findIt != animationMap.end())
		{
			if (findIt->second[0].first == gid)
			{
				return (&findIt->second);
			}
			++findIt;
		}
		return nullptr;
	}

	bool HasAnimations() {return !animationMap.empty(); }

};
#endif // !__TILESET_H__
