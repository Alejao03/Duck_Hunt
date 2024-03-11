#pragma once
#include <cstdio>
#include <vector>

struct SDL_Texture;
struct SDL_Renderer;

/*Los ficheros proporcionados tienen extensión .rgba y contienen las imágenes sin comprimir. Cada una empieza con un entero de 4 bytes en big endian indicando el ancho en
píxeles de la imagen. A continuación aparece otro entero de 4 bytes con el alto en el mismo
formato. Por último, van ancho × alto píxeles, donde cada píxel está formado por cuatro
bytes, cada uno para los canales rojo, verde, azul y alfa, en ese orden. Los píxeles de la
imagen aparecen de izquierda a derecha y de arriba a abajo.

Se considera que un píxel es opaco si el valor del canal alfa es 255 (0xFF). Cualquier
otro valor se considera completamente transparente. Todos los píxeles de las imágenes
suministradas tienen alfas de 0xFF o 0x00.*/
class ImagePC
{
	int width;
	int height;
	SDL_Texture* text;
public:
	ImagePC(int width, int height, uint32_t* pixelDat, SDL_Renderer* rnd);
	~ImagePC();
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	SDL_Texture* getTexture() const;
};

