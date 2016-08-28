//SdlLayer.h
#ifndef __SDL_H__
#define __SDL_H__

#include "../../Utilities/EventSystem/EventListener.h"
#include "../RenderDevice.h"
#include <vector>
#include <memory>

struct SDL_Window;
struct SDL_Renderer;
union SDL_Event;
class GameObject;
class Sprite;

class SdlRenderDevice : public RenderDevice, public EventListener
{

public:
	SdlRenderDevice();
	virtual ~SdlRenderDevice();

	bool CreateNewWindow(const char* windowName, int windowWidth, int windowHeight) override;
	void CloseWindow() override;

	void RegisterSprite(std::shared_ptr<Sprite> pSprite);
	void SetRenderLayers(int renderLayers);
	virtual void OnEvent(const Event* pEvent) override;

	virtual const int GetWindowWidth() const override;
	virtual const int GetWindowHeight()const override;
private:
	void PrepareTextures();

	GameObject* pCamera;

	std::vector<std::vector<std::weak_ptr<Sprite>>> m_pTextureList;
	int m_maxLayers;
	bool Update();
	bool Init() override;
	SDL_Window* m_pWindow;

	int m_windowWidth, m_windowHeight;



};

#endif