//MovementComponent.h
#ifndef __MOVEMENTCOMPONENT_H__
#define __MOVEMENTCOMPONENT_H__

#include "Component.h"
#include "../../Utilities/Vector3.h"
#include "../../Utilities/EventSystem/EventListener.h"

class GameObject;


class MovementComponent : public Component, public EventListener
{
	int m_movementSpeed;
	Vector3<float> m_velocity;
public:
	//speed is in pixels per second
	MovementComponent(GameObject* pParent, int movementSpeed = 300);
	Vector3<float>* GetVelocity() { return &m_velocity; }
	int CppSetVelocity(float x, float y, float z);
	void OnEvent(const Event* pEvent) override;
	void OnUpdate(float deltaTime);
	int GetMovementSpeed() const { return m_movementSpeed; }
};

#endif // !__MOVEMENTCOMPONENT_H__

