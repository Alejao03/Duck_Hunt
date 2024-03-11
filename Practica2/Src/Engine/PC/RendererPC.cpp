#include "RendererPC.h"
#include <SDL.h>
#include <assert.h>
#include "PlatformPC.h"
#include "Image.h"
#include "SDL.h"

#include <iostream>

RendererPC* RendererPC::instance_ = nullptr;

RendererPC::RendererPC(int width, int height) : width_(width), height_(height), renderer_(nullptr), window_(nullptr)
{
}

bool RendererPC::Init(int winWidth, int winHeight, const char* nameWindow) 
{
	assert(instance_ == nullptr);
	instance_ = new RendererPC(winWidth, winHeight);

	return instance_->privateInit(nameWindow);
}

bool RendererPC::privateInit(const char* nameWindow) {
	// Create window
	window_ = SDL_CreateWindow(nameWindow, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_, height_, SDL_WINDOW_SHOWN);
	if (window_ == NULL)
	{
		delete instance_;
		instance_ = nullptr;
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		std::cout << "Window created!" << std::endl;
	}

	// Create renderer for window
	renderer_ = SDL_CreateRenderer(window_, -1, VSYNC ? SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_ACCELERATED);

	if (renderer_ == NULL)
	{
		SDL_DestroyWindow(window_);
		renderer_ = nullptr;
		window_ = nullptr;
		delete instance_;
		instance_ = nullptr;
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		std::cout << "Renderer created!" << std::endl;
	}

	SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
	//	SDL_SetWindowFullscreen(instance_->window_, 1);

	return true;
}

RendererPC::~RendererPC() {

}

void RendererPC::Release()
{
	if (instance_) {
		// Free resources and close SDL
		if(instance_->renderer_)
			SDL_DestroyRenderer(instance_->renderer_);
		if (instance_->window_)
			SDL_DestroyWindow(instance_->window_);

		instance_->renderer_ = nullptr;
		instance_->window_ = nullptr;

		delete instance_;
		instance_ = nullptr;
	}
}

// Clears the screen with the given argb color
void RendererPC::clear(uint32_t color)
{
	uint8_t a = (color >> 24) & 0xFF;
	uint8_t r = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8) & 0xFF;
	uint8_t b = (color >> 0) & 0xFF;

	// Clear screen
	SDL_SetRenderDrawColor(instance_->renderer_, r, g, b, a);
	SDL_RenderClear(instance_->renderer_);
}

void RendererPC::present()
{
	// Update screen
	SDL_RenderPresent(renderer_);
}

// Pon un pixel en la pantalla dado el color argb
void RendererPC::putPixel(int x, int y, uint32_t color)
{
	uint8_t a = (color >> 24) & 0xFF;
	uint8_t r = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8) & 0xFF;
	uint8_t b = (color >> 0) & 0xFF;

	SDL_SetRenderDrawColor(instance_->renderer_, r, g, b, a);
	SDL_RenderDrawPoint(instance_->renderer_, x, y);
}

// Dibujo un rectangulo con centro (x,y) dados el ancho y el ralto con el color en argb
void RendererPC::drawRect(int x, int y, int width, int height, uint32_t color)
{
	uint8_t a = (color >> 24) & 0xFF;
	uint8_t r = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8) & 0xFF;
	uint8_t b = (color >> 0) & 0xFF;

	SDL_SetRenderDrawColor(instance_->renderer_, r, g, b, a);
	SDL_Rect Rect = { x - (width / 2), y - (height / 2), width, height };
	SDL_RenderFillRect(instance_->renderer_, &Rect);
}

// Draws a image with center int dstX, dstY
/*
* orgX -> origin x of the source rectangle
* orgY -> origin y of the source rectangle
* orgW -> width of the source rectangle
* orgH -> height of the source rectangle
*
* dstX -> position x of the destination rectangle (center pivoted)
* dstY -> position y of the destination rectangle (center pivoted)
* dstW -> width of the destination rectangle
* dstH -> height of the destination rectangle
*/
void RendererPC::drawImage(const Image* image, int orgX, int orgY, int orgW, int orgH, int dstX, int dstY, int dstW, int dstH) {

	 SDL_Rect srcRect = { orgX, orgY, orgW, orgH };
	 SDL_Rect dstRect = { dstX - (dstW / 2), dstY - (dstH / 2), dstW, dstH };

	 SDL_RenderCopyEx(this->renderer_, image->getTexture(), &srcRect, &dstRect, 0, nullptr, SDL_FLIP_NONE);
}

// Crea una imagen a partir de los datos de pixelData
Image* RendererPC::createImage(int width, int height, uint32_t* pixelData)
{
	auto img = new Image(width, height, pixelData, this->renderer_);
	return img;
}

int RendererPC::getWidth() const
{
	return width_;
}

int RendererPC::getHeight() const
{
	return height_;
}

RendererPC* RendererPC::Instance()
{
	return instance_;
}
