#include "SdlTexture.h"
#include <SDL.h>
#include <SDL_image.h>
#include "../../Logic/ResourceManager/BaseVirtual/ResourceManager.h"

extern SDL_Renderer* g_pRenderer;

SdlTexture::SdlTexture(const char* pFileName)
	:m_pRect(nullptr)
	, m_pMyTexture(nullptr)
	,m_pRenderer(g_pRenderer)
	,Texture(pFileName)
{
	
}

SdlTexture::~SdlTexture()
{
	delete m_pRect;
}

Texture * SdlTexture::GetTexture() const
{
	return (Texture*)m_pMyTexture;
}

const void SdlTexture::SetTexture(SDL_Texture* pTexture)
{
	m_pMyTexture = pTexture;
}


const void SdlTexture::SetRect(SDL_Rect* pRect)
{
	m_pRect = pRect;
}