//RenderComponent.h
#ifndef __RENDERCOMPONENT_H__
#define __RENDERCOMPONENT_H__

#include "Component.h"
#include <memory>
#include <string>
#include <vector>
class GameObject;
class Sprite;
class TileSet;

class RenderComponent : public Component
{
	std::vector<std::shared_ptr<Sprite>> m_pSprite;
	
public:

	RenderComponent(GameObject* pParent);
	virtual ~RenderComponent();
	void Init(int gid, GameObject* pOwner);

	//Returns the path of the texture used on the sprite
	const char* GetTexturePath() const;
	void SetAnimation(const std::string& animationName);
	//Will find a best animation to set after all the animations stop playing;
	void ResetAnimations();
	void GetSpriteRect(int& x, int& y, int& w, int& h);

};

#endif // !__RENDERCOMPONENT_H__
