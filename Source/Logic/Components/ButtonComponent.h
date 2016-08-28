#ifndef __BUTTONCOMPONENT_H__
#define __BUTTONCOMPONENT_H__

#include "Component.h"
#include "../../Utilities/EventSystem/EventListener.h"

//A component used for mouse input recognition
class ButtonComponent : public Component, public EventListener
{
public:
	ButtonComponent(GameObject* pParent);

	void OnEvent(const Event* pEvent) override;

};

#endif // !__BUTTONCOMPONENT_H__
