#include "PointEffector.h"
#include "raymath.h"

void PointEffector::Apply(std::vector<Body>& bodies)
{
	for (auto& body : bodies)
	{
		// Direction from the effector point toward the body
		Vector2 direction = { body.position.x - position.x, body.position.y - position.y };

		// Only affect bodies within the effector's range
		if (Vector2Length(direction) <= size)
		{
			// Can't use * operator on Vector2 in Raylib C++ without raymath operators
			Vector2 norm = Vector2Normalize(direction);
			Vector2 force = { norm.x * forceMagnitude, norm.y * forceMagnitude };
			body.addForce(force);
		}
	}
}

void PointEffector::Draw()
{
	// Show the influence radius in red so it's visible during the simulation
	DrawCircleLinesV(position, size, RED);
}