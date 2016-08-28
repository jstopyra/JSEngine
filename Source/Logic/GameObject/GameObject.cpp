//GameObject.cpp
#include "GameObject.h"

#include "../Components/ComponentIncludes.h"
#include "../Components/MovementComponent.h"
#include "../Components/LuaScriptComponent.h"
#include "GameObjectManager.h"
#include "../../Utilities/DebugLog.h"
#include "../Components/ButtonComponent.h"
#include <stdlib.h>
#include <string.h>

GameObject::GameObject(int objectId)
{
  
    //make sure we're using a valid ID
    GameObjectManager::ValidateObjectId(objectId);
    m_objectId = objectId;
	m_components.reserve(10);

    AddRequiredComponents();

	//set the name of the object to its id number
	int bufferSize = 20;
	char* uniqueName = new char[bufferSize]();
	memset(uniqueName, 0, bufferSize);
	_itoa_s(m_objectId, uniqueName, bufferSize, 10);
	m_uniqueName = uniqueName;
	delete uniqueName;
}


void GameObject::AddRequiredComponents()
{
	//force a TransformComponent to be added onto a GameObject
	AddComponent(new TransformComponent(this));
}

GameObject::~GameObject()
{
	//Clear all our components
	auto it = m_components.begin();
	while (it != m_components.end())
	{
		delete it->second;
		it->second = nullptr;
		it = m_components.erase(it);
	}
	delete m_pTransformComponent;
	m_pTransformComponent = nullptr;
	m_components.clear();
}

std::string& GameObject::GetUniqueName()
{
	return m_uniqueName;
}

//Add component by its name. The name of the component must match exacly!
void GameObject::AddComponent(const char* component)
{
	//TODO: This is horrible string compare!! Needs to be replaced with a more friendly solution
	std::string componentName = (component);
	if (componentName == "TransformComponent")
	{

	}
	else if (componentName == "ColliderComponent")
	{
		float x, y, w, h;
		if (m_pTransformComponent)
		{
			x = m_pTransformComponent->GetPosition()->GetX();
			y = m_pTransformComponent->GetPosition()->GetY();
			w = m_pTransformComponent->GetScale()->GetX();
			h = m_pTransformComponent->GetScale()->GetY();
			AddComponent(new ColliderComponent(this, x, y, w, h));
		}
	}
	else if (componentName == "LuaScriptComponent")
	{
		AddComponent(new LuaScriptComponent(this));

	}
	else if (componentName == "MovementComponent")
	{
		AddComponent(new MovementComponent(this));
	}
	else if (componentName == "PlayerControllerComponent")
	{
		AddComponent(new PlayerControllerComponent(this));

	}
	else if (componentName == "RenderComponent")
	{

	}
	else if (componentName == "ButtonComponent")
	{
		AddComponent(new ButtonComponent(this));
	}
	else
	{
		DEBUGLOG("GameObject::AddComponent: Specified component doesnt exist:");
		DEBUGLOG(component);
	}
}