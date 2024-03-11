#pragma once
#include "Vector2D.h"

class Sprite;

class StaticEntity
{
public:
	StaticEntity(Vector2D position, Sprite* sprite, int width, int height);
	const Vector2D getPos() const;
	const Sprite* getSprite() const;
	const int getWidth() const;
	const int getHeight() const;
private:
	const Vector2D position;
	const Sprite* sprite;
	const int width, height;
};

