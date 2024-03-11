#pragma once

struct Vector3 {
	float x, y, z;
};

struct InputState
{
	//Joystick Izq
	float motionX;
	float motionY;

	//X y O
	bool x;
	bool o;

	//Gatillos
	float l2;
	float r2;

	//Giroscopo
	Vector3 angle;
};
