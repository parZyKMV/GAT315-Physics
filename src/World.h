#pragma once

#include "raylib.h"
#include "Body.h"
#include <vector>
#include "Effector.h"
#include "collision.h"

struct World
{
	std::vector<Body> bodies;
	Vector2 gravity{ 0, 100.0f };

	void AddBody(const Body& body);
	void AddEffector(Effector* effector);

	std::vector<Body>& GetBodies() { return bodies; }
	const std::vector<Body>& GetBodies() const { return bodies; }

	void Step(float dt);

	void Draw() const;

	void UpdateCollisions();

	void ApplyRadialForce(Vector2 origin, float radius, float strength);

	std::vector<class Effector*> effectors;

	std::vector<Contact> contacts;

};
