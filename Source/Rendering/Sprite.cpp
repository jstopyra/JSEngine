//Sprite.cpp

#include "Sprite.h"
#include "../Logic/ResourceManager/BaseVirtual/ResourceManager.h"
#include "Texture.h"
#include "../Utilities/DebugLog.h"
#include "../Logic/ResourceManager/TiledManager/TileSet.h"
#include <assert.h>
#include "../Logic/Components/PlayerControllerComponent.h"
#include "../Logic/Components/RenderComponent.h"
#include "../Logic/GameObject/GameObject.h"
#include "../Logic/GameObject/GameObjectManager.h"
#include "../Logic/ResourceManager/TiledManager/TiledManager.h"
#include "../Logic/Components/MovementComponent.h"

extern GameObjectManager* g_pGameObjectManager;

Sprite::Sprite(GameObject* pOwner, int gid)
	: m_pTexture(nullptr)
	, m_pRect(nullptr)
	, m_pOwner(pOwner)
	, m_gid(gid)
	, m_pTileSet(nullptr)
	, m_pAnimation(nullptr)
{
	//Ask the resource manager for the texture of a given name.
	//Get the tileset for the specified gid
	TileSet* pTileSet = g_pGameObjectManager->GetTiledManager()->GetTileSetWithGid(gid);
	if (pTileSet)
	{
		m_pTileSet = pTileSet;
		m_pTexture = pTileSet->GetTexture();
		if (pTileSet->HasAnimations())
		{
			m_pAnimation = new SpriteAnimation(this);
			int animationGid = gid - m_pTileSet->GetGID();
			m_pAnimation->PlayAnimationWithGid(animationGid, m_pTileSet);
		}
	}
}

Sprite::~Sprite()
{
	delete m_pRect;
	delete m_pAnimation;
}


//--------------------------------------------------------------------------------------------
//Getters/Setters
//--------------------------------------------------------------------------------------------
const Texture* Sprite::GetTexture() const
{
	return m_pTexture;
}

const char* Sprite::GetTexturePath() const
{
	if (m_pTexture)
		return m_pTexture->GetTexturePath();
	DEBUGLOG("No texture has been found on the sprite");
	return nullptr;
}

const Rect* Sprite::GetRect() const
{
	return m_pRect;
}

void Sprite::GetRect(int& x, int& y, int& w, int& h)
{
	x = m_pRect->m_x;
	y = m_pRect->m_y;
	w = m_pRect->m_w;
	h = m_pRect->m_h;
}

GameObject* Sprite::GetOwner() const
{
	return m_pOwner;
}


const void Sprite::SetTexture(Texture* pTexture)
{
	m_pTexture = pTexture;
}

//--------------------------------------------------------------------------------------------
//END Getters/Setters
//--------------------------------------------------------------------------------------------

void Sprite::SetAnimation(const std::string& animationName)
{
	if (m_pAnimation)
		m_pAnimation->SwitchAnimation(animationName, m_pTileSet);
}

void Sprite::SetGid(const int gid)
{

	m_gid = gid + m_pTileSet->GetGID();
}

void Sprite::ResetAnimation()
{
	if (m_pAnimation)
		m_pAnimation->ResetAnimation(m_pTileSet);
}



//--------------------------------------------------------------------------------------------
// SPRITE_ANIMATION
//--------------------------------------------------------------------------------------------
void SpriteAnimation::PlayAnimationWithGid(int gid, TileSet* pTileSet)
{
	if (pTileSet->HasAnimations())
	{
		auto animation = pTileSet->PlayAnimationWithGid(gid);
		pCurrentAnimation = animation;
	}
}
void SpriteAnimation::SwitchAnimation(const std::string& animationName, TileSet* pTileSet)
{
	//Check if the passed animation name exists in the list of animations that our exist on our TileSet
	if (pTileSet->HasAnimations())
	{
		auto animation = pTileSet->GetAnimationByName(animationName);
		pCurrentAnimation = animation;
	}
}

void SpriteAnimation::ResetAnimation(TileSet* pTileSet)
{
	Vector3<float>* velocity = m_pOwner->GetOwner()->GetComponent<MovementComponent>()->GetVelocity();
	if (velocity->GetX() > 0)
	{
		SwitchAnimation("MoveRight", pTileSet);
	}
	else if (velocity->GetX() < 0)
	{
		SwitchAnimation("MoveLeft", pTileSet);
	}
	else if (velocity->GetY() < 0)
	{
		SwitchAnimation("MoveUp", pTileSet);
	}
	else
	{
		SwitchAnimation("MoveDown", pTileSet);
	}
}


void SpriteAnimation::OnEvent(const Event* pEvent)
{
	if (pEvent->GetEventId() == k_updateEvent)
	{
		UpdateEvent* pUpdateEvent = (UpdateEvent*)(pEvent);
		OnUpdate(pUpdateEvent->GetDeltaTime());
	}
}

void SpriteAnimation::OnUpdate(float deltaTime)
{
	//Change the time left untill we have to switch the animation
	m_timeLeftForNextFrame -= (int)(deltaTime * 1000);

	//TOOD: This was player specific, I need to move that somewhere else but keeping the code for reference.
	//MovementComponent* pMovementComponent = m_pOwner->GetOwner()->GetComponent<MovementComponent>();
	//if (pMovementComponent)
	//{
		//Check if th eplayer is moving, if so then switch the animation
		//Vector3<float>* playerVelocity = pMovementComponent->GetVelocity();
		//if (playerVelocity->GetX() != 0
		//	|| playerVelocity->GetY() != 0
		//	|| playerVelocity->GetZ() != 0)
		//{
		//	m_timeLeftForNextFrame -= (int)(deltaTime * 1000);
		//}
		//else
		//{
		//	if(pCurrentAnimation)
		//		m_pOwner->SetGid(pCurrentAnimation->at(0).first);
		//}
	//}

	//We have no time left, lets switch the frame
	if (m_timeLeftForNextFrame <= 0)
	{
		++m_currFrame;
		if (pCurrentAnimation)
		{
			//reset the animation loop
			if (m_currFrame >= (int)pCurrentAnimation->size())
			{
				m_currFrame = 0;
			}
			//set the time remaining for next switch of animation frame
			m_timeLeftForNextFrame = pCurrentAnimation->at(m_currFrame).second;
			//Set the animation tile
			m_pOwner->SetGid(pCurrentAnimation->at(m_currFrame).first);

		}
	}
}


//--------------------------------------------------------------------------------------------
// END_SPRITE_ANIMATION
//--------------------------------------------------------------------------------------------
