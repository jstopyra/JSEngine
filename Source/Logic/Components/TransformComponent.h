#ifndef __TRANSFORMCOMPONENT_H__
#define __TRANSFORMCOMPONENT_H__

#include "Component.h"

class GameObject;

template <class temp>
class Vector3;

class TransformComponent : public Component
{
	//--------------------------------------------------------------------------------------------
	//private member variables
	//--------------------------------------------------------------------------------------------

	Vector3<float>* m_pPosition;
	Vector3<float>* m_pRotation;
	Vector3<float>* m_pScale;

	//--------------------------------------------------------------------------------------------
	//END private member variables
	//--------------------------------------------------------------------------------------------

public:
	TransformComponent(GameObject* pParent);
	virtual ~TransformComponent();

	//--------------------------------------------------------------------------------------------
	//public Getters/Setters
	//--------------------------------------------------------------------------------------------

	void SetPosition(Vector3<float>& position);
	void SetScale(Vector3<float>& scale);
	void SetRotation(Vector3<float>& rotation);

	void SetPosition(float posX, float posY, float posZ);
	void SetRotation(float rotX, float rotY, float rotZ);
	void SetScale(float scaleX, float scaleY, float scaleZ);

	//x and y are top left corner position of an object
	Vector3<float>* GetPosition() {return m_pPosition; }
	Vector3<float>* GetRotation() { return m_pRotation; }
	//used as a width and height of an object
	Vector3<float>* GetScale() { return m_pScale; }

	//--------------------------------------------------------------------------------------------
	//END public Getters/Setters
	//--------------------------------------------------------------------------------------------

	template<class temp>
	void OffsetPosition(Vector3<temp>& offset)
	{
		temp x = m_pPosition->GetX();
		temp y = m_pPosition->GetY();
		temp z = m_pPosition->GetZ();

		x += offset.GetX();
		y += offset.GetY();
		z += offset.GetZ();
		m_pPosition->SetXYZ(x, y, z);
	}

	template<class temp>
	void OffsetPosition(temp deltaX, temp deltaY, temp deltaZ)
	{
		temp x = m_pPosition->GetX();
		temp y = m_pPosition->GetY();
		temp z = m_pPosition->GetZ();

		x += deltaX;
		y += deltaY;
		z += deltaZ;
		m_pPosition->SetXYZ(x, y, z);
	}

};

#endif // !__TRANSFORMCOMPONENT_H__
