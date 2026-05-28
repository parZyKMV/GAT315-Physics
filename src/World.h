#pragma once

#include "raylib.h"
#include "Body.h"
#include <vector>
#include "Effector.h"
#include "Spring.h"
#include "collision.h"

struct World
{
	std::vector<Body> bodies;

	void AddBody(const Body& body);
	void AddEffector(Effector* effector);

	std::vector<Body>& GetBodies() { return bodies; }
	const std::vector<Body>& GetBodies() const { return bodies; }

	void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }

	Body* GetBodyInteract(const Vector2& position);

	static void SetGravity(Vector2 newGravity) { gravity = newGravity; }

	void Step(float dt);

	void Draw() const;

	void UpdateCollisions();

	void ApplyRadialForce(Vector2 origin, float radius, float strength);

	std::vector<class Effector*> effectors;

	std::vector<Contact> contacts;
	static Vector2 gravity;
	std::vector<Spring*> springs;
private:

	Vector2 boundsMin{ -10.0f, -5.0f };
	Vector2 boundsMax{ 10.0f,  5.0f };

};
