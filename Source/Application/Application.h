//Application.h
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//This is a macro that needs to be used before every function that we want to expose out of the dll into other applications using the dll
#define ENGINE_DLL_API __declspec(dllexport) 

#include "../Utilities/EventSystem/EventListener.h"

class RenderDevice;
class LuaPlusSingleton;
union SDL_Event;
enum class AppState
{
	k_opening
	, k_running
	, k_requestClose
	, k_closing
	, k_closed
};

class Application : public EventListener
{

	//--------------------------------------------------------------------------------------------
	//private member variables
	//--------------------------------------------------------------------------------------------
	AppState m_applicationState;
	
	static Application* m_pInstance;
	LuaPlusSingleton* m_pLuaPlusSingleton;

	unsigned int m_lastTicks;

	//--------------------------------------------------------------------------------------------
	//END private member variables
	//--------------------------------------------------------------------------------------------


public:

	//--------------------------------------------------------------------------------------------
	//Public Interface For Game
	//--------------------------------------------------------------------------------------------

	//will initialize the engine and start a loop for the renderer and logic
	static ENGINE_DLL_API void StartEngine();

	//loops the game untill the state of the application changes.
	//If the loop stops the engine will close down all of its components.
	static ENGINE_DLL_API bool MainLoop();

	//calls the object manager to load a specified map. The map must live under Media\Maps
	static ENGINE_DLL_API void LoadMap(const char* mapName);

	//public interface to close the engine
	static ENGINE_DLL_API void RequestClose();

	//--------------------------------------------------------------------------------------------
	//Public Interface For Engine
	//--------------------------------------------------------------------------------------------

	//Return current state of the application
	const AppState GetApplicationState() const { return m_applicationState; }

	//Entry point for the event manager
	void OnEvent(const Event* pEvent) override;

	//Get the instance of the Application singleton
	static ENGINE_DLL_API Application* GetInstance();

private:
	//private constructor so nothing can create the singleton but itself
	Application();

	//Initialize the Application, returns fail if fails
	bool Init();

	//destroys the engine elements and the game instance
	ENGINE_DLL_API void Destroy();

	//Private Destructor so nothing can destroy the singleton but itself
	~Application();

	//Read events from sdl
	void ReadEvent(SDL_Event sdlEvent);

	//--------------------------------------------------------------------------------------------
	//Internal functions that call the singleton version of the function
	//--------------------------------------------------------------------------------------------
	//Loop until there is a request to close the engine. Clean up the engine on false.
	bool InternalMainLoop();

	//Load a specified map by name
	void InternalLoadMap(const char* mapName);

	//Request a close on an engine
	void InternalRequestClose();
};

#endif // !__APPLICATION_H__


