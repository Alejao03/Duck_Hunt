#include "ResourceManager.h"
#include "Renderer.h"
#include "Platform.h"
#include <exception>
#include <assert.h>
#include <cstdint>
#include <Endianess.h>

ResourceManager* ResourceManager::instance_ = nullptr;

ResourceManager::ResourceManager()
{

}

// Gets an image from the resource manager, returns nullptr if the image is not found
Image* ResourceManager::getImage(const std::string& imageName)
{
	auto image = this->imageMap.find(imageName);
	if (image != this->imageMap.end()) {
		return image->second;
	}
	else {
		return nullptr;
	}
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::Init()
{
	assert(instance_ == nullptr);
	instance_ = new ResourceManager();
	return true;
}

void ResourceManager::Release()
{
	// Borrar imagenes
	for (auto it = instance_->imageMap.begin(); it != instance_->imageMap.end(); ++it) {
		delete it->second;
	}

	delete instance_;
}

ResourceManager* ResourceManager::Instance()
{
	return instance_;
}

// Loads an image from a file and stores it in the resource manager
// Returns the image if the image was loaded correctly, nullptr otherwise
Image* ResourceManager::getTexture(const std::string& path)
{
	// Image key is filename plus extension without path
	std::string key = path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);

	//Image* img = nullptr;
	Image* img = this->getImage(key);
	if (!img)
	{
		// Esto habria que pasarlo a common
		FILE* file = Platform::Instance()->openBinaryFile(path);

		if (file == nullptr)
			return nullptr;

		//assert(file != nullptr); //quitar este assert 

		// Read the width and height
		uint32_t width = 0;
		uint32_t height = 0;
		size_t error = 0;
		error = fread(&width, sizeof(uint32_t), 1, file);
		if (error == 0)
			return nullptr;

		error = fread(&height, sizeof(uint32_t), 1, file);
		if (error == 0)
			return nullptr;

#ifdef  PLATFORM_LITTLE_ENDIAN
		width = SWAP_ENDIANESS32(width);
		height = SWAP_ENDIANESS32(height)
#endif // ! ENDIANESS_LITTLE

			// Read the pixel data
			uint32_t pixel = 0;

		// Read the pixel data
		uint32_t* pixelData = new uint32_t[width * height];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				error = fread(&pixel, sizeof(uint32_t), 1, file);
				if (error == 0)
					return nullptr;
#ifdef  PLATFORM_LITTLE_ENDIAN
				pixel = SWAP_ENDIANESS32(pixel)
#endif
					pixelData[y * width + x] = CONVERT_RGBA_TO_ARGB(pixel);
			}
		}

		// Close the file
		fclose(file);
		img = Renderer::Instance()->createImage(width, height, pixelData);
		this->imageMap.emplace(key, img);
	}

	// Create the sprite
	return img;
}
