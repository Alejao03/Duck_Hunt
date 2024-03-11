#pragma once

#include "Vector2D.h"
#include <cstdint>
#include <string>
#include <vector>
#include <list>

class Sprite;
class Bird;

class Cursor
{
public:
	Cursor(Vector2D position, Sprite* cursor_, Bird* birds_[], int numBirds, int width, int height, float vel_ = 550,float giroscopeMultiplier_ = 500);
	~Cursor();
	void update(float deltaTime);
	void shoot();
	bool isOutOfBounds() const;
	const Sprite* getSprite() const;
	Vector2D getPos();
	const int getWidth() const;
	const int getHeight() const;
private:
	Vector2D position;
	Vector2D direction;
	Vector2D size;
	int width, height;
	float joystickMultiplier;
	float giroscopeMultiplier;
	Bird** birdsList;
	int numOfBirds;
	bool shootedWithButton;
	bool shootedWithTrigger;
	bool leftTriggerPressed;
	bool rightTriggerPressed;

	Sprite* sprite;

	void updatePosition(float deltaTime, const float& motionX, const float& motionY, const float& accelerationX, const float& accelerationY);
	void handleShoot(bool button);
	void handleTriggers(float triggerLeft, float triggerRight);
};