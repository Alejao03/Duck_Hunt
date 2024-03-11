#include "Cursor.h"
#include "Renderer.h"
#include "Platform.h"
#include "Endianess.h"
#include "Sprite.h"
#include <math.h>
#include "Definitions.h"
#include "Input.h"
#include "Bird.h"
#include <list>
#include <InputState.h>

Cursor::Cursor(Vector2D position, Sprite* cursor_, Bird* birds_[], int numBirds_, int width, int height, float vel_, float giroscopeMultiplier_) : position(position), direction(0, 0),
joystickMultiplier(vel_), giroscopeMultiplier(giroscopeMultiplier_), sprite(cursor_), size(Vector2D(width, height)), birdsList(birds_), numOfBirds(numBirds_),
leftTriggerPressed(false), rightTriggerPressed(false), width(width), height(height), shootedWithButton(false), shootedWithTrigger(false)
{
}

Cursor::~Cursor()
{
}

void Cursor::update(float deltaTime)
{
	InputState e = Input::Instance()->GetInputState();

	updatePosition(deltaTime, e.motionX, e.motionY, e.angle.x, e.angle.y);
	handleTriggers(e.l2, e.r2);
	handleShoot(e.x || e.o);
}

const Sprite* Cursor::getSprite() const{ return sprite; }
Vector2D Cursor::getPos() { return position; }

const int Cursor::getWidth() const
{
	return this->width;
}

const int Cursor::getHeight() const
{
	return this->height;
}

bool Cursor::isOutOfBounds() const	
{
	return false;
}

void Cursor::updatePosition(float deltaTime, const float& motionX, const float& motionY, const float& accelerationX, const float& accelerationY)
{
	// No se mueve si ha sido golpeado
	position += (Vector2D(motionX, motionY) * joystickMultiplier + Vector2D(accelerationX, accelerationY) * giroscopeMultiplier) * deltaTime;
	int width = Renderer::Instance()->getWidth();
	int height = Renderer::Instance()->getHeight();

	// Clamp positions
	position.setX(std::min(std::max((int)position.getX(), 0), width));
	position.setY(std::min(std::max((int)position.getY(), 0), height));
}

void Cursor::handleShoot(bool button)
{
	bool trigger = rightTriggerPressed || leftTriggerPressed;


	if (button && !shootedWithButton)
	{
		shoot();
		shootedWithButton = true;
	}
	else if (!button)
		shootedWithButton = false;

	if (trigger && !shootedWithTrigger)
	{
		shoot();
		shootedWithTrigger = true;
	}
	else if (!trigger)
		shootedWithTrigger = false;
}

void Cursor::handleTriggers(float triggerLeft, float triggerRight)
{
	if (triggerLeft > 0.7f)
		this->leftTriggerPressed = true;
	else if (triggerLeft < 0.3f)
		this->leftTriggerPressed = false;
	
	if (triggerRight > 0.7f)
		this->rightTriggerPressed = true;
	else if (triggerRight < 0.3f)
		this->rightTriggerPressed = false;
}

void Cursor::shoot()
{
	for (int i = 0; i < numOfBirds; i++)
		if (birdsList[i]->handleShot(position))
			break;
}