#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <string>
#include <utility>
#include <vector>
#include "../Utilities/EventSystem/Events/EventsInclude.h"
#include "../Utilities/EventSystem/EventListener.h"
//frame gid/ frame duration
typedef std::vector<std::pair<int, int>> FrameIdDuration;

class Texture;
class RenderComponent;
class TileSet;
extern EventSystem* g_pEventSystem;

class GameObject;
class Sprite;

struct Rect
{
	//X and Y position of the Sprite
	int m_x, m_y;
	//Width and Height of the Sprite
	int m_w, m_h;
};

class SpriteAnimation : public EventListener
{
	//is the sprite animated?
	bool m_animated;
	int m_timeLeftForNextFrame;
	//current animation frame
	int m_currFrame;

	//This is our currently playing animation
	int currentAnimation;

	Sprite* m_pOwner;
	FrameIdDuration* pCurrentAnimation;
	void OnUpdate(float deltaTime);

public:
	//Set the current animation
	SpriteAnimation(Sprite* pOwner) :pCurrentAnimation(nullptr), m_pOwner(pOwner) 
	{
		g_pEventSystem->AttachListener(k_updateEvent, this);
	};
	~SpriteAnimation() {
		m_pOwner = nullptr; 
		pCurrentAnimation = nullptr;
	}
	void ResetAnimation(TileSet* pTileSet);
	void SwitchAnimation(const std::string& animationName, TileSet* pTileSet);
	virtual void OnEvent(const Event* pEvent);
	//Play an animation that has a specific gid
	void PlayAnimationWithGid(int gid, TileSet* pTileSet);

};

class Sprite
{

//--------------------------------------------------------------------------------------------
//private member variables
//--------------------------------------------------------------------------------------------

	//Flyweight texture shared amongst many sprites
	Texture* m_pTexture;
	//Rect of that specific sprite x, y, w, h
	Rect* m_pRect;
	//RenderComponent that owns the sprite
	GameObject* m_pOwner;

	//tile set that contains our Texture
	TileSet* m_pTileSet;

	//grid id on the tile set
	int m_gid;

	//Our animation settings
	SpriteAnimation* m_pAnimation;

//--------------------------------------------------------------------------------------------
//END private member variables
//--------------------------------------------------------------------------------------------

public:
	Sprite(GameObject* pOwner, int gid);
	~Sprite();

//--------------------------------------------------------------------------------------------
//public Getters/Setters
//--------------------------------------------------------------------------------------------
	
	//Get texture component thats chared amongst all the objects with the same texture
	const Texture* GetTexture() const;

	//Return the path of the texture used for this sprite
	const char* GetTexturePath() const;

	//Get the Rect of the sprite, x, y, w, h
	const Rect* GetRect() const;
	void GetRect(int& x, int& y, int& w, int& h);

	//return the render component owning this sprite
	GameObject* GetOwner() const;

	//Set the texture thats shared amongst the objects
	const void SetTexture(Texture* pTexture);

	const int GetGid() const { return m_gid; }
	TileSet* GetTileSet() const { return m_pTileSet; }

//--------------------------------------------------------------------------------------------
//END public Getters/Setters
//--------------------------------------------------------------------------------------------
	void ResetAnimation();

	void SetAnimation(const std::string& animationName);
	void SetGid(const int gid);

private:

};
#endif // !__SPRITE_H__
