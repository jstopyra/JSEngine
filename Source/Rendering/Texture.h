#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include "../Logic/ResourceManager/BaseVirtual/Resource.h"
#include <string>

class RenderComponent;

class Texture : public Resource
{

	
	const char* m_charPath;


public: //Methods
	Texture(const char* pFileName):m_charPath(pFileName){}
	virtual ~Texture() {};

	virtual Texture* GetTexture() const = 0;

	const char* GetTexturePath() {return m_charPath;}

private: //Methods
};

#endif // !__SDLTEXTURE_H__
