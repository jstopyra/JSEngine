//ButtonComponent.cpp
#include "ButtonComponent.h"

#include "../../Utilities/EventSystem/Events/EventsInclude.h"
#include "../../Utilities/Vector3.h"
#include "../GameObject/GameObject.h"
#include "TransformComponent.h"
#include "LuaScriptComponent.h"
#include "CameraComponent.h"

extern EventSystem* g_pEventSystem;
extern GameObject* g_pCamera;

ButtonComponent::ButtonComponent(GameObject* pParent)
	:Component(pParent)
{
	g_pEventSystem->AttachListener(k_mouseClickEvent, this);

}

void ButtonComponent::OnEvent(const Event* pEvent)
{
	if (pEvent->GetEventId() == k_mouseClickEvent)
	{
		//Check if the mouse click happened within the objects rect.
		MouseClickEvent* pClick = (MouseClickEvent*)(pEvent);
		if (pClick)
		{
			//position of the camera
			Vector3<float>* pCamPosition = g_pCamera->GetComponent<TransformComponent>()->GetPosition();
			//our global position
			Vector3<float>* pPosition = GetParent()->GetComponent<TransformComponent>()->GetPosition();
			//our scale.
			Vector3<float>* pScale = GetParent()->GetComponent<TransformComponent>()->GetScale();

			//Take the position of the object and offset it by the position of the camera -This will give use position on the screen of the object
			//Offset it by the half of the width of the object - this will go from the center position opf the object to the left corner of the object

			float posX = pPosition->GetX() - pCamPosition->GetX() - (pScale->GetX()/2);
			float posY = pPosition->GetY() - pCamPosition->GetY() - (pScale->GetY()/2);
			int x = pClick->GetXPos();
			int y = pClick->GetYPos();
			//Check if x is within the rect
			if (x < posX + pScale->GetX() && x > posX &&
				//Check if y is within the rect
				y < posY + pScale->GetY() && y > posY)
			{
				//Tell our script component that there was a click
				GetParent()->GetComponent<LuaScriptComponent>()->LuaOnClicked();
			}
		}
	}
}