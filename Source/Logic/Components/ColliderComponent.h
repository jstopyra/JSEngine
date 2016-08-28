//ColliderComponent.h
#ifndef __COLLIDERCOMPONENT_H__
#define __COLLIDERCOMPONENT_H__

#include "Component.h"
#include "../GameObject/GameObjectManager.h"
#include <vector>
#include <SDL.h>//SDL_IntersectRect
#include <SDL_rect.h>
#include "../../Rendering/RenderDevice.h"

extern SDL_Renderer* g_pRenderer;


class ColliderComponent : public Component
{
public:
	enum class Layers
	{
		kPlayer,
		kProjectile,
		kTerrain,
		kLayersNum
	};

private:
	float m_x, m_y;
	float m_w, m_h;
	std::vector<Layers> m_collisionLayers;

public:

	ColliderComponent(GameObject* pParent, float x, float y, float w, float h)
		:Component(pParent), 
		m_x(x), m_y(y), m_w(w), m_h(h)
	{
		//m_w /= 2;
		//m_h /= 2;
		//Register the collider
		//Passed position was at the center of the object. Lets reset the rect to be at the top left corner of the object.
		m_x -= w / 2;
		m_y -= h / 2;
		GameObjectManager::RegisterCollision(this);
	};

	//Check if there is collision and what side was the collision on
	bool HasCollision(ColliderComponent& other, int& leftRight, int& topDown)
	{
		SDL_Rect leftRect;
		GetCollisionRegion(leftRect.x, leftRect.y, leftRect.w, leftRect.h);

		SDL_Rect rightRect;

		//skip if we're checking the same collider
		if (GetParent() == other.GetParent())
			return false;

		other.GetCollisionRegion(rightRect.x, rightRect.y, rightRect.w, rightRect.h);

		//Add the radius of our collider to the width of the other collider
		//rightRect.w += leftRect.w;
		//rightRect.x -= leftRect.w/2;

		//rightRect.h += leftRect.h;
		//rightRect.y -= leftRect.h/2;
		//SDL_Point myPos;
		//myPos.x = leftRect.x + (leftRect.w/2);
		//myPos.y = leftRect.y + (leftRect.h / 2);



		int bottomCol = rightRect.y + rightRect.h - leftRect.y;
		int topCol = leftRect.y + leftRect.h - rightRect.y;
		int leftCol = leftRect.x + leftRect.w - rightRect.x;
		int rightCol = rightRect.x + rightRect.w - leftRect.x;
		//Check if there was collision
		//if (SDL_PointInRect(&myPos, &rightRect))
		if (SDL_HasIntersection(&leftRect, &rightRect))
		{
			//Check if the collision was on the top
			if (topCol < bottomCol && topCol < leftCol && topCol < rightCol)
			{
				//Top collision
				topDown = 1;
			}
			//Check if the collision was on the bottom
			if (bottomCol < topCol && bottomCol < leftCol && bottomCol < rightCol)
			{
				//bottom collision
				topDown = -1;
			}
			//Check if the collision was on the left
			if (leftCol < rightCol && leftCol < topCol && leftCol < bottomCol)
			{
				//Left collision
				leftRight = -1;
			}
			//Check if the collision was on the right
			if (rightCol < leftCol && rightCol < topCol && rightCol < bottomCol)
			{
				//Right collision
				leftRight = 1;
			}
			return true;
		}
		return false;
	}

	//Get the collider size
	const void GetCollisionRegion (int& x, int& y, int& w, int& h) const
	{
		x = (int)(m_x + 0.5f);
		y = (int)(m_y + 0.5f);
		w = (int)(m_w + 0.5f);
		h = (int)(m_h + 0.5f);
	}

	const void GetCollisionRegion(int& x, int& y) const
	{
		x = (int)m_x;
		y = (int)m_y;
	}

	//Move the collider by the specific delta
	const void ShiftCollider(float deltaX, float deltaY)
	{
		if( deltaX != 0)
		m_x += deltaX;
		if(deltaY != 0)
		m_y += deltaY;
	}

	//Set the collider position to a specific values
	const void SetCollider(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

};

#endif // !__COLLIDERCOMPONENT_H__
