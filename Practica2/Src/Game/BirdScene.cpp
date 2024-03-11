#include "BirdScene.h"
#include "Sprite.h"
#include "Platform.h"
#include "Renderer.h"
#include "Endianess.h"
#include "Bird.h"
#include <string>
#include "RenderThread.h"
#include "ResourceManager.h"
#include "DuckSprites.h"
#include <stack>
#include "StaticEntity.h"
#include "Cursor.h"
#include "Definitions.h"

BirdScene::BirdScene() : renderThread(nullptr), blueBirdSprites(nullptr), blackBirdSprites(nullptr), redBirdSprites(nullptr), birds(), cursorEntity(nullptr) {}

bool BirdScene::init(RenderThread* renderThread)
{
	bool correctInit = true;
	correctInit &= loadAnimations(0);
	correctInit &= loadAnimations(1);
	correctInit &= loadAnimations(2);
	correctInit &= loadBackground();
	correctInit &= loadCursor();
	if (!correctInit)
	{
		release();
		return false;
	}

	this->renderThread = renderThread;

	for (int i = 0; i < NUM_OF_BIRDS; i++)
		birds[i] = getNewBirb();

	return true;
}

void BirdScene::update(float deltaTime)
{
	for (int i = 0; i < NUM_OF_BIRDS; i++)
	{
		if (birds[i]->update(deltaTime))
		{
			delete birds[i];
			birds[i] = getNewBirb();
		}
	}

	cursorEntity->update(deltaTime);
}

void BirdScene::render()
{
	Command comandoRender{ }; // Pseudo inicializacion para evitar warning

	for (int i = NUM_OF_BIRDS - 1; i >= 0; i--)
	{
		Bird* bird = birds[i];

		comandoRender.type = DRAW_SPRITE;
		SpriteParam pajaro = { bird->getSprite(), (int)bird->getPos().getX(), (int)bird->getPos().getY(), BIRD_WIDTH_SCREEN, BIRD_HEIGHT_SCREEN };
		comandoRender.spriteParam = pajaro;
		this->renderThread->enqueCommand(comandoRender);
	}

	for (auto it = background.begin(); it != background.end(); ++it) {
		const StaticEntity* entity = (*it);

		comandoRender.type = DRAW_SPRITE;
		SpriteParam sprite = { entity->getSprite(), (int)entity->getPos().getX(), (int)entity->getPos().getY(), entity->getWidth(), entity->getHeight()};
		comandoRender.spriteParam = sprite;
		this->renderThread->enqueCommand(comandoRender);
	}

	if (cursorEntity)
	{
		comandoRender.type = DRAW_SPRITE;
		SpriteParam sprite = { cursorEntity->getSprite(), (int)cursorEntity->getPos().getX(), (int)cursorEntity->getPos().getY(), cursorEntity->getWidth(), cursorEntity->getHeight()};
		comandoRender.spriteParam = sprite;
		this->renderThread->enqueCommand(comandoRender);
	}
}

void BirdScene::release()
{
	for (auto it = sprites.begin(); it != sprites.end(); ++it) {
		delete* it;
	}

	for (auto it = background.begin(); it != background.end(); ++it) {
		delete* it;
	}

	sprites.clear();
	background.clear();

	if (birds[0] != nullptr)
	{
		for (int i = 0; i < NUM_OF_BIRDS; i++)
			delete birds[i];
	}

	if (blueBirdSprites)
		delete blueBirdSprites;
	if (blackBirdSprites)
		delete blackBirdSprites;
	if (redBirdSprites)
		delete redBirdSprites;

	blueBirdSprites = nullptr;
	blackBirdSprites = nullptr;
	redBirdSprites = nullptr;
	
	delete cursorEntity;
	cursorEntity = nullptr;
}

Bird* BirdScene::getNewBirb()
{
	int config = Platform::Instance()->randomInt(0, 2);
	std::string borb = "negro";
	const  DuckSprites* sprites = nullptr;
	float vel = 0;
	switch (config)
	{
	case 0:
		vel = BLACK_SPEED;
		sprites = blackBirdSprites;
		break;
	case 1:
		vel = BLUE_SPEED;
		sprites = blueBirdSprites;
		break;
	case 2:
		vel = RED_SPEED;
		sprites = redBirdSprites;
		break;
	default:
		break;
	}

	int width = Renderer::Instance()->getWidth();
	int height = Renderer::Instance()->getHeight();

	int xPos = Platform::Instance()->randomInt(BIRD_WIDTH_2_SCREEN / 2, width - BIRD_WIDTH_2_SCREEN / 2);

	Vector2D dir = Vector2D(0, -1);
	dir.rotate(Platform::Instance()->randomFloat(-5, 5));

	const int groundHeight = ResourceManager::Instance()->getTexture(GROUND_PATH)->getHeight() * SCALE_FACTOR;

	return new Bird(Vector2D(xPos, height - BIRD_HEIGHT_SCREEN / 2), dir, vel, BIRD_SCREEN_DURATON, sprites, groundHeight);
}

bool BirdScene::loadAnimations(int colorIndex)
{
	std::string color = "";
	switch (colorIndex)
	{
	case 0:
		color = "negro";
		break;
	case 1:
		color = "azul";
		break;
	case 2:
		color = "rojo";
		break;
	default:
		color = "negro";
		break;
	}

	ResourceManager* mngr = ResourceManager::Instance();

	std::vector<const Sprite*> spriteColorBirdDirection;
	std::vector<const Sprite*> spritesFallingAnimation;
	const Sprite* hitSprite;

	Image* bird0 = mngr->getTexture("assets\\" + color + "_0.rgba");
	Image* bird45 = mngr->getTexture("assets\\" + color + "_45.rgba");
	Image* bird90 = mngr->getTexture("assets\\" + color + "_90.rgba");
	Image* birdDado = mngr->getTexture("assets\\" + color + "_dado.rgba");
	Image* bird_menos_90 = mngr->getTexture("assets\\" + color + "_-90.rgba");

	// El juego no puede continuar tras este error
	if (bird0 == nullptr || bird45 == nullptr || bird90 == nullptr || birdDado == nullptr || bird_menos_90 == nullptr)
	{
		std::cerr << "No se han podido cargar las im�genes" << std::endl;
		return false;
	}

	// RIGHT, LEFT animations
	for (int i = 0; i < 6; i++)
	{
		spriteColorBirdDirection.push_back(Sprite::CreateSpriteFromRectangularSpriteSheet(bird0, 2, 3, i));
		sprites.push_back(spriteColorBirdDirection.back());
	}

	// UP_RIGHT, UP_LEFT animations
	for (int i = 0; i < 6; i++)
	{
		spriteColorBirdDirection.push_back(Sprite::CreateSpriteFromRectangularSpriteSheet(bird45, 2, 3, i));
		sprites.push_back(spriteColorBirdDirection.back());
	}

	// UP
	for (int i = 0; i < 3; i++)
	{
		spriteColorBirdDirection.push_back(Sprite::CreateSpriteFromRectangularSpriteSheet(bird90, 1, 3, i));
		sprites.push_back(spriteColorBirdDirection.back());
	}

	// DOWN_RIGHT, DOWN_LEFT (Son las mismas que UP_RIGHT, UP_LEFT actualmente)
	for (int i = 0; i < 6; i++)
	{
		spriteColorBirdDirection.push_back(Sprite::CreateSpriteFromRectangularSpriteSheet(bird45, 2, 3, i));
		sprites.push_back(spriteColorBirdDirection.back());
	}

	hitSprite = Sprite::CreateSpriteFromRectangularSpriteSheet(birdDado, 1, 1, 0);
	sprites.push_back(hitSprite);

	for (int i = 0; i < 2; i++)
	{
		spritesFallingAnimation.push_back(Sprite::CreateSpriteFromRectangularSpriteSheet(bird_menos_90, 1, 2, i));
		sprites.push_back(spritesFallingAnimation.back());
	}

	switch (colorIndex)
	{
	case 0:
		blackBirdSprites = new DuckSprites(spriteColorBirdDirection, hitSprite, spritesFallingAnimation);
		break;
	case 1:
		blueBirdSprites = new DuckSprites(spriteColorBirdDirection, hitSprite, spritesFallingAnimation);
		break;
	case 2:
		redBirdSprites = new DuckSprites(spriteColorBirdDirection, hitSprite, spritesFallingAnimation);
		break;
	default:
		break;
	}

	return true;
}

bool BirdScene::loadBackground()
{
	ResourceManager* mngr = ResourceManager::Instance();
	Image* arbol = mngr->getTexture(TREE_PATH);
	Image* arbusto = mngr->getTexture(BUSH_PATH);
	Image* suelo = mngr->getTexture(GROUND_PATH);

	// El juego no puede continuar tras este error
	if (arbol == nullptr || arbusto == nullptr || suelo == nullptr)
	{
		std::cerr << "No se han podido cargar las im�genes" << std::endl;
		return false;
	}

	Sprite* arbol_sprite = Sprite::CreateSpriteFromRectangularSpriteSheet(arbol, 1, 1, 0);
	Sprite* arbusto_sprite = Sprite::CreateSpriteFromRectangularSpriteSheet(arbusto, 1, 1, 0);
	Sprite* suelo_sprite = Sprite::CreateSpriteFromRectangularSpriteSheet(suelo, 1, 1, 0);

	sprites.push_back(arbol_sprite);
	sprites.push_back(arbusto_sprite);
	sprites.push_back(suelo_sprite);

	const int TREE_HEIGHT = arbol->getHeight() * SCALE_FACTOR;
	const int TREE_WIDTH = arbol->getWidth() * SCALE_FACTOR;
	const int BUSH_HEIGHT = arbusto->getHeight() * SCALE_FACTOR;
	const int BUSH_WIDTH = arbusto->getWidth() * SCALE_FACTOR;
	const int GROUND_HEIGHT = suelo->getHeight() * SCALE_FACTOR;
	const int GROUND_WIDTH = suelo->getWidth() * SCALE_FACTOR;

	const int heightMinusGround = SCREEN_HEIGHT - GROUND_HEIGHT;
	const int treePos = GROUND_WIDTH * 0.225;
	const int bushPos = GROUND_WIDTH * 0.75;

	background.push_back(new StaticEntity(Vector2D(treePos, heightMinusGround - (TREE_HEIGHT / 2)), arbol_sprite, TREE_WIDTH, TREE_HEIGHT));
	background.push_back(new StaticEntity(Vector2D(bushPos, heightMinusGround - (BUSH_HEIGHT / 2)), arbusto_sprite, BUSH_WIDTH, BUSH_HEIGHT));
	background.push_back(new StaticEntity(Vector2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (GROUND_HEIGHT / 2)), suelo_sprite, GROUND_WIDTH, GROUND_HEIGHT));

	return true;
}

bool BirdScene::loadCursor()
{
	ResourceManager* mngr = ResourceManager::Instance();
	Image* cursor = mngr->getTexture("assets\\cursor.rgba");

	if (cursor == nullptr)
	{
		std::cerr << "No se han podido cargar las im�genes" << std::endl;
		return false;
	}

	Sprite* cursor_sprite = Sprite::CreateSpriteFromRectangularSpriteSheet(cursor, 1, 1, 0);

	sprites.push_back(cursor_sprite);

	cursorEntity = new Cursor(Vector2D(CURSOR_X, CURSOR_Y), cursor_sprite, birds, NUM_OF_BIRDS, cursor->getWidth() * SCALE_FACTOR, cursor->getHeight() * SCALE_FACTOR);

	return true;
}
