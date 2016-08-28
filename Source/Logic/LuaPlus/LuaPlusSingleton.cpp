#include "luaplusSingleton.h"
LuaPlusSingleton* LuaPlusSingleton::s_pInstance = nullptr;
LuaState* LuaPlusSingleton::s_pLuaState = LuaState::Create(true);
LuaObject LuaPlusSingleton::s_metaTable = s_pLuaState->GetGlobals().CreateTable("MetaTable");

LuaPlusSingleton* LuaPlusSingleton::GetInstance()
{
	if (!s_pInstance)
		s_pInstance = new LuaPlusSingleton();
	return s_pInstance;
}

LuaPlusSingleton::LuaPlusSingleton()
{
	InitMetaTable();
	s_metaTable.SetObject("__index", s_metaTable);
}
void LuaPlusSingleton::Destroy()
{
	delete s_pInstance;
}
LuaPlusSingleton::~LuaPlusSingleton()
{
	LuaState::Destroy(s_pLuaState);
}

LuaState* LuaPlusSingleton::GetLuaState()
{
	return s_pLuaState;
}

void LuaPlusSingleton::InitMetaTable()
{
	s_pLuaState->DoFile("LuaScript.lua");
	//s_pLuaState->DoFile("PlayerLuaScript.lua");

}

LuaObject& LuaPlusSingleton::GetMetaTable()
{
	return s_metaTable;
}
