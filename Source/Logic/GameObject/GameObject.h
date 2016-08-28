#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "../Components/Component.h"
#include <typeinfo>
#include <unordered_map>
#include <string>
#include <memory>

///TODO:
//GameObject needs to have a transform component.
//Transform component cannot be removed from the object

class RenderComponent;
class TransformComponent;

class GameObject
{

	RenderComponent* m_pRenderComponent;
	TransformComponent* m_pTransformComponent;
	std::string m_uniqueName;
	std::unordered_map<const std::type_info*, Component*> m_components;
	int m_objectId;

public:
	GameObject(int objectId = 0);
	~GameObject();


	int GetObjectId() const { return m_objectId; }
	std::string& GetUniqueName();
	
	//removes the specified component from the component list if it exits
	template< class ComponentType >
	void RemoveComponent()
	{
		if (typeid(ComponentType) == typeid(m_pTransformComponent))
		{
			delete m_pTransformComponent;
			m_pTransformComponent = nullptr;
			return;
		}
		if (m_components.count(&typeid(ComponentType)) != 0)
		{
			m_components.erase(&typeid(ComponentType));
		}
	}

	void AddComponent(const char* component);

	//adds the specified component to the compnent list
	template< class ComponentType >
	void AddComponent(ComponentType* pNewComponent)
	{
		if (typeid(pNewComponent).name() == typeid(m_pTransformComponent).name())
		{	
			m_pTransformComponent = (TransformComponent*)(pNewComponent);
			return;
		}
		else if (typeid(ComponentType).name() == typeid(RenderComponent).name())
		{
			m_pRenderComponent = (RenderComponent*)(pNewComponent);
			return;
		}
		m_components[&typeid(ComponentType)] = pNewComponent;
	}

	//Checks the passed type of the component and checks if it exists and returns it.
	//TransformComponent is constant lookup
	template< class ComponentType >
	ComponentType* GetComponent()
	{
		if (typeid(ComponentType).name() == typeid(*m_pTransformComponent).name())
			return (ComponentType*)(m_pTransformComponent);
		else if (typeid(ComponentType).name() == typeid(RenderComponent).name())
			return (ComponentType*)(m_pRenderComponent);

		auto it = m_components.find(&typeid(ComponentType));
		if(it != m_components.end())
			return static_cast<ComponentType*> (it->second);
		return nullptr;
	}

private:
	void AddRequiredComponents();
};

#endif // !__GAMEOBJECT_H__
