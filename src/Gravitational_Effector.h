#pragma once
#include "Effector.h"

class GravitationalEffector : public Effector
{
public:
	// Takes position and size so it can be placed anywhere in the scene
	GravitationalEffector(Vector2 position, float size, float strength)
		: Effector(position, size), strength(strength)
	{
	}

	// Attracts every pair of bodies toward each other based on their masses
	void Apply(std::vector<Body>& bodies) override;

private:
	float strength; // acts as the gravitational constant G
};