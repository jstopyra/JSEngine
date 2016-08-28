#ifndef __GAMEOBJECTMANAGER_H__
#define __GAMEOBJECTMANAGER_H__


#include <unordered_map>
#include <vector>
//class GameObject;
#include "GameObject.h"
#include "../../Utilities/EventSystem/EventListener.h"

#include <LuaPlus.h>
using namespace LuaPlus;


template<class type>
class Vector3;
class RenderComponent;
class ColliderComponent;
class TileSet;
class TiledManager;

typedef std::vector<GameObject*> objList;

class GameObjectManager : public EventListener
{
	TiledManager* m_pTiledManager;
	static std::unordered_map<int , RenderComponent*> m_renderComponentMap;

	int m_clickCounter;

public:
	GameObjectManager();
	~GameObjectManager();

	void OnEvent(const Event* pEvent) override;

	//Goes through all the ID's and see if we have a collision.
	//Will create and replace the old ID with a new one in case of collision
	static void ValidateObjectId(int& objectId);

	void CppRequestClose();

	//save out the entire game onto an xml file.
	static void SaveObjects();

	static void WipeObjects();

	//Destroy gameobject with the given ID
	static void DestroyObjectWithId(int itemId);
	//destroy passed object
	static void DestroyObject(GameObject* pGameObject);
	
	static void RegisterCollision(ColliderComponent* pCollider);
	
	static bool CheckCollision(ColliderComponent& other, int& leftRight, int& topDown);

	static GameObject* CreateObject(const char* objectName, Vector3<float>& position, Vector3<float>& rotation, Vector3<float>& scale, int gid);
	LuaObject CppCreateObject(float posX, float posY, float posZ, int gid);

	TiledManager* GetTiledManager() const { return m_pTiledManager; }

	void Init();
	void LoadMap(const char* mapName);

private:
	

	//stores all the Gameobjects under their unique ID.
	static std::vector<GameObject*> s_objects;

	//list of all the collider components that have been instantiated into the game
	static std::vector<ColliderComponent*> s_colliderList;

	//------------------------------------------------------------------------------------------------------------------------------
	// ID STUFF
	//------------------------------------------------------------------------------------------------------------------------------
	//The highest id to be created
	static int s_idBuffer;
	static int GenerateId() {return rand() % s_idBuffer;}
	static void RegisterId(int objectId) { s_idList.push_back(objectId); }

	//list of all the registered IDs in the game
	static std::vector<int> s_idList;
	//------------------------------------------------------------------------------------------------------------------------------
	// END-ID STUFF
	//------------------------------------------------------------------------------------------------------------------------------

};
#endif // !__GAMEOBJECTMANAGER_H__
