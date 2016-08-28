//RenderComponent.cpp

#include "RenderComponent.h"

#include "../../Rendering/Sprite.h"
#include "../../Rendering/SDL/SdlRenderDevice.h"
#include "../GameObject/GameObject.h"

extern RenderDevice* g_pRenderDevice;
RenderComponent::RenderComponent(GameObject* pParent)
	: Component(pParent)
{

}

RenderComponent::~RenderComponent()
{
	for (auto sprite : m_pSprite)
	{
		//null out the shared pointers
		sprite = nullptr;
	}
	m_pSprite.clear();
}

void RenderComponent::Init(int gid, GameObject* pOwner)
{
	//Create a sprite for itself
	std::shared_ptr<Sprite> newSpritePointer = std::shared_ptr<Sprite>(new Sprite(pOwner, gid));
	m_pSprite.push_back(newSpritePointer);
	//Get the global render device
	SdlRenderDevice* renderDevice = static_cast<SdlRenderDevice*>(g_pRenderDevice);
	//register the sprite so it can be rendered onto the screen
	renderDevice->RegisterSprite(newSpritePointer);
}

const char* RenderComponent::GetTexturePath() const
{
	return m_pSprite[0]->GetTexturePath();
}

void RenderComponent::SetAnimation(const std::string& animationName)
{
	//at the moment only the very first sprite will get its animation changed
	m_pSprite[0]->SetAnimation(animationName);
}


void RenderComponent::ResetAnimations()
{
	m_pSprite[0]->ResetAnimation();
}

void RenderComponent::GetSpriteRect(int& x, int& y, int& w, int& h)
{
	m_pSprite[0]->GetRect(x, y, w, h);
}