//ResourceManager.h

#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include <string>
#include <vector>
#include <unordered_map>

class XmlReader;
class Resource;

typedef std::unordered_map<std::string, Resource*> ResourceMap;

class ResourceManager
{
	enum FileExtention
	{
		k_xml,
		k_png,
		k_gz,
		k_unknown
	};

	//unordered map of all the loaded resources paired with their filename.
	ResourceMap m_resourceMap;
public:
	
	ResourceManager() {	};

	~ResourceManager();

	//Checks file extention, finds the file, loads it and returns the file
	Resource* LoadResource(const char* fileName);


	Resource* GetResource(const char* fileName);


private:
	//Will check what kind of extention is the file
	FileExtention CheckFileExtention(const std::string fileName)
	{
		std::string fileExtention = fileName.substr(fileName.find_last_of(".") + 1);
		if (fileExtention == "xml")
		{
			return FileExtention::k_xml;
		}
		else if (fileExtention == "png")
		{
			//use the png loader
		return FileExtention::k_png;
		}
		else if (fileExtention == "gz")
		{
			//use the ZLib loader
		return FileExtention::k_gz;
		}
		return FileExtention::k_unknown;
	}



};
#endif // !__RESOURCEMANAGER_H__
