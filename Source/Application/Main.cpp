//Main.cpp

//-------------------------------------------------------------------------------------------------
//TODO:
//	* Write a ton of defensive code.
//		- Go through and nullcheck the crap out of things.
//		- Assert on things that are crucial to the logic of the engine.
//
//	* Create GameObjectFactory.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//Author: Jakub Stopyra
//2/17/2016
//GameEngine
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//Libraries include
//-------------------------------------------------------------------------------------------------
#include <sdl.h>
#include "Application.h"
#include "../Utilities/DebugLog.h"

//-------------------------------------------------------------------------------------------------
// MAIN
//-------------------------------------------------------------------------------------------------
int main(int, char*[])
{
	DEBUGLOG("Program Started");

	Application::GetInstance();
	//Initialize the application and check if everything is ok.
	if (Application::GetInstance()->Init())
		//loop the application
		Application::GetInstance()->MainLoop();
	//Cleanup after the application
	Application::GetInstance()->Destroy();

	DEBUGLOG("Program Ended");

	//we're done :)
	return 0;
}