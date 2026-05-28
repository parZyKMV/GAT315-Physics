#include "area_Effector.h"
#include "raymath.h"

void AreaEffector::Apply(std::vector<Body>& ibodies)
{
	// First collect only bodies inside the circle
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);

	for (auto& body : bodies)
	{
		// Area effector applies a constant force in a fixed direction defined by angle
		// Formula: F = d * s  where d is normalized direction from angle, s is strength
		Vector2 direction = { cosf(angle), sinf(angle) };
		Vector2 force = { direction.x * forceMagnitude, direction.y * forceMagnitude };
		body->addForce(force);
	}
}

void AreaEffector::Draw()
{
	Effector::Draw();
	DrawCircleV(position, size, Fade(YELLOW, 0.02f));

	// Draw an arrow showing the direction the force pushes toward
	Vector2 arrowEnd = { position.x + cosf(angle) * size * 0.5f,
						 position.y + sinf(angle) * size * 0.5f };
	DrawLineV(position, arrowEnd, YELLOW);
}