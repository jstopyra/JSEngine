//LuaScriptComponent.h

#include "Component.h"
#include <LuaPlus.h>
#include <string>
using namespace LuaPlus;

class LuaScriptComponent : public Component
{
	std::string m_scriptName;

	//My meta table
	LuaObject m_metaTable;
	//My luaObject
	LuaObject m_luaObject;
	//References to the lua functions
	LuaObject m_luaOnMouseInput;
	LuaObject m_luaOnKeyboardInput;
	LuaObject m_luaOnClicked;

public:
	LuaScriptComponent(GameObject* pParent);
	~LuaScriptComponent();

	void Init(const std::string& fileName);
	void DoFile();

	//LuaFunctions called from CPP
	//Key Press
	void LuaOnKeyboardInput(const char* input, const char* pressState);
	//MousePress
	void LuaOnMouseInput(int posX, int posY, int button);
	//Mouse clicked on collider
	void LuaOnClicked();

	//CPP functions called from lua
	int CppPlayAnimation(const char* animationName);
	int CppResetAnimation();
	int CppSetVelocity(float x, float y, float z);

	void PrintInt(int number);
	void Print(const char* string);

	int CppGetPositionX();
	int CppGetPositionY();
	int CppGetPositionZ();

	int CppGetVelocityX();
	int CppGetVelocityY();
	int CppGetVelocityZ();

	int CppGetOnScreenX();
	int CppGetOnScreenY();
	int CppGetOnScreenZ();
	int CppSetRotation(float rotationX, float rotationY, float rotationZ);
	int CppAddComponent(const char* component);
	

	LuaObject& GetLuaObject();

	
};