#pragma once
#include "Effector.h"

class AreaEffector : public Effector
{
public:
	// angle in radians - determines the direction of the constant force
	AreaEffector(Vector2 position, float size, float forceMagnitude, float angle = 0.0f)
		:
		Effector(position, size),
		forceMagnitude{ forceMagnitude },
		angle{ angle }
	{
	}

	// Applies a constant directional force to all bodies inside the area
	void Apply(std::vector<Body>& bodies) override;

	void Draw() override;

private:
	float forceMagnitude;
	float angle; // direction of the force in radians (0 = right, PI/2 = down)
};