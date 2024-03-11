#include "StaticEntity.h"

StaticEntity::StaticEntity(Vector2D position, Sprite* sprite, int width, int height) : position(position), sprite(sprite), width(width), height(height) {}

const Vector2D StaticEntity::getPos() const
{
    return position;
}

const Sprite* StaticEntity::getSprite() const
{
    return sprite;
}

const int StaticEntity::getWidth() const
{
    return this->width;
}

const int StaticEntity::getHeight() const
{
    return this->height;
}
