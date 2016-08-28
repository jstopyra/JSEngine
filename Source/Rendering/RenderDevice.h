#ifndef __RENDERDEVICE_H__
#define __RENDERDEVICE_H__

class RenderDevice
{
public:
	RenderDevice() {};
	virtual ~RenderDevice() {};
	virtual bool Init() = 0;


	virtual bool CreateNewWindow(const char* windowName, int windowWidth, int windowHeight) = 0;
	virtual void CloseWindow() = 0;
	virtual const int GetWindowWidth()const = 0;
	virtual const int GetWindowHeight() const= 0;
};
#endif // !__RENDERDEVICE_H__
