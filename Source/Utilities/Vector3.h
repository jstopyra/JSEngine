//Vector3.h

#ifndef __VECTOR3_H__
#define __VECTOR3_H__

template<typename type>
class Vector3
{

public:
	//Creates a vector3 with all values set to 0
	Vector3() : m_x(0), m_y(0), m_z(0) { m_initialVal = 0; };
	Vector3(type allValue) : m_x(allValue), m_y(allValue), m_z(allValue) { m_initialVal = allValue; };
	//creates a vector3 with values set to a specified params
	Vector3(type xValue, type yValue, type zValue) : m_x(xValue), m_y(yValue), m_z(zValue) { m_initialVal = 0; };
	~Vector3() {};

public:
	//Getters:
	type GetX() const { return m_x; }
	type GetY() const { return m_y; }
	type GetZ() const { return m_z; }

	static Vector3<type> Up() { return Vector3<type>(0, -1, 0); }
	static Vector3<type> Forward() { return Vector3<type>(0, 0, 1); }
	static Vector3<type> Right() { return Vector3<type>(1, 0, 0); }

	static Vector3<type> Down() { return Vector3<type>(0, 1, 0); }
	static Vector3<type> Back() { return Vector3<type>(0, 0, -1); }
	static Vector3<type> Left() { return Vector3<type>(-1, 0, 0); }


	//Setters:
	void SetX(type setValue) { m_x = setValue; }
	void SetY(type setValue) { m_y = setValue; }
	void SetZ(type setValue) { m_z = setValue; }

	//sets all values, X, Y, and Z to specified value
	void SetAll(type setValue) { m_x = setValue; m_y = setValue; m_z = setValue; }
	void SetXYZ(type xVal, type yVal, type zVal) { m_x = xVal; m_y = yVal; m_z = zVal; }

	//Sets all the values to 0;
	void Zero() { m_x = 0; m_y = 0; m_z = 0; }

	void Reset() { m_x = m_initialVal; m_y = m_initialVal; m_z = m_initialVal; }

	//Overloading the operators
	void operator*(type value)
	{
		SetX(GetX() * value);
		SetY(GetY() * value);
		SetZ(GetZ() * value);
	}

	Vector3<type> operator-(Vector3<type>& other)
	{
		SetX(GetX() + other.GetX());
		SetY(GetY() + other.GetY());
		SetZ(GetZ() + other.GetZ());
	}

private:
    type m_x;
    type m_y;
    type m_z;
	type m_initialVal;
};

#endif