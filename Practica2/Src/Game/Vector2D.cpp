#include "Vector2D.h"
#define _USE_MATH_DEFINES
#include <math.h>

Vector2D Vector2D::operator+(const Vector2D& vector) const { return { x + vector.x, y + vector.y }; }

Vector2D Vector2D::operator-(const Vector2D& vector) const { return { x - vector.x, y - vector.y }; }

Vector2D Vector2D::operator+=(const Vector2D vector) { return { x += vector.x, y += vector.y }; }

float Vector2D::operator*(const Vector2D& vector) const { return vector.x * x + vector.y * y; }

Vector2D Vector2D::operator*(float number) const { return { x * number , y * number }; }

void Vector2D::operator*=(float number) { x *= number; y *= number; }

bool Vector2D::operator==(const Vector2D& vector) const { return (x == vector.x && y == vector.y); }

bool Vector2D::operator!=(const Vector2D& vector) const { return (x != vector.x || y != vector.y); }

void Vector2D::operator=(const Vector2D& vector) { x = vector.x; y = vector.y; }

// Returns the inverted vector
Vector2D Vector2D::inverted() const { return *this * -1; }

// Returns the normalized vector 
Vector2D Vector2D::normalized() const
{
	if (this->x == 0 && this->y == 0) return { 0,0 };
	float module = magnitude();
	return { (this->x / module), (this->y / module) };
}

// Returns the magnitude of the vector
float Vector2D::magnitude() const
{
	return hypot(x, y);
}

// Rotate the vector by the given angle in radians
void Vector2D::rotate(float angle)
{
	float s = sin(angle);
	float c = cos(angle);
	float x = this->x * c - this->y * s;
	float y = this->x * s + this->y * c;
	this->x = x;
	this->y = y;
}

// Rotate the vector by the given angle in degrees
void Vector2D::rotateDegrees(float degreeAngle)
{
	rotate(degreeAngle * (3.14f / 180.0f));
}

// Returns the angle of the vector in degrees
float Vector2D::angle() const
{
	return atan2(y, x) * 180 / M_PI;
}