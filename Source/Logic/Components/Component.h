// Component.h
#ifndef __COMPONENT_H__
#define __COMPONENT_H__


class GameObject;

class Component
{

	//this is parent gameobject that owns me.
	GameObject* m_pParentObject;

public:
	
	//Constructor needs a pointer to the GameObject that owns the component
	Component(GameObject* pParent) : m_pParentObject(pParent) {};
	virtual ~Component() {};

	//Returns the parent that owns the component
	GameObject* GetParent() { return m_pParentObject; }

};

#endif
