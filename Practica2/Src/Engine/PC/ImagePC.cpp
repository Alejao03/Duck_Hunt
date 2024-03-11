#include "ImagePC.h"
#include "SDL.h"

ImagePC::ImagePC(int width, int height, uint32_t* pixelDat, SDL_Renderer* rnd) : width(width), height(height), text (nullptr)
{
	// 0xAARRGGBB
	uint32_t alpha_mask = 0xFF000000;
	uint32_t red_mask = 0x00FF0000;
	uint32_t green_mask = 0x0000FF00;
	uint32_t blue_mask = 0x000000FF;

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixelDat, width, height, 32, width * 4, red_mask, green_mask, blue_mask, alpha_mask);
	
	if (surface != nullptr) 
	{
		// Crear texture a partir del surface
		text = SDL_CreateTextureFromSurface(rnd, surface);

		// Liberar recursos del surface
		SDL_FreeSurface(surface);
	}

	delete[] pixelDat;
}

SDL_Texture* ImagePC::getTexture() const
{
	return this->text;
}

ImagePC::~ImagePC()
{
	if(text != nullptr)
		SDL_DestroyTexture(text);
}
