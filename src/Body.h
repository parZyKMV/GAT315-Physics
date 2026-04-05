#pragma once

#include "raylib.h"
#include "raymath.h"

struct Body
{
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float mass;
	float size;	
	float restitution;
	Color color;

	void addForce(Vector2 force);

	void Step(float dt);

	void Draw() const;
};
