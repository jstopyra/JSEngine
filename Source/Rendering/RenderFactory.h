#ifndef __RENDERFACTORY_H__
#define __RENDERFACTORY_H__

//Define if you want to use Nullrender layer
//#define NULLRENDERER
#define SDLLAYER

#ifdef NULLRENDERER
    #include "../Rendering/NullRenderer/NullRendererLayer.h"
	#include "NullRenderer\NullTexture.h"
#elif defined(SDLLAYER)
    #include "../Rendering/SDL/SdlRenderDevice.h"
	#include "SDL\SdlTexture.h"
#endif // NULLRENDERER

#include "../Rendering/RenderDevice.h"
#include "Texture.h"

class RenderFactory
{
public:
	//return a new instance of the renderer layer depending on which define we're in
	static bool CreateRenderLayer(RenderDevice* &pReturnDevice)
	{
		#ifdef NULLRENDERER
		pReturnDevice = new NullRendererLayer();

		#elif defined(SDLLAYER)
		pReturnDevice = new SdlRenderDevice();

		#endif
		//will return fail if Init() function fails
		return pReturnDevice->Init();
	}

	static bool CreateTexture(Texture* &pReturnTexture, const char* texturePath)
	{
		#ifdef NULLRENDERER
		pReturnTexture = new NullTexture();

		#elif defined(SDLLAYER)		
		pReturnTexture = new SdlTexture(texturePath);

		#endif

		return true;
	}

};
#endif // !__RENDERFACTORY_H__
