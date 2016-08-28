//LuaScriptComponent.cpp
#include "LuaScriptComponent.h"
#include "../GameObject/GameObject.h"
#include "../LuaPlus/LuaPlusSingleton.h"
#include "MovementComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "../../Utilities/DebugLog.h"

LuaScriptComponent::LuaScriptComponent(GameObject* pParent)
:Component(pParent)
{
	//Create metatable for this GameObject instance

	
}

LuaScriptComponent::~LuaScriptComponent()
{
	
}

#ifdef _DEBUG
//This function should only run in debug. we dont want anything printing in release
void LuaScriptComponent::Print(const char* string)
{
	DEBUGLOG(string);
}
void LuaScriptComponent::PrintInt(int number)
{
	std::string s = std::to_string(number);
	const char* string = s.c_str();
	DEBUGLOG(string);
}
#else
void LuaScriptComponent::Print(const char*)
{
	//This function only runs in Debug
}
void LuaScriptComponent::PrintInt(int)
{
	//This function only runs in Debug
}
#endif // DEBUG

LuaObject& LuaScriptComponent::GetLuaObject()
{
	return m_luaObject;
}

void LuaScriptComponent::Init(const std::string& fileName)
{
	m_scriptName = fileName;
	DoFile();
	std::string uniqueName = GetParent()->GetUniqueName();       // number to be converted to a string

	LuaState* pLuaState = nullptr;
	pLuaState = LuaPlusSingleton::GetInstance()->GetLuaState();
	pLuaState->DoFile(m_scriptName.c_str());
	std::string metaTableName(uniqueName);
	metaTableName.append("Table");
	m_metaTable = pLuaState->GetGlobals().CreateTable(metaTableName.c_str());
	m_metaTable.SetObject("__index", m_metaTable);

	m_metaTable.RegisterObjectDirect("CppResetAnimation", (LuaScriptComponent *)0, &LuaScriptComponent::CppResetAnimation);
	m_metaTable.RegisterObjectDirect("CppPlayAnimation", (LuaScriptComponent *)0, &LuaScriptComponent::CppPlayAnimation);
	m_metaTable.RegisterObjectDirect("CppSetVelocity", (LuaScriptComponent *)0, &LuaScriptComponent::CppSetVelocity);

	m_metaTable.RegisterObjectDirect("CppGetPositionX", (LuaScriptComponent *)0, &LuaScriptComponent::CppGetPositionX);
	m_metaTable.RegisterObjectDirect("CppGetPositionY", (LuaScriptComponent *)0, &LuaScriptComponent::CppGetPositionY);
	m_metaTable.RegisterObjectDirect("CppGetPositionZ", (LuaScriptComponent *)0, &LuaScriptComponent::CppGetPositionZ);

	m_metaTable.RegisterObjectDirect("CppGetVelocityX", (LuaScriptComponent *)0, &LuaScriptComponent::CppGetVelocityX);
	m_metaTable.RegisterObjectDirect("CppGetVelocityY", (LuaScriptComponent *)0, &LuaScriptComponent::CppGetVelocityY);
	m_metaTable.RegisterObjectDirect("CppGetVelocityZ", (LuaScriptComponent *)0, &LuaScriptComponent::CppGetVelocityZ);

	m_metaTable.RegisterObjectDirect("CppGetOnScreenX", (LuaScriptComponent *)0, &LuaScriptComponent::CppGetOnScreenX);
	m_metaTable.RegisterObjectDirect("CppGetOnScreenY", (LuaScriptComponent *)0, &LuaScriptComponent::CppGetOnScreenY);
	m_metaTable.RegisterObjectDirect("CppGetOnScreenZ", (LuaScriptComponent *)0, &LuaScriptComponent::CppGetOnScreenZ);

	m_metaTable.RegisterObjectDirect("CppAddComponent", (LuaScriptComponent *)0, &LuaScriptComponent::CppAddComponent);
	m_metaTable.RegisterObjectDirect("CppSetRotation", (LuaScriptComponent *)0, &LuaScriptComponent::CppSetRotation);


	m_metaTable.RegisterObjectDirect("Print", (LuaScriptComponent *)0, &LuaScriptComponent::Print);
	m_metaTable.RegisterObjectDirect("PrintInt", (LuaScriptComponent *)0, &LuaScriptComponent::PrintInt);


	m_luaObject = pLuaState->BoxPointer(this);
	m_luaObject.SetMetaTable(m_metaTable);
	pLuaState->GetGlobals().SetObject(uniqueName.c_str(), m_luaObject);

	//Hook up lua functions to cpp calls
	m_luaOnKeyboardInput = LuaPlusSingleton::GetInstance()->GetLuaState()->GetGlobal("OnKeyboardInput");
	m_luaOnMouseInput = LuaPlusSingleton::GetInstance()->GetLuaState()->GetGlobal("OnMouseInput");
	m_luaOnClicked = LuaPlusSingleton::GetInstance()->GetLuaState()->GetGlobal("OnClicked");

}

void LuaScriptComponent::DoFile()
{
	LuaPlusSingleton::GetInstance()->GetLuaState()->DoFile(m_scriptName.c_str());
}

void LuaScriptComponent::LuaOnKeyboardInput(const char* input, const char* pressState)
{
	DoFile();
	if (&m_luaObject)
	{
		LuaFunction<int> luaCall(m_luaOnKeyboardInput);
		luaCall(input, m_luaObject, pressState);
	}
}

void LuaScriptComponent::LuaOnMouseInput(int posX, int posY, int button)
{
	DoFile();
	LuaPlusSingleton::GetInstance()->GetLuaState()->DoFile("PlayerLuaScript.lua");
	LuaFunction<int> luaCall(m_luaOnMouseInput);
	luaCall(m_luaObject, posX, posY, button);
}

void LuaScriptComponent::LuaOnClicked()
{
	DoFile();
	LuaFunction<int> luaCall(m_luaOnClicked);
	luaCall();
}


int LuaScriptComponent::CppAddComponent(const char* component)
{
	GetParent()->AddComponent(component);
	return 0;
}


int LuaScriptComponent::CppPlayAnimation(const char* animationName)
{
	RenderComponent* pRender = GetParent()->GetComponent<RenderComponent>();
	if(pRender)
		pRender->SetAnimation(animationName);
	return 0;
}

int LuaScriptComponent::CppResetAnimation()
{
	RenderComponent* pRender = GetParent()->GetComponent<RenderComponent>();
	if(pRender)
		pRender->ResetAnimations();
	return 0;
}

int LuaScriptComponent::CppSetVelocity(float x, float y, float z)
{

	MovementComponent* pMovement = GetParent()->GetComponent<MovementComponent>();
	if(pMovement)
		pMovement->CppSetVelocity(x, y, z);
	return 0;
}


int LuaScriptComponent::CppGetPositionX()
{
	TransformComponent* pTransform = GetParent()->GetComponent<TransformComponent>();
	if(pTransform)
		return (int)pTransform->GetPosition()->GetX();
	return 0;
}

int LuaScriptComponent::CppGetPositionY()
{
	TransformComponent* pTransform = GetParent()->GetComponent<TransformComponent>();
	if (pTransform)
		return (int)pTransform->GetPosition()->GetY();

	return 0;
}

int LuaScriptComponent::CppGetPositionZ()
{
	TransformComponent* pTransform = GetParent()->GetComponent<TransformComponent>();
	if (pTransform)
		return (int)pTransform->GetPosition()->GetZ();

	return 0;
}


int LuaScriptComponent::CppGetVelocityX()
{
	MovementComponent* pMovement = GetParent()->GetComponent<MovementComponent>();
	if(pMovement)
		return (int)pMovement->GetVelocity()->GetX();
	return 0;
}
int LuaScriptComponent::CppGetVelocityY()
{
	MovementComponent* pMovement = GetParent()->GetComponent<MovementComponent>();
	if (pMovement)
		return (int)pMovement->GetVelocity()->GetY();
	return 0;
}
int LuaScriptComponent::CppGetVelocityZ()
{
	MovementComponent* pMovement = GetParent()->GetComponent<MovementComponent>();
	if (pMovement)
		return (int)pMovement->GetVelocity()->GetZ();
	return 0;
}


int LuaScriptComponent::CppGetOnScreenX()
{
	PlayerControllerComponent* pPlayer = GetParent()->GetComponent<PlayerControllerComponent>();
	if (pPlayer)
	{
		Vector3<float> positionVec;
		pPlayer->GetOnScreenPosition(positionVec);
		return (int)positionVec.GetX();
	}
	return 0;
}
int LuaScriptComponent::CppGetOnScreenY()
{
	PlayerControllerComponent* pPlayer = GetParent()->GetComponent<PlayerControllerComponent>();
	if (pPlayer)
	{
		Vector3<float> positionVec;
		pPlayer->GetOnScreenPosition(positionVec);
		return (int)positionVec.GetY();
	}
	return 0;
}
int LuaScriptComponent::CppGetOnScreenZ()
{
	PlayerControllerComponent* pPlayer = GetParent()->GetComponent<PlayerControllerComponent>();
	if (pPlayer)
	{
		Vector3<float> positionVec;
		pPlayer->GetOnScreenPosition(positionVec);
		return (int)positionVec.GetZ();
	}
	return 0;
}

int LuaScriptComponent::CppSetRotation(float rotationX, float, float)
{
	TransformComponent* pTransform = GetParent()->GetComponent<TransformComponent>();
	if (pTransform)
		pTransform->GetRotation()->SetX(rotationX);
	return 0;
}