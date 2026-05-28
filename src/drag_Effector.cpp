#include "drag_Effector.h"
#include "raymath.h"

void DragEffector::Apply(std::vector<Body>& ibodies)
{
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);

	for (auto& body : bodies)
	{
		// Drag formula: Fd = -k * v
		// Force is opposite to velocity direction - faster bodies experience more drag
		Vector2 force = { -body->velocity.x * drag,
						  -body->velocity.y * drag };
		body->addForce(force);
	}
}

void DragEffector::Draw()
{
	Effector::Draw();
	// Draw the drag area in green so it's visually distinct from other effectors
	DrawCircleV(position, size, Fade(GREEN, 0.02f));
}