#pragma once
#include <vector>
#include "Body.h"

class Effector
{
public:
	virtual void Apply(std::vector<Body>& bodies) = 0;
	virtual void Draw() {}
};

