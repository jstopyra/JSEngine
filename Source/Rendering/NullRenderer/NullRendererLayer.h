#ifndef __NULLRENDERERLAYER_H__
#define __NULLRENDERERLAYER_H__

#include "../RenderDevice.h"

class NullRendererLayer : public RenderDevice
{
public:
	bool CreateNewWindow(const char* windowName, int windowWidth, int windowHeight) override;

	void CloseWindow() override;

private:

	bool Init() override;

};

#endif // !__NULLRENDERERLAYER_H__
