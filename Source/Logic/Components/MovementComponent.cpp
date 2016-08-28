//MovementComponent.cpp

#include "MovementComponent.h"
#include "../LuaPlus/LuaPlusSingleton.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"

#include "../../Utilities/EventSystem/Events/EventsInclude.h"
extern EventSystem* g_pEventSystem;

MovementComponent::MovementComponent(GameObject* pParent, int movementSpeed)
	:Component(pParent)
	, m_movementSpeed(movementSpeed)
{
	m_velocity.SetAll(0.f);
	g_pEventSystem->AttachListener(k_updateEvent, this);

}

int MovementComponent::CppSetVelocity(float x, float y, float z)
{
	m_velocity.SetX(m_velocity.GetX() + x * m_movementSpeed);
	m_velocity.SetY(m_velocity.GetY() + y * m_movementSpeed);
	m_velocity.SetZ(m_velocity.GetZ() + z * m_movementSpeed);
	return 0;
}

void MovementComponent::OnUpdate(float deltaTime)
{
	Vector3<float> preciseOffset;
	float x = m_velocity.GetX() * deltaTime;
	float y = m_velocity.GetY() * deltaTime;
	float z = m_velocity.GetZ() * deltaTime;
	preciseOffset.SetXYZ(x, y, z);
	//Check for collision
	//If collision happened then we have to move the character back to the oposite of his velocity
	GameObject* pParent = GetParent();
	if (pParent)
	{
		pParent->GetComponent<TransformComponent>()->OffsetPosition<float>(preciseOffset);
		ColliderComponent* pCollider = pParent->GetComponent<ColliderComponent>();
		if (pCollider)
		{
			pCollider->ShiftCollider(preciseOffset.GetX(), preciseOffset.GetY());
			int collisionX = 0;
			int collisionY = 0;
			//Check collision
			if (GameObjectManager::CheckCollision(*pCollider, collisionX, collisionY))
			{
				//There was a horizontal collision, lets reset our x position
				if (collisionX == -1 || collisionX == 1)
				{
					x *= -1;
				}
				//There was no horizontal collision so lets not change anything.
				else
				{
					x = 0;
				}
				//There was a vertical collision, lets reset our y position
				if (collisionY == -1 || collisionY == 1)
				{
					y *= -1;
				}
				//There was no vertical collision, we wont shift in y
				else
				{
					y = 0;
				}
				//set the offset we'll be doing
				preciseOffset.SetXYZ(x, y, z);
				//shift the transform
				pParent->GetComponent<TransformComponent>()->OffsetPosition<float>(preciseOffset);
				//shift the collider
				pCollider->ShiftCollider(preciseOffset.GetX(), preciseOffset.GetY());
			}
		}
	}
}

void MovementComponent::OnEvent(const Event* pEvent)
{
	if (pEvent->GetEventId() == k_updateEvent)
	{
		UpdateEvent* pUpdateEvent = (UpdateEvent*)(pEvent);
		OnUpdate(pUpdateEvent->GetDeltaTime());
	}
}
