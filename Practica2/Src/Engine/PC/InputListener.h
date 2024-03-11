#pragma once

union SDL_Event;

class InputListener
{
public:
	virtual void receive(SDL_Event* event) = 0;
};
