#include "SdlResourceManager.h"
#include <SDL_image.h>
#include "../../../Rendering/SDL/SdlTexture.h"
extern SDL_Renderer* g_pRenderer;

SdlResourceManager::SdlResourceManager()
{
}

SdlResourceManager::~SdlResourceManager()
{
}


Resource* SdlResourceManager::LoadSdlTexture(const char* pFileName)
{
	//Code from prev project
	SdlTexture* pReturnTexture = new SdlTexture(pFileName);
	SDL_Log("Preparing to load the texture: %s", pFileName);

	SDL_RWops* pRwop = SDL_RWFromFile(pFileName, "rb");
	SDL_Surface* m_surface = IMG_LoadPNG_RW(pRwop);
	SDL_Texture* pSdlNativeTexture;
	SDL_Rect* pRect = new SDL_Rect();
	if (m_surface == nullptr)
	{
		SDL_Log("Failed to load the surface %s", SDL_GetError());
		return false;
	}
	pSdlNativeTexture = SDL_CreateTextureFromSurface(g_pRenderer, m_surface);

	if (pSdlNativeTexture == nullptr)
	{
		SDL_Log("Failed to load the texture %s", SDL_GetError());
		return false;
	}
	SDL_Log("Loaded texture: %s", pFileName);
	pRect->x = 0;
	pRect->y = 0;
	pRect->w = m_surface->w;
	pRect->h = m_surface->h;
	pReturnTexture->SetRect(pRect);
	SDL_FreeSurface(m_surface);
	pReturnTexture->SetTexture(pSdlNativeTexture);

	return pReturnTexture;
}
