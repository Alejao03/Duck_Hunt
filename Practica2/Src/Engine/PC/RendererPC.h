#pragma once
#include "Image.h"
#include <cstdint>
#include <string>

#define DISPLAY_BUFFER_NUM 2 // minimum 2 required, maximum 16

struct Color {
	static uint32_t White() { return 0xffffffff; };
	static uint32_t Red() { return 0xffff0000; };
	static uint32_t Blue() { return 0xff0000ff; };
	static uint32_t Green() { return 0xff00ff00; };
	static uint32_t Black() { return 0xff000000; };
};

struct SDL_Renderer;
struct SDL_Window;

// En modo Release, la práctica deberá ir a 60 FPS (con sincronización vertical)
const bool VSYNC = true;

class RendererPC
{
private:
	static RendererPC* instance_;
	SDL_Renderer* renderer_; 
	SDL_Window* window_;
	int width_;
	int height_;

	RendererPC(int height, int width);
	~RendererPC();
	bool privateInit(const char* nameWindow);

public:
	void clear(uint32_t color);
	void putPixel(int x, int y, uint32_t color);
	void drawRect(int x, int y, int width, int height, uint32_t color); //Horizontal y Vertical
	void present();
	int getWidth() const;
	int getHeight() const;
	void drawImage(const Image* image, int orgX, int orgY, int orgW, int orgH, int dstX, int dstY, int dstW, int dstH);
	Image* createImage(int width, int height, uint32_t* pixelData);
	static bool Init(int winWidth = 800, int winHeight = 600, const char* nameWindow = "SDL_Tutorial");
	static void Release();
	static RendererPC* Instance();
};



