#pragma once
#include <SDL.h>

#include "EndianessOperations.h"

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define PLATFORM_LITTLE_ENDIAN
#endif

