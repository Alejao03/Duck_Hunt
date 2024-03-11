#pragma once
#include <vector>
#include <list>

class RenderThread;

class Sprite;
class Bird;
class DuckSprites;
class StaticEntity;
class Cursor;

#define NUM_OF_BIRDS 5

class BirdScene
{
public:
	BirdScene();
	virtual bool init(RenderThread*);
	virtual void update(float deltaTime);
	virtual void render();
	virtual void release();

private:
	RenderThread* renderThread;
	Bird* birds[NUM_OF_BIRDS];
	const DuckSprites* blueBirdSprites;
	const DuckSprites* blackBirdSprites;
	const DuckSprites* redBirdSprites;
	std::vector<const Sprite*> sprites;

	std::vector<const StaticEntity*> background;
	Cursor* cursorEntity;
	
	Bird* getNewBirb();
	bool loadAnimations(int color);
	bool loadBackground();
	bool loadCursor();
};

