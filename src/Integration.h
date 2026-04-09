#pragma once
#include "Body.h"

// Explicit Euler: updates position first, then velocity.
// Less stable than semi-implicit but kept here for reference.
inline void ExplicitEuler(Body& body, float dt)
{
	body.position.x += body.velocity.x * dt;
	body.position.y += body.velocity.y * dt;

	body.velocity.x += body.acceleration.x * dt;
	body.velocity.y += body.acceleration.y * dt;

	// Damping slows the body down over time - must apply to both axes
	float damp = 1.0f / (1.0f + (body.damping * dt));
	body.velocity.x *= damp;
	body.velocity.y *= damp;
}

// Semi-implicit Euler: updates velocity first, then position with the new velocity.
// More stable than explicit - this is the one we actually use.
inline void SemiImplicitEuler(Body& body, float dt)
{
	body.velocity.x += body.acceleration.x * dt;
	body.velocity.y += body.acceleration.y * dt;

	// Damping slows the body down over time - must apply to both axes
	float damp = 1.0f / (1.0f + (body.damping * dt));
	body.velocity.x *= damp;
	body.velocity.y *= damp;

	body.position.x += body.velocity.x * dt;
	body.position.y += body.velocity.y * dt;
}