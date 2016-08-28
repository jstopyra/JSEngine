#include "CameraComponent.h"
#include "../GameObject/GameObject.h"

#include "../../Utilities/EventSystem/Events/EventsInclude.h"

extern EventSystem* g_pEventSystem;

CameraComponent::CameraComponent(int cameraWidth, int cameraHeight, GameObject* pParent)
	: Component(pParent)
	, m_cameraWidth(cameraWidth)
	, m_cameraHeight(cameraHeight)
	, m_screenCoushion(10)
{
	g_pEventSystem->AttachListener(k_updateEvent, this);

	g_pEventSystem->AttachListener(k_cameraMoveEvent, this);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::UpdateCamera()
{ 
	//Check if the pTarget is still in our view
	Vector3<float>* myPos = GetParent()->GetComponent<TransformComponent>()->GetPosition();
	if (m_pTargetPos)
	{
		float rightEdge = myPos->GetX() + (m_cameraWidth / 2);
		float leftEdge = myPos->GetX() - (m_cameraWidth / 2);
		float topEdge = myPos->GetY() - (m_cameraHeight / 2);
		float bottomEdge = myPos->GetY() + (m_cameraHeight / 2);
		if (m_pTargetPos->GetX() + 32 > rightEdge - m_screenCoushion)
		{
			GetParent()->GetComponent<TransformComponent>()->SetPosition(rightEdge - m_screenCoushion, myPos->GetY(), myPos->GetZ());
		}
		else if (m_pTargetPos->GetX() + 32 < leftEdge + m_screenCoushion)
		{
			GetParent()->GetComponent<TransformComponent>()->SetPosition(leftEdge + m_screenCoushion, myPos->GetY(), myPos->GetZ());
		}
		else if (m_pTargetPos->GetY() + 32 < topEdge + m_screenCoushion)
		{
			GetParent()->GetComponent<TransformComponent>()->SetPosition(myPos->GetX(), topEdge + m_screenCoushion, myPos->GetZ());
		}
		else if (m_pTargetPos->GetY() + 32 > bottomEdge - m_screenCoushion)
		{
			GetParent()->GetComponent<TransformComponent>()->SetPosition(myPos->GetX(), bottomEdge - m_screenCoushion, myPos->GetZ());
	}
	}
}


void CameraComponent::SetTarget(Vector3<float>* pTargetVector)
{
	m_pTargetPos = pTargetVector;
}

void CameraComponent::OnEvent(const Event* pEvent)
{
	if (pEvent->GetEventId() == k_updateEvent)
	{
		UpdateCamera();
	}
	
	if (pEvent->GetEventId() == k_cameraMoveEvent)
	{
		//Move the camera
		CameraMoveEvent* pMoveEvent = (CameraMoveEvent*)(pEvent);
		Vector3<float>* pPosition = GetParent()->GetComponent<TransformComponent>()->GetPosition();
		float xPos = pPosition->GetX() + pMoveEvent->GetDeltaX()*-1;
		float yPos = pPosition->GetY() + pMoveEvent->GetDeltaY()*-1;
		float zPos = pPosition->GetZ() + pMoveEvent->GetDeltaZ()*-1;
		GetParent()->GetComponent<TransformComponent>()->SetPosition(xPos, yPos, zPos);
	}
}

Vector3<float>* CameraComponent::GetPosition()
{
	return GetParent()->GetComponent<TransformComponent>()->GetPosition();
}

int CameraComponent::GetWidth()
{
	return m_cameraWidth;
}

int CameraComponent::GetHeight()
{
	return m_cameraHeight;
}