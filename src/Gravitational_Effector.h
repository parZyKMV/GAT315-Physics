#pragma once
#include "Effector.h"

class GravitationalEffector : public Effector
{
public:
	// Constructor name matches the class name
	GravitationalEffector(float strength) : strength(strength) {}

	// Applies gravitational attraction between every pair of bodies
	void Apply(std::vector<Body>& bodies) override;

private:
	float strength; // acts as the gravitational constant G
};