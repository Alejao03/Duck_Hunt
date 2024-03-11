#pragma once

#include <iostream>
#include <fstream>
#include "Image.h"

// facilitar el control de los sprites, entendidos como una porción de una imagen,
class Sprite
{
private:
	//Guardar Imagen
	const Image* image_;
	//Sprite en File
	int xImage_, yImage_;
	int widthImage_, heightImage_;

public:
	Sprite(const Image* image, int orgX, int orgY, int width, int height);
	const void draw(int x, int y, int width, int height) const;
	static Sprite* CreateSpriteFromRectangularSpriteSheet(const Image* image, int numCols, int numRows, int cell);
};

