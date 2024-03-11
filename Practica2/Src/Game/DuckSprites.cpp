#include "DuckSprites.h"

DuckSprites::DuckSprites(const std::vector<const Sprite*>& sprites, const Sprite* hitSprite, const std::vector<const Sprite*>& fallingSprite) : 
	sprites(sprites), hitSprite(hitSprite), fallingSprite(fallingSprite) {}

// El orden de los sprites es el mismo que en el enum y deb eser así, cada direccion tiene 3 frames
const Sprite* DuckSprites::getSpriteFromDirection(BirdAnimationState direction, int frame) const
{	
	// Campo auxiliar para evitar overflow (advertencia de visual studio)
	const int index = direction * 3 + frame; 
	return sprites[index];
}

const Sprite* DuckSprites::getHitSprite() const
{
	return hitSprite;
}

const Sprite* DuckSprites::getFallingSprite(int frame) const
{
	return fallingSprite[frame];
}
