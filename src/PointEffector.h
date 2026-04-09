#pragma once
#include "Effector.h"

class PointEffector : public Effector
{
public:
	PointEffector(Vector2 position, float size, float forceMagnitude)
		: position{ position },
		size{ size },
		forceMagnitude{ forceMagnitude }
	{}

	//Pushes or pulls bodies within range toward/away from the point
	void Apply(std::vector<Body>& bodies) override;

	//
	void Draw() override;

private:
	Vector2 position;
	float size;
	float forceMagnitude;
};
