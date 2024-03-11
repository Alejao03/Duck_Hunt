#include "PlatformPC.h"
#include <iostream>
#include <SDL.h> 
#include <iostream>
#include <assert.h>
#include <chrono>
#include "SDLEventListener.h"

typedef std::chrono::high_resolution_clock Clock;

PlatformPC* PlatformPC::instance_ = nullptr;

PlatformPC::PlatformPC()
{
}

PlatformPC::~PlatformPC() {

}

void PlatformPC::send(SDL_Event sdlEvent) const
{
	for (auto it = listeners.begin(); it != listeners.end(); ++it)
		(*it)->receive(sdlEvent);
}

bool PlatformPC::Init() {
	assert(instance_ == nullptr);
	srand(static_cast<unsigned int>(time(NULL))); // Explicit cast to avoid warning

	instance_ = new PlatformPC();

	return instance_->privateInit();
}

bool PlatformPC::privateInit() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		delete instance_;
		instance_ = nullptr;
		SDL_Quit();
		return false;
	}
	else
	{
		std::cerr << "SDL initialized!" << std::endl;
	}

	return true;
}

void PlatformPC::Release() {
	if (instance_) {
		SDL_Quit();

		delete instance_;
		instance_ = nullptr;
	}
}

FILE* PlatformPC::openBinaryFile(const std::string& name) const {
	FILE* file = nullptr;
	const char* archivo = name.c_str();
	fopen_s(&file, archivo, "r"); //permiso lectura
	if (file == NULL) {
		std::cerr << "Couldn't open File " + name << std::endl;
	}
	
	return file;
}

bool PlatformPC::tick() const {
	bool quit = false;

	// Event handler
	SDL_Event e;

	// Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			quit = true;
		}
		// EVENTOs PARA Listeners de Input
		else {
			send(e);
		}
	}

	return !quit;
}

PlatformPC* PlatformPC::Instance()
{
	return instance_;
}

// Returns the time passed since the start of the app in miliseconds
uint32_t PlatformPC::getTimeSinceStart() const
{
	return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now().time_since_epoch()).count());
}

/**
 * Wait a specified number of milliseconds before returning.
 *
 * This function waits a specified number of milliseconds before returning. It
 * waits at least the specified time, but possibly longer due to OS
 * scheduling.
 *
 * \param ms the number of milliseconds to delay
 *
 */
void PlatformPC::delay(uint32_t miliseconds) const
{
	SDL_Delay(miliseconds);
}

// Returns a random float between [min, max]
float PlatformPC::randomFloat(float min, float max) const
{
	return (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

// Returns a random int between [min, max]
int PlatformPC::randomInt(int min, int max) const
{
	min--;
	return rand() % (max - min) + min + 1;
}

void PlatformPC::addInputListener(SDLEventListener* inputListener)
{
	this->listeners.push_back(inputListener);
}

void PlatformPC::removeInputListener(SDLEventListener* inputListener)
{
	this->listeners.remove(inputListener);
}
