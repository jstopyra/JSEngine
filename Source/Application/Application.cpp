//Application.cpp
#if _DEBUG
#include <vld.h>
#endif //_DEBUG
#include "Application.h"
#include "../Utilities/RandomGenerator.h"
#include "../Utilities/EventSystem/Events/EventsInclude.h"
#include "../Utilities/Macros.h" //safedelete

#include "../Logic/GameObject/GameObjectManager.h"
#include "../Rendering/RenderFactory.h"
#include "../Logic/ResourceManager/BaseVirtual/ResourceManager.h"
#include "../Logic/LuaPlus/LuaPlusSingleton.h"
Application* Application::m_pInstance = nullptr; //null out the instance of the game

//--------------------------------------------------------------------------------------
//Global Classes that are being managed by the application
//--------------------------------------------------------------------------------------
EventSystem* g_pEventSystem;
GameObjectManager* g_pGameObjectManager;
RenderDevice* g_pRenderDevice;
ResourceManager g_resourceManager;
//--------------------------------------------------------------------------------------
//END Global Classes that are being managed by the application
//--------------------------------------------------------------------------------------


Application::Application()
: m_lastTicks(0)

{
	GENERATE_NEW_SEED

	m_applicationState = AppState::k_opening;
	m_pLuaPlusSingleton = LuaPlusSingleton::GetInstance();
	assert(m_pLuaPlusSingleton);

	m_lastTicks = SDL_GetTicks();
}

void Application::Destroy()
{
	//running the destructor on the only instance of the game
	//will destroy everything but the game.
	GetInstance()->~Application();

	//now destroy the instance of the game
	SAFE_DELETE(m_pInstance);
}

Application* Application::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new Application();
	}

	assert(m_pInstance);
	return m_pInstance;
}

Application::~Application()
{
	//Delete all the global subsystems
	SAFE_DELETE( g_pGameObjectManager);
	SAFE_DELETE( g_pRenderDevice);
	SAFE_DELETE (g_pRenderDevice);
	SAFE_DELETE( g_pEventSystem);

	assert(m_pLuaPlusSingleton);
	m_pLuaPlusSingleton->GetInstance()->Destroy();
}

bool Application::InternalMainLoop()
{
	//Run the main loop of the entire game here
	//We need to run a loop here instead of the renderer and update the renderer.
	if (m_applicationState == AppState::k_running || m_applicationState == AppState::k_opening)
	{
		//current framerate
		unsigned int currentTicks = SDL_GetTicks();
		float deltaTime = (currentTicks - m_lastTicks) / 1000.f;
		m_lastTicks = currentTicks;
		//Send an event out that its a new Update
		g_pEventSystem->TriggerEvent(new UpdateEvent(deltaTime));

		//pull the event from the sdl
		SDL_Event event;
		SDL_PumpEvents();

		while (SDL_PollEvent(&event))
		{
			ReadEvent(event);
		}
		//keep the loop going
		return true;
	}
	//Clean up engine
	GetInstance()->Destroy();
	return false;
}

bool Application::MainLoop()
{
	return GetInstance()->InternalMainLoop();
}

void Application::StartEngine()
{
	Application::GetInstance()->Init();
}

bool Application::Init()
{
	//create all global systems that are used throught the engine
	g_pEventSystem = new EventSystem();
	assert(g_pEventSystem);

	//return if we created the render device correctly
	if (RenderFactory::CreateRenderLayer(g_pRenderDevice) == false)
		return false;

	g_pGameObjectManager = new GameObjectManager();
	assert(g_pGameObjectManager);
	g_pGameObjectManager->Init();
	//Set the running state of the game.
	m_applicationState = AppState::k_running;

	//attach ourselves to listen for the end of the game event
	g_pEventSystem->AttachListener(k_quitGameEvent, this);
	return true;
}

void Application::LoadMap(const char* mapName)
{
	GetInstance()->InternalLoadMap(mapName);
}

void Application::InternalLoadMap(const char* mapName)
{
	g_pGameObjectManager->LoadMap(mapName);
}

void Application::InternalRequestClose()
{
	m_applicationState = AppState::k_requestClose;
}

void Application::RequestClose()
{
	GetInstance()->InternalRequestClose();
}

void Application::OnEvent(const Event* pEvent)
{
	//check if the game has ended.
	if (pEvent->GetEventId() == k_quitGameEvent)
	{
		//Request the engine to close down
		RequestClose();
	}
}

void Application::ReadEvent(SDL_Event sdlEvent)
{

	if (sdlEvent.type == SDL_KEYDOWN)
	{
		if (sdlEvent.key.state == SDL_PRESSED)
			if (sdlEvent.key.repeat == 0)
				g_pEventSystem->TriggerEvent(new InputKeyEvent(sdlEvent.key.keysym.sym, true));
		//Quit
		if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
		{
			g_pEventSystem->TriggerEvent(new QuitGameEvent());
		}
	}
	else if (sdlEvent.type == SDL_KEYUP)
	{
		if (sdlEvent.key.state == SDL_RELEASED)
			if (sdlEvent.key.repeat == 0)
				g_pEventSystem->TriggerEvent(new InputKeyEvent(sdlEvent.key.keysym.sym, false));
	}
	else if (sdlEvent.type == SDL_WINDOWEVENT)
	{
		if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			g_pEventSystem->TriggerEvent(new QuitGameEvent());
		}
	}
	else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN)
	{
		if (sdlEvent.button.button == SDL_BUTTON_LEFT)
		{
			int screenX = sdlEvent.button.x - (g_pRenderDevice->GetWindowWidth()/2);
			int screenY = sdlEvent.button.y - (g_pRenderDevice->GetWindowHeight()/2);
			g_pEventSystem->TriggerEvent(new MouseClickEvent(screenX, screenY, 0));
		}
		if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
		{
			int screenX = sdlEvent.button.x;
			int screenY = sdlEvent.button.y;
			g_pEventSystem->TriggerEvent(new MouseClickEvent(screenX, screenY, 2));
		}
	}
}