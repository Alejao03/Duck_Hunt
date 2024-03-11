#pragma once
#include "Image.h"
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

class ResourceManager
{
private:
	static ResourceManager* instance_;
	std::unordered_map<std::string, Image*> imageMap;
	Image* getImage(const std::string& imageName);

	ResourceManager();
	~ResourceManager();
public:
	static bool Init();
	static void Release();
	static ResourceManager* Instance();

	Image* getTexture(const std::string& imagePath);
};

