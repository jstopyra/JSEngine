#if _DEBUG
#include <vld.h>
#endif //_DEBUG

#include <Application/Application.h>

//#include <iostream>
#include <windows.h>

int main()
{
	//Init the engine
	Application::StartEngine();

	//Load a map
	Application::LoadMap("OasisMap");

	//Logic loop. MainLoop will cause the engine to close when returns false.
	while (Application::MainLoop())
	{
		//check if "E" key was pressed
		if (GetAsyncKeyState(0x45))
		{
			//Request the engine to stop the main loop
			Application::RequestClose();
		}
	}
	return 0;
}