#include "GameObjectManager.h"

//#include "../Xml/XmlReader.h"
#include "GameObject.h"

#include "../../Logic/ResourceManager/BaseVirtual/ResourceManager.h"

#include "../../Utilities/EventSystem/Events/EventsInclude.h"
#include "../ResourceManager/TiledManager/TiledManager.h"
#include "../Components/ComponentIncludes.h"
#include "../../Utilities/Vector3.h"

#include "../LuaPlus/LuaPlusSingleton.h"

#include "../Components/MovementComponent.h"
#include "../Components/LuaScriptComponent.h"

#include "Application\Application.h"

#include <SDL.h>//SDL_IntersectRect
int GameObjectManager::s_idBuffer = 100000;
std::vector<int> GameObjectManager::s_idList;
std::vector<GameObject*> GameObjectManager::s_objects;
std::unordered_map<int, RenderComponent*> GameObjectManager::m_renderComponentMap;
std::vector<ColliderComponent*> GameObjectManager::s_colliderList;



extern EventSystem* g_pEventSystem;


GameObjectManager::GameObjectManager()
	:m_pTiledManager(nullptr)
{
	s_idList.reserve(s_idBuffer);
	//will create all the GameObjects included in the xml file and register them to the m_objects one by one.
	g_pEventSystem->AttachListener(k_mouseClickEvent, this);
	g_pEventSystem->AttachListener(k_inputKeyEvent, this);
	m_clickCounter = 0;
}

void GameObjectManager::LoadMap(const char* mapName)
{
	m_pTiledManager->LoadMap(mapName);
}

void GameObjectManager::Init()
{
	m_pTiledManager = new TiledManager();
	//m_pTiledManager->Init();

	//Register functions to lua
	LuaState* pLuaState = LuaPlusSingleton::GetInstance()->GetLuaState();
	LuaObject metaTable = pLuaState->GetGlobal("MetaTable");

	metaTable.RegisterObjectDirect("CppCreateObject", (GameObjectManager *)0, &GameObjectManager::CppCreateObject);
	metaTable.RegisterObjectDirect("CppRequestClose", (GameObjectManager *)0, &GameObjectManager::CppRequestClose);

	LuaObject object = pLuaState->BoxPointer(this);
	object.SetMetaTable(metaTable);
	pLuaState->GetGlobals().SetObject("ObjectManager", object);
}


GameObjectManager::~GameObjectManager()
{
	WipeObjects();
	delete m_pTiledManager;

}
void GameObjectManager::CppRequestClose()
{
	Application::RequestClose();
}
LuaObject GameObjectManager::CppCreateObject(float posX, float posY, float posZ,int gid)
{
	Vector3<float> position;
	Vector3<float> rotation;
	Vector3<float> scale;

	position.SetXYZ(posX, posY, posZ);
	rotation.SetXYZ( 0,  0,  0);
	scale.SetXYZ( 32,  32,  1);
	GameObject* pObject = GameObjectManager::CreateObject("newObject", position, rotation, scale, gid);

	LuaScriptComponent* pScriptComponent = new LuaScriptComponent(pObject);
	pObject->AddComponent(pScriptComponent);
	pScriptComponent->Init("LuaScript.lua");

	return pScriptComponent->GetLuaObject();
}


GameObject* GameObjectManager::CreateObject(const char* objectName, Vector3<float>& position, Vector3<float>& rotation, Vector3<float>& scale, int gid)
{
	//Create a new GameObject
	GameObject* pNewGameObject = new GameObject();

	//Set the position and rotation and scale of the GameObject
	TransformComponent* pTransform = pNewGameObject->GetComponent<TransformComponent>();
	pTransform->SetPosition(position);
	pTransform->SetScale(scale);
	pTransform->SetRotation(rotation);

	//Check if the texture we want to assign already exists
	auto it = m_renderComponentMap.find(gid);
	if (it != m_renderComponentMap.end())
	{
		//Renderer exists. Lets assign its pointer to the newly created object and Setup the renderer to know
		//about newly created texture
		pNewGameObject->AddComponent(it->second);
		it->second->Init(gid, pNewGameObject);
	}
	else
	{
		//Texture doesnt exist, lets create a renderer that has that texture
		RenderComponent* pRender = new RenderComponent(pNewGameObject);
		pNewGameObject->AddComponent(pRender);
		//Initialize the RenderComponent with the path to the texture
		pRender->Init(gid, pNewGameObject);
		m_renderComponentMap.emplace(std::make_pair(gid, pRender));
	}

	//register the object to the list of objects
	s_objects.push_back(pNewGameObject);
	//Save the object id under the map of objectNameToId
	if (!objectName)
		objectName = "tempName";

	return pNewGameObject;
}

//Check if the given ID already exists, if so, creates a new one and replaces the old one
void GameObjectManager::ValidateObjectId(int& objectId)
{
	bool collision = true;
	while (collision)
	{
		std::vector<int>::iterator it = std::find(s_idList.begin(), s_idList.end(), objectId);

		if (it == s_idList.end())
		{
			GameObjectManager::RegisterId(objectId);
			collision = false;
		}
		else
		{
			objectId = GameObjectManager::GenerateId();
		}
	}
}

void GameObjectManager::WipeObjects()
{
	for (auto it : s_objects)
	{
		DestroyObject(it);
	}
	s_objects.clear();
	for (auto renderComponent : m_renderComponentMap)
	{
		delete renderComponent.second;
	}
	m_renderComponentMap.clear();
}


void GameObjectManager::SaveObjects()
{
	objList* objectList = new objList();
	for (auto object : s_objects)
	{
		objectList->push_back(object);
	}
}

void GameObjectManager::OnEvent(const Event* pEvent)
{
	if (pEvent->GetEventId() == k_inputKeyEvent)
	{
		InputKeyEvent* pInputKey = (InputKeyEvent*)(pEvent);
		if (pInputKey->KeyTypePressed() == SDLK_e && pInputKey->WasKeyDown())
		{
			//m_pTiledManager->LoadMap("noOcd");
		}
	}
}

//Destroy gameobject with the given ID
void GameObjectManager::DestroyObjectWithId(int)
{

}

//destroy passed object
void GameObjectManager::DestroyObject(GameObject* pGameObject)
{

	delete pGameObject;
}

void GameObjectManager::RegisterCollision(ColliderComponent* pCollider)
{
	s_colliderList.push_back(pCollider);
}

bool GameObjectManager::CheckCollision(ColliderComponent& other, int& leftRight, int& topDown)
{
	for (auto rightCollider : s_colliderList)
	{
		if (other.HasCollision(*rightCollider, leftRight, topDown))
			return true;
	}
	return false;
}
