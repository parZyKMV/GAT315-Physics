#include "Gravitational_Effector.h"
#include "raymath.h"

void GravitationalEffector::Apply(std::vector<Body>& bodies)
{
	// Collect only bodies inside this effector's circle
	std::vector<Body*> inside;
	CollectBodiesInside(bodies, inside);

	// Apply gravitational attraction between every pair inside the region
	for (size_t i = 0; i < inside.size(); i++)
	{
		for (size_t j = i + 1; j < inside.size(); j++)
		{
			Body* bodyA = inside[i];
			Body* bodyB = inside[j];

			// STEP 1: Direction vector from B to A
			Vector2 direction = { bodyA->position.x - bodyB->position.x,
								  bodyA->position.y - bodyB->position.y };

			// STEP 2: Distance between the two bodies
			float distance = Vector2Length(direction);

			// STEP 3: Clamp distance so bodies don't explode when too close together
			distance = fmaxf(distance, 1.0f);

			// STEP 4: F = G * mA * mB / d^2
			float forceMagnitude = strength * (bodyA->mass * bodyB->mass) / (distance * distance);

			// STEP 5: Scale normalized direction by force magnitude
			Vector2 normalized = Vector2Normalize(direction);
			Vector2 force = { normalized.x * forceMagnitude, normalized.y * forceMagnitude };

			// STEP 6: Apply equal and opposite forces (Newton's third law)
			bodyA->addForce({ -force.x, -force.y }); // A gets pulled toward B
			bodyB->addForce(force);                   // B gets pulled toward A
		}
	}
}