#pragma once
#include <vector>
#include "Body.h"

enum class EffectorType
{
	Gravitation,
	Point,
	Area,
	Drag
};

class Effector
{
public:
	Effector(Vector2 position, float size) : position{ position }, size{ size } {}

	virtual void Apply(std::vector<Body>& bodies) = 0;
	virtual void Draw();

	void CollectBodiesInside(std::vector<Body>& ibodies, std::vector<Body*>& obodies);

protected:
	Vector2 position;
	float size;
};

