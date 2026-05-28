#pragma once
#include "Effector.h"

class PointEffector : public Effector
{
public:
	PointEffector(Vector2 position, float size, float forceMagnitude)
		:
		Effector(position, size),
		forceMagnitude{ forceMagnitude }
	{}

	//Pushes or pulls bodies within range toward/away from the point
	void Apply(std::vector<Body>& bodies) override;

	//
	void Draw() override;

private:
	
	float forceMagnitude;
};
