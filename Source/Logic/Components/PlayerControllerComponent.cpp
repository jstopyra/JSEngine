#include "PlayerControllerComponent.h"

#include <SDL.h>
#include "../../Utilities/EventSystem/Events/EventsInclude.h"
#include "../GameObject/GameObject.h"

#include "ComponentIncludes.h"
#include "../LuaPlus/LuaPlusSingleton.h"
#include <sstream>
#include <cstdlib>
#include "MovementComponent.h"
#include "LuaScriptComponent.h"

extern EventSystem* g_pEventSystem;

extern GameObject* g_pCamera;


PlayerControllerComponent::PlayerControllerComponent(GameObject* pParent)
	:Component(pParent)
{
	g_pEventSystem->AttachListener(k_inputKeyEvent, this);
	g_pEventSystem->AttachListener(k_updateEvent, this);
	g_pEventSystem->AttachListener(k_mouseClickEvent, this);

	if (g_pCamera)
	{
		g_pCamera->GetComponent<CameraComponent>()->SetTarget(GetParent()->GetComponent<TransformComponent>()->GetPosition());
	}

}

PlayerControllerComponent::~PlayerControllerComponent()
{

}

void PlayerControllerComponent::OnEvent(const Event* pEvent)
{
	if (pEvent->GetEventId() == k_updateEvent)
	{
		UpdateEvent* pUpdateEvent = (UpdateEvent*)(pEvent);
		OnUpdate(pUpdateEvent->GetDeltaTime());
	}
	else if (pEvent->GetEventId() == k_mouseClickEvent)
	{
		MouseClickEvent* pMouseEvent = (MouseClickEvent*)(pEvent);
		GetParent()->GetComponent<LuaScriptComponent>()->LuaOnMouseInput(pMouseEvent->GetXPos(), pMouseEvent->GetYPos(), pMouseEvent->GetButton());
	}

	else if (pEvent->GetEventId() == k_inputKeyEvent)
	{
		InputKeyEvent* pInputEvent = (InputKeyEvent*)(pEvent);
		//Read the inputkey
		SDL_Keycode keyCode = pInputEvent->KeyTypePressed();
		//
		const char* pressState;
		
		//convert input key into a string so we can pass it around
		std::string stringChar;
		std::stringstream ss;
		ss << (char)keyCode;
		ss >> stringChar;
		//Check if the key was pressed or released
		if (pInputEvent->WasKeyDown())
		{
			pressState = "press";
		}
		else
		{
			pressState = "release";
		}

		//Call the lua function
		GetParent()->GetComponent<LuaScriptComponent>()->LuaOnKeyboardInput(stringChar.c_str(), pressState);
	}
}

void PlayerControllerComponent::GetOnScreenPosition(Vector3<float>& returnVec)
{
	Vector3<float>* pCameraPos = g_pCamera->GetComponent<TransformComponent>()->GetPosition();
	Vector3<float>* pMyPos = GetParent()->GetComponent<TransformComponent>()->GetPosition();
	float x, y, z;
	x = pCameraPos->GetX() - pMyPos->GetX();
	y = pCameraPos->GetY() - pMyPos->GetY();
	z = pCameraPos->GetZ() - pMyPos->GetZ();
	//We have to multiply by -1 because the map grows in negative direction
	x *= -1;
	y *= -1;
	z *= -1;
	returnVec.SetXYZ(x, y, z);
}

void PlayerControllerComponent::OnUpdate(float)
{
	

}