#pragma once
#include "Sprite.h"
#include "Vector2D.h"

enum CommandType { CLEAR, DRAW_SPRITE, PRESENT, STOP };

struct SpriteParam {
	const Sprite* sprite;
	int x;
	int y;
	int width;
	int height;
};

struct Command {
public:
	inline Command() = default;
	CommandType type;
	union {
		uint32_t color; // CLEAR
		SpriteParam spriteParam; // DRAW_BIRD		
	};
};