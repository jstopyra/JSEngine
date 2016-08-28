#ifndef __PLAYERCONTROLLERCOMPONENT_H__
#define __PLAYERCONTROLLERCOMPONENT_H__

#include "Component.h"
#include "../../Utilities/EventSystem/EventListener.h"

#include "../../Utilities/Vector3.h"
class GameObject;

template <class temp>
class Vector3;

//This component is responsible of taking an input from the user and determining of the player game object should move, interact etc
class PlayerControllerComponent : public Component, public EventListener
{
	
public:

	PlayerControllerComponent(GameObject* pParent);
	~PlayerControllerComponent();

	void OnEvent(const Event* pEvent) override;
	void OnUpdate(float deltaTime);

	void GetOnScreenPosition(Vector3<float>& returnVec);

private:

};

#endif // !__PLAYERCONTROLLERCOMPONENT_H__
