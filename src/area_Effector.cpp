#include "area_Effector.h"
#include "raymath.h"

void AreaEffector::Apply(std::vector<Body>& ibodies)
{

	std::vector<Body*> bodies;

	CollectBodiesInside(ibodies, bodies);

	for (auto& body : bodies)
	{
		// Direction from the effector point toward the body
		Vector2 direction = Vector2();

		// Only affect bodies within the effector's range
		if (Vector2Length(direction) <= size)
		{
			// Can't use * operator on Vector2 in Raylib C++ without raymath operators
			Vector2 norm = Vector2Normalize(direction);
			Vector2 force = { norm.x * forceMagnitude, norm.y * forceMagnitude };
			body->addForce(force);
		}
	}
}

void AreaEffector::Draw()
{
	// Show the influence radius in red so it's visible during the simulation
	Effector::Draw(); // Draw the base effector (e.g., a circle
	DrawCircleV(position, size, Fade(RED, 0.02f));
}