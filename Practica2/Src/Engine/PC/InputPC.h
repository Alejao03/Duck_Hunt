#pragma once
// Input no se incluye en Renderer/Platform

#include "InputState.h"
#include "SDLEventListener.h"
#include <vector>
#include <cstdint>
#include <unordered_map>

// Para que no se mueva demasiado rapido como pasaba antes, asi funciona más similar a 
// PS4 con los mismos valores de velocidad en cursor
# define MOUSE_MOTION_ADJUSTMENT 14

struct _SDL_GameController;
struct SDL_MouseMotionEvent;
typedef int32_t SDL_Keycode;
typedef uint8_t Uint8;
typedef struct _SDL_GameController SDL_GameController;
union SDL_Event;

class InputPC : public SDLEventListener
{
	struct Gamepad
	{
	public:
		Gamepad(SDL_GameController* controller) : controller(controller), buttonOPressed(false), buttonXPressed(false),
		leftAxisX(0), leftAxisY(0), triggerLeft(0), triggerRight(0) {}

		SDL_GameController* controller;
		bool buttonOPressed, buttonXPressed;
		float leftAxisX, leftAxisY, triggerLeft, triggerRight;
	};

private:
	InputState iea_;
	static InputPC* instance_;
	InputPC();
	~InputPC();
	std::unordered_map<uint32_t, Gamepad*> controllerMap;
	std::vector<SDL_Event> events;
	const uint16_t umbral = 10000;
	const uint16_t halfMax16 = UINT16_MAX / 2;

	// Input parameters
	// Necesitamos guardar todos los valores para no tener que hacer encuesta, al guardar los valores
	// sabemos que hubo en la iteración anterior, y si no hay eventos no perdermos tiempo haciendo encuesta

	// Raton
	bool mouseLeftButtonPressed, mouseRightButtonPressed;
	float mouseMotionX, mouseMotionY;

	// Teclado
	bool leftArrow, rightArrow, upArrow, downArrow, rightShiftPressed, rightControlPressed;
	
	// Reiniciar la estructura InputState iea y tambien los valores de mouse motion
	// debido a que no hay ningún evento que lo anule a diferencia de button down - button up
	// asi que tenemos que resetearlo nosotros en cada iteración
	void setUpTick();

	// Control de gamepad
	void onGameControllerAdded(uint32_t id);
	void onGameControllerRemoved(uint32_t id);
	void closeAllGameControllers();

	// Utilidades
	void setKeyboardInput(const SDL_Keycode& key, const bool pressed);
	void setMouseButtonInput(const Uint8 button, const bool pressed);
	void setMouseMotion(const SDL_MouseMotionEvent& mouseMotion);
	void setGamepadButtonInput(const SDL_Event& event, const bool pressed);
	void setGamepadAxisMotion(const SDL_Event& event);

	// Controlar gamepads
	bool getXButtonCompositeValue();
	bool getYButtonCompositeValue();
	float getLeftAxisXCompositeValue();
	float getLeftAxisYCompositeValue();
	float getTriggerLeftValueCompositeValue();
	float getTriggerRightValueCompositeValue();

	// Para asignar botones del ratón y de teclado tras leer el input y actualizar variables
	// El estado de iea.x e iea.o tambien lo mantenemos y ya se encargara el cursor de tenerlo en cuenta
	// solo el primer frame en que se pulso
	void setInputAfterTick();
public:
	static bool Init();
	static void Release();
	static InputPC* Instance();
	void tick();
	void receive(const SDL_Event& event) override;
	InputState GetInputState(); //Devolver el estado del mando PS4
};
