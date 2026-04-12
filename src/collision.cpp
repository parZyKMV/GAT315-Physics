#include "collision.h"
#include "Random.h"

bool Intersect(const Body& bodyA, const Body& bodyB)
{
	float distance = Vector2Distance(bodyA.position, bodyB.position);
	float radius = bodyA.size + bodyB.size;

	return (distance < radius);
}


void CreateContact(std::vector<Body>& bodies, std::vector<Contact>& contacts)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		for (int j = 0; j < bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			if (bodyA.type != BodyType::Dynamic && bodyB.type != BodyType::Dynamic) continue;

			if (Intersect(bodyA, bodyB))
			{
				Contact contact;
				contact.bodyA = &bodyA;
				contact.bodyB = &bodyB;

				Vector2 direction = bodyA.position - bodyB.position;
				float distanceSqr = Vector2LengthSqr(direction);

				if (distanceSqr <= EPSILON) // if the bodies are exactly on top of each other, we can't calculate a normal
				{
					direction = Vector2{ GetRandomFloat(-0.05f,0.05f),GetRandomFloat(-0.05f,0.05f) };
					distanceSqr = Vector2LengthSqr(direction);
				}

				float distance = sqrtf(distanceSqr);
				float radius = bodyA.size + bodyB.size;
				contact.depth = radius - distance;
				contact.normal = Vector2Normalize(direction);
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
		Vector2 separation = contact.normal * (contact.depth/totalInverseMass);
		contact.bodyA->position = contact.bodyA->position + (separation * contact.bodyA->inverseMass);
		contact.bodyB->position = contact.bodyB->position - (separation * contact.bodyB->inverseMass);
	}
}

void ResolveContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		// compute relative velocity
		Vector2 rv = contact.bodyA->velocity - contact.bodyB->velocity;
		// project relative velocity onto the contact normal
		float nv = Vector2DotProduct(rv, contact.normal);

		// skip if bodies are separating
		if (nv > 0) continue;

		// total inverse mass = (1/mA + 1/mB)
		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;
		// impulse scalar = -(1 + restitution) * vn / (1/mA + 1/mB)
		float impulseMagnitude = -(1 + contact.restitution) * nv / totalInverseMass;

		// impulse vector along contact normal
		Vector2 impulse = contact.normal * impulseMagnitude;

			// apply equal and opposite impulses
			contact.bodyA->addForce(impulse,ForceMode::Impulse);
		contact.bodyB->addForce({-impulse.x,-impulse.y},ForceMode::Impulse);
	}
}
