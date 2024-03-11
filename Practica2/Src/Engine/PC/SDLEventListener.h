#pragma once

union SDL_Event;

class SDLEventListener
{
public:
	virtual ~SDLEventListener() {};
	virtual void receive(const SDL_Event& event) = 0;
};