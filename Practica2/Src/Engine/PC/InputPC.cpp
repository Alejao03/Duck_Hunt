#include "InputPC.h"
#include <cassert>
#include <deque>
#include <SDL.h>
#include <iostream>
#include "Platform.h"

InputPC* InputPC::instance_ = nullptr;

InputPC::InputPC() :
	mouseLeftButtonPressed(false), mouseRightButtonPressed(false), mouseMotionX(0), mouseMotionY(0),
	leftArrow(false), rightArrow(false), upArrow(false), downArrow(false), rightShiftPressed(false), rightControlPressed(false),
	controllerMap()
{
	iea_ =
	{
		0,0, //Joystick
		false,false, //Botones
		false,false, //Gatillos
		{ 0, 0, 0 } //Angulo
	};
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

	PlatformPC::Instance()->addInputListener(this);
}

InputPC::~InputPC()
{
	closeAllGameControllers();
	PlatformPC::Instance()->removeInputListener(this);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

bool InputPC::Init()
{
	assert(instance_ == nullptr);

	instance_ = new InputPC();

	return true;
}

void InputPC::Release()
{
	if (instance_)
		delete instance_;
	instance_ = nullptr;
}

InputPC* InputPC::Instance()
{
	return instance_;
}

void InputPC::tick()
{
	setUpTick();

	for (const SDL_Event& e : events) {
		auto key = e.key.keysym.sym;

		switch (e.type)
		{

			// Gestion del  gamepad
		case SDL_JOYDEVICEADDED:
			onGameControllerAdded(e.jdevice.which);
			break;
		case SDL_JOYDEVICEREMOVED:
			onGameControllerRemoved(e.jdevice.which);
			break;

			// Gamepad input
		case SDL_JOYBUTTONDOWN:
			setGamepadButtonInput(e, true);
			break;
		case SDL_JOYBUTTONUP:
			setGamepadButtonInput(e, false);
			break;
		case SDL_JOYAXISMOTION:
			setGamepadAxisMotion(e);
			break;

			// Mouse (motion y botones)
		case SDL_MOUSEMOTION:
			setMouseMotion(e.motion);
			break;
		case SDL_MOUSEBUTTONDOWN:
			setMouseButtonInput(e.button.button, true);
			break;
		case SDL_MOUSEBUTTONUP:
			setMouseButtonInput(e.button.button, false);
			break;

			// Teclado, por si alguien quiere jugar así
		case SDL_KEYDOWN:
			setKeyboardInput(key, true);
			break;
		case SDL_KEYUP:
			setKeyboardInput(key, false);
			break;

			// Otros eventos que no nos importan
		default:
			break;
		}
	}

	events.clear();

	// Con los datos obtenidos, actualizar la estructura iea
	setInputAfterTick();
}

void InputPC::setKeyboardInput(const SDL_Keycode& key, const bool pressed)
{
	switch (key) {
	case SDLK_LEFT:
		leftArrow = pressed;
		break;
	case SDLK_RIGHT:
		rightArrow = pressed;
		break;
	case SDLK_UP:
		downArrow = pressed;
		break;
	case SDLK_DOWN:
		upArrow = pressed;
		break;

		// Cualquiera de estos botones cuenta como boton de disparo
	case SDLK_RSHIFT:
		rightShiftPressed = pressed;
	case SDLK_RCTRL:
		rightControlPressed = pressed;
		break;
	default:
		// Nunca deberia llegar aqui
		break;
	}
}

void InputPC::setMouseButtonInput(Uint8 button, bool pressed)
{
	if (button == SDL_BUTTON_LEFT)
		mouseLeftButtonPressed = pressed;
	else if (button == SDL_BUTTON_RIGHT)
		mouseRightButtonPressed = pressed;
}

void InputPC::setMouseMotion(const SDL_MouseMotionEvent& mouseMotion)
{
	mouseMotionX = (float)mouseMotion.xrel / MOUSE_MOTION_ADJUSTMENT;
	mouseMotionY = (float)mouseMotion.yrel / MOUSE_MOTION_ADJUSTMENT;
}

void InputPC::setGamepadButtonInput(const SDL_Event& event, const bool pressed)
{
	auto button = event.cbutton.button;

	if (button == SDL_CONTROLLER_BUTTON_A)
		controllerMap[event.jdevice.which]->buttonXPressed = pressed;

	if (button == SDL_CONTROLLER_BUTTON_B)
		controllerMap[event.jdevice.which]->buttonOPressed = pressed;
}

void InputPC::setGamepadAxisMotion(const SDL_Event& event)
{
	auto value = std::abs(event.caxis.value) > umbral ? (float)event.caxis.value / halfMax16 : 0;
	Gamepad* gamepad = controllerMap[event.jdevice.which];

	switch (event.caxis.axis)
	{
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
		gamepad->triggerLeft = value;
		break;
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
		gamepad->triggerRight = value;
		break;
	case SDL_CONTROLLER_AXIS_LEFTX:
		gamepad->leftAxisX = value;
		break;
	case SDL_CONTROLLER_AXIS_LEFTY:
		gamepad->leftAxisY = value;
	}
}

bool InputPC::getXButtonCompositeValue()
{
	bool compositeValue = false;

	for (auto it = controllerMap.begin(); it != controllerMap.end(); ++it)
		compositeValue |= it->second->buttonXPressed;

	return compositeValue;
}

bool InputPC::getYButtonCompositeValue()
{
	bool compositeValue = false;

	for (auto it = controllerMap.begin(); it != controllerMap.end(); ++it)
		compositeValue |= it->second->buttonOPressed;

	return compositeValue;
}

float InputPC::getLeftAxisXCompositeValue()
{
	float compositeValue = 0.0f;

	for (auto it = controllerMap.begin(); it != controllerMap.end(); ++it)
		compositeValue += it->second->leftAxisX;

	return compositeValue;
}

float InputPC::getLeftAxisYCompositeValue()
{
	float compositeValue = 0.0f;

	for (auto it = controllerMap.begin(); it != controllerMap.end(); ++it)
		compositeValue += it->second->leftAxisY;

	return compositeValue;
}

float InputPC::getTriggerLeftValueCompositeValue()
{
	float compositeValue = 0.0f;

	for (auto it = controllerMap.begin(); it != controllerMap.end(); ++it)
		compositeValue += it->second->triggerLeft;

	return compositeValue;
}

float InputPC::getTriggerRightValueCompositeValue()
{
	float compositeValue = 0.0f;

	for (auto it = controllerMap.begin(); it != controllerMap.end(); ++it)
		compositeValue += it->second->triggerRight;

	return compositeValue;
}

void InputPC::setInputAfterTick()
{
	iea_.motionX = mouseMotionX;
	iea_.motionY = mouseMotionY;

	// Acumulativo
	if (leftArrow)
		iea_.motionX += -1;

	if (rightArrow)
		iea_.motionX += 1;

	if (upArrow)
		iea_.motionY += 1;

	if (downArrow)
		iea_.motionY += -1;

	// Botones de disparo
	iea_.x = rightShiftPressed || mouseLeftButtonPressed || getXButtonCompositeValue();
	iea_.o = rightControlPressed || mouseRightButtonPressed || getYButtonCompositeValue();

	// Gamepad sobrescribe teclado y raton
	iea_.motionX += getLeftAxisXCompositeValue();
	iea_.motionY += getLeftAxisYCompositeValue();

	iea_.l2 += getTriggerLeftValueCompositeValue();
	iea_.r2 += getTriggerRightValueCompositeValue();
}

void InputPC::onGameControllerAdded(uint32_t id)
{
	SDL_GameController* controller = SDL_GameControllerOpen(id);
	if (controller != nullptr)
	{
		SDL_Joystick* j = SDL_GameControllerGetJoystick(controller);
		SDL_JoystickID joyId = SDL_JoystickInstanceID(j);
		controllerMap.emplace(joyId, new Gamepad(controller));
	}
}

void InputPC::onGameControllerRemoved(uint32_t id)
{
	SDL_JoystickID joyId = SDL_JoystickGetDeviceInstanceID(id);
	auto controllerIt = controllerMap.find(joyId);
	if (controllerIt != controllerMap.end())
	{
		Gamepad* gamepad = controllerIt->second;
		SDL_GameControllerClose(gamepad->controller);
		delete gamepad;
		controllerMap.erase(controllerIt);
	}
}

void InputPC::closeAllGameControllers()
{
	for (auto it = controllerMap.begin(); it != controllerMap.end(); ++it)
	{
		SDL_GameControllerClose(it->second->controller);
		delete it->second;
	}

	controllerMap.clear();
}

void InputPC::setUpTick()
{
	iea_ = {
		0,0, //Joystick
		false,false, //Botones
		false,false, //Gatillos
		{ 0, 0, 0 } //Angulo
	};

	mouseMotionX = 0;
	mouseMotionY = 0;
}

InputState InputPC::GetInputState()
{
	return iea_;
}

void InputPC::receive(const SDL_Event& sdlEvent)
{
	this->events.push_back(sdlEvent);
}