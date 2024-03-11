#include "Bird.h"
#include "Renderer.h"
#include "Platform.h"
#include "Endianess.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include <math.h>
#include "Definitions.h"

Bird::Bird(Vector2D position, Vector2D direction, float vel, float duration, const  DuckSprites* animations, int groundHeight) : position(position), direction(direction),
vel(vel), lifeDuration(duration), lifeTimer(0), canBounce(true), current_(nullptr), size(Vector2D(BIRD_WIDTH_2_SCREEN, BIRD_HEIGHT_SCREEN)), sprites(animations), frameIndex(0),
timer(0), state(BirdState::BIRTH), hitDuration(HIT_DURATION), hitTimer(0), groundHeight(groundHeight), halfGroundHeight(groundHeight * 0.5)
{
	updateBirdDirection();
}

Bird::~Bird()
{
}

bool Bird::update(float deltaTime)
{
	lifeTimer += deltaTime;

	if (lifeTimer > lifeDuration)
		canBounce = false;

	updatePosition(deltaTime);

	// Handle Collisions
	handleCollisions();

	// Handle State
	updateState(deltaTime);

	// Finally, set the appropiate sprite of the current animation
	handleAnimation(deltaTime);

	return isOutOfBounds();
}

const Sprite* Bird::getSprite() const { return current_; }
Vector2D Bird::getPos() const { return position; }

void Bird::updateState(float deltaTime)
{
	int pos = (position.getY() + (size.getY() / 2));
	int ground = SCREEN_HEIGHT - groundHeight;
	if (state == BIRTH && pos < ground)
		state = LIVING;

	if (state == HITTED)
	{
		if (hitTimer > hitDuration)
		{
			state = FALLING;

			canBounce = false; // Independiente del tiempo de vida ya puede salir de la pantalla
		}

		hitTimer += deltaTime;
	}
}

bool Bird::isOutOfBounds() const
{
	if (canBounce)
		return false;

	int width = Renderer::Instance()->getWidth();
	int height = Renderer::Instance()->getHeight();

	// Mirar derecha e izquierda
	if (position.getX() - (size.getX() / 2) > width || position.getX() + (size.getX() / 2) < 0)
		return true;

	// Mirar arriba y abajo
	if (position.getY() - (size.getY() / 2) > height - getBaselineHeight() || position.getY() + (size.getY() / 2) < 0)
		return true;

	return false;
}

void Bird::handleCollisions()
{
	// Si aún quedan rebotes, comprobar colisiones
	if (canBounce)
	{
		int width = Renderer::Instance()->getWidth();
		int height = Renderer::Instance()->getHeight();

		// Comprobar colisiones por ambos laterales
		if (position.getX() - (size.getX() / 2) < 0 || position.getX() + (size.getX() / 2) > width)
		{
			direction.setX(direction.getX() * -1.0f);
			direction.setY(0);
			direction.rotateDegrees(Platform::Instance()->randomFloat(-BIRD_BOUNCE_ANGLE, BIRD_BOUNCE_ANGLE));
			direction = direction.normalized();

			// Clampear posiciones
			if (position.getX() - (size.getX() / 2) < 0)
				position.setX(size.getX() / 2);
			else
				position.setX(width - size.getX() / 2);

			updateBirdDirection();
		}

		// Comprobar colisiones por arriba y por abajo
		if (position.getY() - (size.getY() / 2) < 0 || position.getY() + (size.getY() / 2) > height - getBaselineHeight())
		{
			direction.setY(direction.getY() * -1);
			direction.setX(0);
			direction.rotateDegrees(Platform::Instance()->randomFloat(-BIRD_BOUNCE_ANGLE, BIRD_BOUNCE_ANGLE));
			direction = direction.normalized();

			// Clampear posiciones
			if (position.getY() - (size.getY() / 2) < 0)
				position.setY(size.getY() / 2);
			else
				position.setY((height - getBaselineHeight()) - size.getY() / 2);
			updateBirdDirection();
		}

		updateSprite();
	}
}

void Bird::updatePosition(float deltaTime)
{
	const Vector2D dir = state != FALLING ? direction : Vector2D(0, 1);
	const float velocity = state != FALLING ? vel : FALL_SPEED;

	// No se mueve si ha sido golpeado
	if (state != BirdState::HITTED)
		position += dir * velocity * deltaTime;
}

bool Bird::handleShot(Vector2D shotPoint)
{
	bool pointInsideRect = shotPoint.getX() > position.getX() - (size.getX() / 2) && shotPoint.getX() < position.getX() + (size.getX() / 2) &&
		shotPoint.getY() > position.getY() - (size.getY() / 2) && shotPoint.getY() < position.getY() + (size.getY() / 2);

	if (pointInsideRect)
	{
		if (state == BirdState::LIVING)
			state = BirdState::HITTED;

		return true;
	}

	return false;
}

void Bird::updateBirdDirection()
{
	float angle = direction.angle() + 180;

	if ((angle >= 330 && angle <= 360) || (angle >= 0 && angle <= 30))
		birdAnimation = BirdAnimationState::LEFT;
	else if (angle >= 30 && angle <= 60)
		birdAnimation = BirdAnimationState::UP_LEFT;
	else if (angle >= 60 && angle <= 120)
		birdAnimation = BirdAnimationState::UP;
	else if (angle >= 120 && angle <= 150)
		birdAnimation = BirdAnimationState::UP_RIGHT;
	else if (angle >= 150 && angle <= 210)
		birdAnimation = BirdAnimationState::RIGHT;
	else if (angle >= 210 && angle <= 270)
		birdAnimation = BirdAnimationState::DOWN_RIGHT;
	else if (angle >= 270 && angle <= 330)
		birdAnimation = BirdAnimationState::DOWN_LEFT;
}

void Bird::handleAnimation(float deltaTime)
{
	timer += deltaTime;
	if (timer > FRAME_DURATION)
	{
		frameIndex = (frameIndex + 1) % NUMBER_OF_FRAMES;
		timer = 0;
	}

	// El sprite lo actualizamos en cada iteracion por si el estado ha cambiado
	updateSprite();
}

void Bird::updateSprite()
{
	if (state == BIRTH || state == LIVING)
	{
		// Animacion ping pong, cuando llegamos al frame 3 volvemos al 1
		const int frame = frameIndex > 2 ? 1 : frameIndex;
		current_ = sprites->getSpriteFromDirection(birdAnimation, frame);
	}
	else if (state == HITTED)
		current_ = sprites->getHitSprite();
	else if (state == FALLING)
		// Esta animacion solo tiene 2 frames
		current_ = sprites->getFallingSprite(frameIndex % 2);
}

int Bird::getBaselineHeight() const
{
	return state == BIRTH ? halfGroundHeight : groundHeight;
}
