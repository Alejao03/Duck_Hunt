#pragma once

#include "Vector2D.h"
#include <cstdint>
#include <string>
#include <vector>
#include "DuckSprites.h"
#include "Renderer.h"

struct Color;
class Sprite;

enum BirdState { BIRTH, LIVING, HITTED, FALLING };

class Bird
{
public:
	Bird(Vector2D position, Vector2D direction, float vel, float duration, const  DuckSprites* animations, int groundHeight);
	~Bird();
	bool update(float deltaTime);
	bool isOutOfBounds() const;
	const Sprite* getSprite() const;
	Vector2D getPos() const;

	// Practica 4

	// Dado el punto en el que se ha disparado, comprobar si ha sido afectado y devolver true si fue dado
	// De esta forma si había un pájaro detrás este no morirá porque solo muere un pájaro por disparo
	bool handleShot(Vector2D shotPoint);

private:
	Vector2D position;
	Vector2D direction;
	Vector2D size;
	float vel;

	BirdState state;

	float lifeDuration;
	float lifeTimer;
	float hitDuration;
	float hitTimer;

	bool canBounce;

	int frameIndex;
	const int NUMBER_OF_FRAMES = 4;
	float timer;
	const DuckSprites* sprites;
	BirdAnimationState birdAnimation;
	const Sprite* current_;

	int groundHeight;
	int halfGroundHeight;

	void handleCollisions();
	void handleAnimation(float deltaTime);
	void updatePosition(float deltaTime);
	// Actualizar estado (BirdState)
	void updateState(float deltaTime);
	void updateBirdDirection();
	void updateSprite();
	int getBaselineHeight() const;
};

