//SdlLayer.cpp
#include "SdlRenderDevice.h"
#include "../../Application/Application.h"
#include <Sdl.h>
#include <SDL_image.h>
#include <string>

#include "../../Utilities/EventSystem/Events/EventsInclude.h"

#include "../../Logic/Components/ComponentIncludes.h"
#include "../../Utilities/Vector3.h"
#include "../SDL/SdlTexture.h"
#include "../Sprite.h"
#include "../../Logic/GameObject/GameObject.h"
#include "../../Logic/ResourceManager/TiledManager/TileSet.h"


#include "SDL_rect.h"

extern EventSystem* g_pEventSystem;
SDL_Renderer* g_pRenderer;
struct SDL_Texture;

GameObject* g_pCamera;

SdlRenderDevice::SdlRenderDevice()
	:m_windowWidth(600)
	, m_windowHeight(480)
{
	std::vector<std::weak_ptr<Sprite>>;
	std::vector<std::vector<std::weak_ptr<Sprite>>>;
	
	//Create the camera
	g_pCamera = new GameObject();
	CameraComponent* pCameraComp = new CameraComponent(m_windowWidth, m_windowHeight, g_pCamera);
	g_pCamera->AddComponent(pCameraComp);
}

SdlRenderDevice::~SdlRenderDevice()
{
	SDL_DestroyRenderer(g_pRenderer);
	g_pRenderer = nullptr;
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

	delete g_pCamera;
	for (auto vectorSprite : m_pTextureList)
	{	
		for (auto sprite : vectorSprite)
		{
			sprite.reset();
		}
	}

	SDL_Quit();
}

bool SdlRenderDevice::Init()
{

	//return if the SDL fails to initialize video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return false;
	}
	SDL_Log("Video Initialized Correctly");

	//creates sdl window
	CreateNewWindow("JSEngine", m_windowWidth, m_windowHeight);
	g_pEventSystem->AttachListener(k_updateEvent, this);

	return true;
}

void SdlRenderDevice::SetRenderLayers(int renderLayers)
{
	m_maxLayers = renderLayers;
	m_pTextureList.reserve(m_maxLayers);

	std::vector<std::weak_ptr<Sprite>> layer1;
	for (int i = (int)m_pTextureList.size(); i < m_maxLayers; ++i)
		m_pTextureList.push_back(layer1);
}

bool SdlRenderDevice::Update()
{
	//clear the renderer
	SDL_RenderClear(g_pRenderer);
	PrepareTextures();
	SDL_RenderPresent(g_pRenderer);
	return true;
}

bool SdlRenderDevice::CreateNewWindow(const char* windowName, int windowWidth, int windowHeight)
{
	m_pWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (m_pWindow == nullptr)
	{
		SDL_Log("Failed to create the window %s:", SDL_GetError());
		return false;
	}
	g_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (g_pRenderer == nullptr)
	{
		SDL_Log("Failed to create the renderer %s:", SDL_GetError());
		return false;
	}
	return true;
}


const int SdlRenderDevice::GetWindowWidth()const
{
	return m_windowWidth;
}
const int SdlRenderDevice::GetWindowHeight()const
{
	return m_windowHeight;
}

void SdlRenderDevice::PrepareTextures()
{
	CameraComponent* pCameraComponent = g_pCamera->GetComponent<CameraComponent>();

	int currentLayer = 0;
	while (currentLayer < m_maxLayers)
	{	
		auto it = m_pTextureList[currentLayer].begin();
		while(it != m_pTextureList[currentLayer].end())
		{
			//Check if the texture still exists
			if (!it->expired())
			{
				auto texture = it->lock();
				++it;

				TransformComponent* pTextureTransformComponent = texture->GetOwner()->GetComponent<TransformComponent>();

				Vector3<float>* texturePos = pTextureTransformComponent->GetPosition();
				Vector3<float>* textureSize = pTextureTransformComponent->GetScale();
				Vector3<float>* textureRotation = pTextureTransformComponent->GetRotation();

				//Check if the texture is in view
				int cameraPosX = (int)pCameraComponent->GetPosition()->GetX();
				int cameraPosY = (int)pCameraComponent->GetPosition()->GetY();
				SDL_Rect cameraView;
				cameraView.x = cameraPosX - (pCameraComponent->GetWidth() / 2);
				cameraView.y = cameraPosY - (pCameraComponent->GetHeight() / 2);
				cameraView.w = pCameraComponent->GetWidth();
				cameraView.h = pCameraComponent->GetHeight();


				//Create the destination rect
				SDL_Rect pDestRect;
				pDestRect.x = (int)texturePos->GetX() - (int)textureSize ->GetX()/2;
				pDestRect.y = (int)texturePos->GetY() - (int)textureSize->GetY()/2;
				pDestRect.w = (int)textureSize->GetX();
				pDestRect.h = (int)textureSize->GetY();


				//Check for the collision
				if (SDL_HasIntersection(&pDestRect, &cameraView) == SDL_FALSE)
				{
					continue;
				}
				//offset the textiles
				pDestRect.x -= cameraView.x;
				pDestRect.y -= cameraView.y;
				//Draw a rect around the collision box of the object
				ColliderComponent* pCollider = texture->GetOwner()->GetComponent<ColliderComponent>();
				if (pCollider)
				{
					SDL_Rect colliderRect;
					pCollider->GetCollisionRegion(colliderRect.x, colliderRect.y, colliderRect.w, colliderRect.h);
					colliderRect.x -= cameraView.x;
					colliderRect.y -= cameraView.y;
#if _DEBUG

					SDL_SetRenderDrawColor(g_pRenderer, 255, 0, 0, 0);
					SDL_RenderDrawRect(g_pRenderer, &colliderRect);
					SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 0);
#endif //_DEBUG
				}
				

				TileSet* pTileSet = texture->GetTileSet();
				if (!pTileSet)
				{
					continue;
				}
				//Create the source rect
				SDL_Rect pSourceRect;
				int position = texture->GetGid() - pTileSet->GetGID();
				position %= texture->GetTileSet()->GetWidth();
				position *= (int)textureSize->GetX();
				pSourceRect.x = position;

				position = texture->GetGid() - pTileSet->GetGID();
				position /= texture->GetTileSet()->GetWidth();
				position *= (int)textureSize->GetY();
				pSourceRect.y = position;
				pSourceRect.w = (int)textureSize->GetX();
				pSourceRect.h = (int)textureSize->GetY();
			
				SdlTexture* pTexture = (SdlTexture*)texture.get()->GetTexture();
				SDL_Texture* pSdlTexture = (SDL_Texture*)pTexture->GetTexture();

#if _DEBUG
				//Draw a rect around the object
				SDL_RenderDrawRect(g_pRenderer, &pDestRect);

				//Draw the point in the middle of the object
				SDL_SetRenderDrawColor(g_pRenderer, 0, 255, 0, 0);
				SDL_RenderDrawPoint(g_pRenderer, pDestRect.x, pDestRect.y);
				SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 0);
#endif//_DEBUG
				SDL_RenderCopyEx(g_pRenderer, pSdlTexture, &pSourceRect, &pDestRect, textureRotation->GetX(), NULL, SDL_FLIP_NONE);
			}
			else
			{
				it = m_pTextureList[currentLayer].erase(it);
			}
		}
		++currentLayer;
	}
}

void SdlRenderDevice::CloseWindow()
{

}


void SdlRenderDevice::RegisterSprite(std::shared_ptr<Sprite> pSprite)
{
	//Register a texture into a map of textures that will be rendered every frame
	int posZ = (int)pSprite->GetOwner()->GetComponent<TransformComponent>()->GetPosition()->GetZ();
	
	auto my_ptr = std::weak_ptr<Sprite>(pSprite);
	m_pTextureList[posZ].push_back(my_ptr);
}

void SdlRenderDevice::OnEvent(const Event* pEvent)
{
	if (pEvent->GetEventId() == k_updateEvent)
	{
		Update();
	}
}
