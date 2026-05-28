#include "collision.h"
#include "Random.h"

#define EPSILON 0.0001f

bool Intersect(const Body& bodyA, const Body& bodyB)
{
	float distance = Vector2Distance(bodyA.position, bodyB.position);
	float radius = bodyA.size + bodyB.size;
	return (distance < radius);
}

void CreateContact(std::vector<Body>& bodies, std::vector<Contact>& contacts)
{
	for (int i = 0; i < (int)bodies.size(); i++)
	{
		// j starts at i+1 so we never test a body against itself
		// and we never process the same pair twice
		for (int j = i + 1; j < (int)bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			if (bodyA.type != BodyType::Dynamic && bodyB.type != BodyType::Dynamic) continue;

			if (Intersect(bodyA, bodyB))
			{
				Contact contact;
				contact.bodyA = &bodyA;
				contact.bodyB = &bodyB;

				Vector2 direction = { bodyA.position.x - bodyB.position.x,
									  bodyA.position.y - bodyB.position.y };
				float distanceSqr = Vector2LengthSqr(direction);

				// If bodies are exactly on top of each other we can't calculate a normal,
				// so nudge them apart with a tiny random direction
				if (distanceSqr <= EPSILON)
				{
					direction = Vector2{ GetRandomFloat(-0.05f, 0.05f), GetRandomFloat(-0.05f, 0.05f) };
					distanceSqr = Vector2LengthSqr(direction);
				}

				float distance = sqrtf(distanceSqr);
				float radius = bodyA.size + bodyB.size;
				contact.depth = radius - distance;
				contact.normal = Vector2Normalize(direction);

				// Average the two restitutions so bounciness blends between materials
				contact.restitution = (bodyA.restitution + bodyB.restitution) * 0.5f;

				contacts.push_back(contact);
			}
		}
	}
}

void SeperateContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;
		if (totalInverseMass <= 0) continue; // both static, nothing to separate

		// Each body moves proportionally to its inverse mass - lighter bodies move more
		float separationScale = contact.depth / totalInverseMass;
		Vector2 separation = { contact.normal.x * separationScale,
							   contact.normal.y * separationScale };

		contact.bodyA->position.x += separation.x * contact.bodyA->inverseMass;
		contact.bodyA->position.y += separation.y * contact.bodyA->inverseMass;

		contact.bodyB->position.x -= separation.x * contact.bodyB->inverseMass;
		contact.bodyB->position.y -= separation.y * contact.bodyB->inverseMass;
	}
}

void ResolveContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		// Compute relative velocity between the two bodies
		Vector2 rv = { contact.bodyA->velocity.x - contact.bodyB->velocity.x,
					   contact.bodyA->velocity.y - contact.bodyB->velocity.y };

		// Project relative velocity onto the contact normal
		float nv = Vector2DotProduct(rv, contact.normal);

		// If bodies are already moving apart, no impulse needed
		if (nv > 0) continue;

		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;
		if (totalInverseMass <= 0) continue; // both static, nothing to resolve

		// Impulse scalar = -(1 + restitution) * relativeVelocityAlongNormal / totalInverseMass
		float impulseMagnitude = -(1.0f + contact.restitution) * nv / totalInverseMass;

		// Impulse vector points along the contact normal
		Vector2 impulse = { contact.normal.x * impulseMagnitude,
							contact.normal.y * impulseMagnitude };

		// Apply equal and opposite impulses - A gets pushed one way, B the other
		contact.bodyA->addForce(impulse, ForceMode::Impulse);
		contact.bodyB->addForce({ -impulse.x, -impulse.y }, ForceMode::Impulse);
	}
}