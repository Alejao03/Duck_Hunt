#include "Renderer.h"
#include "Platform.h"
#include "Vector2D.h"

#include <iostream>
#include <vector>
#include <list>
#include <cstdint>
#include <stack>

#include "BirdScene.h"
#include "RenderThread.h"
#include "Bird.h"
#include "ResourceManager.h"
#include "Input.h"

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //Hay que estar en Debug

	if (!Platform::Init())
	{
		std::cerr << "Platform::Init failed" << std::endl;
		return 1;
	}

	if (!Renderer::Init(1920, 1080))
	{
		std::cerr << "Renderer::Init failed" << std::endl;
		// Orden de cierre inverso al de apertura
		Platform::Release();
		return 1;
	}

	if (!ResourceManager::Init())
	{
		std::cerr << "ResourceManager::Init failed" << std::endl;
		Renderer::Release();
		Platform::Release();
		return 1;
	}

	if (!Input::Init())
	{
		ResourceManager::Release();
		Renderer::Release();
		Platform::Release();
		return 1;
	}

	float timer = 0;
	const float DURATION = 5; // en segundos

	//-----------------------------------------------
	RenderThread renderThread;
	renderThread.start();

	BirdScene scene = BirdScene();
	if (!scene.init(&renderThread))
	{
		std::cerr << "Scene::Init failed" << std::endl;
		renderThread.stop();
		Input::Release();
		ResourceManager::Release();
		Renderer::Release();
		Platform::Release();
		return 1;
	}

	//COMANDOS:
	Command comandoClear;
	comandoClear.type = CLEAR;
	comandoClear.color = 0x3cbcfc;

	Command commandPresent;
	commandPresent.type = PRESENT;

	uint32_t frameTime = Platform::Instance()->getTimeSinceStart();
	uint32_t currentTime = Platform::Instance()->getTimeSinceStart();
	float deltaTime =  0;

	while (Platform::Instance()->tick())
	{
		currentTime = Platform::Instance()->getTimeSinceStart(); // Tiempo en ns
		deltaTime = (currentTime - frameTime) / 1000000000.0f;
		frameTime = currentTime;
		Input::Instance()->tick();

		scene.update(deltaTime);
		timer += deltaTime;

		if (timer > DURATION)
		{
			timer -= DURATION;
			std::cout << "FPS: " << 1.0f / deltaTime << std::endl;
		}

		renderThread.enqueCommand(comandoClear);

		scene.render();

		// Logica de renderizado
		renderThread.enqueCommand(commandPresent);

		while (renderThread.getFramesLeft() > DISPLAY_BUFFER_NUM) {}
	}

	std::cout << "Fin Aplicaci0n" << std::endl;

	// LIBERAR MEMORIA ==> Orden de cierre inverso al de apertura
	renderThread.stop(); //paramos de renderizar
	scene.release(); //Liberamos la memoria de las imagenes ==> stop thread > release imagenes
	Input::Release();
	ResourceManager::Instance()->Release();
	Renderer::Release();
	Platform::Release();

	return 0;
}