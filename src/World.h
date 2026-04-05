#pragma once

#include "raylib.h"
#include "Body.h"
#include <vector>

struct World
{
	std::vector<Body> bodies;
	Vector2 gravity{ 0, 100.0f };

	void AddBody(const Body& body);

	void Step(float dt);

	void Draw() const;

	void ApplyRadialForce(Vector2 origin, float radius, float strength);

};
