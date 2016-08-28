#ifndef __NULLTEXTURE_H__
#define __NULLTEXTURE_H__

#include "../Texture.h"

class NullTexture : public Texture
{
public:
	NullTexture();
	virtual ~NullTexture();

	virtual Texture* GetTexture() const  override;
private:

};

#endif // !__NULLTEXTURE_H__
