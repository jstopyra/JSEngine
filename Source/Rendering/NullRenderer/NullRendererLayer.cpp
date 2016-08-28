#include "NullRendererLayer.h"

bool NullRendererLayer::CreateNewWindow(const char*, int, int)
{
	return false;
}

void NullRendererLayer::CloseWindow()
{
}

bool  NullRendererLayer::Init()
{
	return false;
}
