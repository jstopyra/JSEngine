#ifndef __SDLRESOURCEMANAGER_H__
#define __SDLRESOURCEMANAGER_H__

class Resource;


class SdlResourceManager
{
public:
	SdlResourceManager();
	~SdlResourceManager();

	static Resource* LoadSdlTexture(const char*);
		
private:

};

#endif // !__RESOURCEMANAGER_H__
