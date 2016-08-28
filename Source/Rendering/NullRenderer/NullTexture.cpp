#include "NullTexture.h"


NullTexture::NullTexture()
	:Texture(nullptr)
{
}

NullTexture::~NullTexture()
{
}


Texture * NullTexture::GetTexture() const
{
	return nullptr;
}
