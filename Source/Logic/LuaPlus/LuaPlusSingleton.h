#ifndef __LUAPLUSSINGLETON_H__
#define __LUAPLUSSINGLETON_H__

#include <LuaPlus.h>
#include <sdl.h>
class LuaPlusComponent;
using namespace LuaPlus;

class LuaPlusSingleton
{
	static LuaState* s_pLuaState;
	static LuaPlusSingleton* s_pInstance;
	static LuaObject s_metaTable;
public:
	static LuaPlusSingleton* GetInstance();
	static void Destroy();
	LuaState* GetLuaState();

	void InitMetaTable();
	LuaObject& GetMetaTable();


	//CppFunction to Register
	//LuaCall string to register
	//Object that has the function
	template <typename Callee, typename Func>
	void RegisterFunction(char* funcName, const Callee* callee, Func func, int objectId, LuaPlusComponent* pComponent)
	{
		
		//s_metaTable.RegisterObjectFunctor(luaCall, func);
		s_metaTable.RegisterObjectDirect(funcName, pComponent, func);
		LuaObject test1Obj = s_pLuaState->BoxPointer(&pComponent);
		test1Obj.SetMetaTable(s_metaTable);
		s_pLuaState->GetGlobals().SetObject(objectId + '0', test1Obj);
	}

private:
	LuaPlusSingleton();
	~LuaPlusSingleton();

};



#endif // !__LUAPLUSSINGLETON_H__
