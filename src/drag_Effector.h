#pragma once
#include "Effector.h"

class DragEffector : public Effector
{
public:
	DragEffector(Vector2 position, float size, float drag)
		:
		Effector(position, size),
		drag{ drag }
	{
	}

	// Slows down all bodies inside the area - force is opposite to velocity (Fd = -kv)
	void Apply(std::vector<Body>& bodies) override;

	void Draw() override;

private:
	float drag; // drag coefficient k - higher = stronger slowdown
};