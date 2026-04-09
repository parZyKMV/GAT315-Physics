#include "Gravitational_Effector.h"
#include "raymath.h"

void GravitationalEffector::Apply(std::vector<Body>& bodies)
{
	for (size_t i = 0; i < bodies.size(); i++)
	{
		for (size_t j = i + 1; j < bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			// STEP 1: Direction vector - points from bodyB toward bodyA
			Vector2 direction = { bodyA.position.x - bodyB.position.x,
								  bodyA.position.y - bodyB.position.y };

			// STEP 2: Distance between the two bodies
			float distance = Vector2Length(direction);

			// STEP 3: Clamp distance so bodies don't explode when too close together
			distance = fmaxf(distance, 1.0f);

			// STEP 4: Force magnitude using Newton's law: F = G * mA * mB / d^2
			float forceMagnitude = strength * (bodyA.mass * bodyB.mass) / (distance * distance);

			// STEP 5: Force vector - normalize direction then scale by magnitude
			Vector2 normalized = Vector2Normalize(direction);
			Vector2 force = { normalized.x * forceMagnitude, normalized.y * forceMagnitude };

			// STEP 6: Apply equal and opposite forces (Newton's third law)
			bodyA.addForce({ -force.x, -force.y }); // A gets pulled toward B
			bodyB.addForce(force);                   // B gets pulled toward A
		}
	}
}