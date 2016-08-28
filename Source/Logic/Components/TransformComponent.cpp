#include "TransformComponent.h"

#include "RenderComponent.h"
//#include "../GameObject/GameObject.h"
#include "../../Utilities/Vector3.h"

TransformComponent::TransformComponent(GameObject* pParent)
	: m_pPosition(nullptr)
	, m_pRotation(nullptr)
	, m_pScale(nullptr)
	, Component(pParent)
{
	m_pPosition = new Vector3<float>();
	m_pRotation = new Vector3<float>();
	m_pScale = new Vector3<float>(1.f, 1.f, 1.f);
}

TransformComponent::~TransformComponent()
{
	delete m_pPosition;
	delete m_pRotation;
	delete m_pScale;
}

void TransformComponent::SetPosition(float posX, float posY, float posZ)
{
	m_pPosition->SetX(posX);
	m_pPosition->SetY(posY);
	m_pPosition->SetZ(posZ);
}

void TransformComponent::SetPosition(Vector3<float>& position)
{
	SetPosition(position.GetX(), position.GetY(), position.GetZ());
}

void TransformComponent::SetRotation(float rotX, float rotY, float rotZ)
{
	m_pRotation->SetX(rotX);
	m_pRotation->SetY(rotY);
	m_pRotation->SetZ(rotZ);
}


void TransformComponent::SetRotation(Vector3<float>& rotation)
{
	SetRotation(rotation.GetX(), rotation.GetY(), rotation.GetZ());
}


void TransformComponent::SetScale(float scaleX, float scaleY, float scaleZ)
{
	m_pScale->SetX(scaleX);
	m_pScale->SetY(scaleY);
	m_pScale->SetZ(scaleZ);
}

void TransformComponent::SetScale(Vector3<float>& scale)
{
	SetScale(scale.GetX(), scale.GetY(), scale.GetZ());
}