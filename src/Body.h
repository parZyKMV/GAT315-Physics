#pragma once

#include "raylib.h"
#include "raymath.h"

enum class BodyType
{
	Static,
	Kinematic,
	Dynamic
};

// Controls how a force is applied to a body
enum class ForceMode
{
	Default,        // same as Force
	Force,          // adds to acceleration scaled by inverse mass (F = ma)
	Impulse,        // instantly changes velocity scaled by inverse mass
	Acceleration,   // adds directly to acceleration, ignores mass
	VelocityChange  // instantly changes velocity, ignores mass
};

struct Body
{
	BodyType type = BodyType::Dynamic;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float mass = 1.0f;
	float inverseMass = 1.0f; // precomputed 1/mass so we avoid dividing every frame
	float size = 1.0f;
	float restitution = 1.0f;
	float gravityScale = 1.0f;
	float damping = 0.0f; // 0 means no damping, 1 means full stop
	Color color;

	void addForce(Vector2 force, ForceMode forceMode = ForceMode::Force);
	void Step(float dt);
	void Draw() const;
};