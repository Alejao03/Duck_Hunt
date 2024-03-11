#include "Sprite.h"
#include "Renderer.h"
#include "Image.h"

#include <assert.h>

Sprite::Sprite(const Image* image, int orgX, int orgY, int width, int height) {
	assert(image != nullptr);
	image_ = image;

	//Sprite en File
	xImage_ = orgX;
	yImage_ = orgY;
	widthImage_ = width;
	heightImage_ = height;
}

// Draws the sprite in the screen with center in (x,y)
const void Sprite::draw(int x, int y, int width, int height) const
{									//sx1, sy1, sx2, sy2, tx1, ty1, tx2, ty2
	Renderer::Instance()->drawImage(image_, xImage_, yImage_, widthImage_, heightImage_, x, y, width, height);
}

// Allows to create a sprite from a rectangular sprite sheet that has no empty spaces between sprites
Sprite* Sprite::CreateSpriteFromRectangularSpriteSheet(const Image* image, int numRows, int numCols, int cell)
{
	int spriteWidth = image->getWidth() / numCols;
	int spriteHeight = image->getHeight() / numRows;
	int row = cell / numCols;
	int col = cell % numCols;
	int xImage = col * spriteWidth;
	int yImage = row * spriteHeight;

	return new Sprite(image, xImage, yImage, spriteWidth, spriteHeight);
}