#pragma once
#include <vector>
class Sprite;

// EL ORDEN DE LOS ENUM ES IMPORTANTE PARA LA LOGICA. ES EL MISMO QUE EL DE LOS SPRITES.
enum BirdAnimationState { RIGHT, LEFT, UP_RIGHT, UP_LEFT, UP, DOWN_RIGHT, DOWN_LEFT };

class DuckSprites
{
public:
	DuckSprites(const std::vector<const Sprite*>& directionSprites, const Sprite* hitSprite, const std::vector<const Sprite*>& fallingSprite);
	const Sprite* getSpriteFromDirection(BirdAnimationState directon, int frame) const;
	const Sprite* getHitSprite() const;
	const Sprite* getFallingSprite(int frame) const;
private:
	const std::vector<const Sprite*> sprites;
	const Sprite* hitSprite;
	const std::vector<const Sprite*> fallingSprite;
};