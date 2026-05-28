#include "Effector.h"

void Effector::Draw()
{
	// Draw the effector as a circle
	DrawCircleLinesV(position, size, WHITE);
}

void Effector::CollectBodiesInside(std::vector<Body>& ibodies, std::vector<Body*>& obodies)
{
	for(auto& body : ibodies)
	{
		if (Vector2Distance(body.position, position) <= (size * size))
		{
			obodies.push_back(&body);
		}
	}
}
