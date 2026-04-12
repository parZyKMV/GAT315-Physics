#include "World.h"
#include "raymath.h"
#include "Effector.h"
#include "Integration.h"


void World::AddBody(const Body& body)
{
    bodies.push_back(body);
}

void World::AddEffector(Effector* effector)
{
    effectors.push_back(effector);
}



void World::Step(float dt)
{
    // Reset acceleration at the start of every frame before applying any forces
    for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };

    // Apply gravity as acceleration so all bodies fall at the same rate regardless of mass.
    // Using ForceMode::Acceleration bypasses the mass division inside addForce.
    for (auto& body : bodies)
        body.addForce({ gravity.x * body.gravityScale,
                        gravity.y * body.gravityScale }, ForceMode::Acceleration);

    // Force effectors - must run before integration so forces affect this frame
    for (auto& effector : effectors) effector->Apply(bodies);

    // Integrate only dynamic bodies - static and kinematic don't move on their own
    for (auto& body : bodies)
    {
        if (body.type == BodyType::Dynamic) SemiImplicitEuler(body, dt);
    }

	UpdateCollisions();
    
}

void World::Draw() const
{
    // Let each effector draw itself first (e.g. the point effector circle)
    for (auto& effector : effectors) effector->Draw();

    for (const Body& body : bodies)
        body.Draw();
}

void World::UpdateCollisions()
{
	contacts.clear();
	CreateContact(bodies, contacts);
	SeperateContacts(contacts);
    ResolveContacts(contacts);

    // Bounce bodies off the window walls
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    for (auto& body : bodies)
    {
        // Right wall
        if (body.position.x + body.size > screenWidth)
        {
            body.position.x = screenWidth - body.size;
            body.velocity.x *= -body.restitution;
        }
        // Left wall
        if (body.position.x - body.size < 0)
        {
            body.position.x = body.size;
            body.velocity.x *= -body.restitution;
        }
        // Bottom wall
        if (body.position.y + body.size > screenHeight)
        {
            body.position.y = screenHeight - body.size;
            body.velocity.y *= -body.restitution;
        }
        // Top wall
        if (body.position.y - body.size < 0)
        {
            body.position.y = body.size;
            body.velocity.y *= -body.restitution;
        }
    }
}

void World::ApplyRadialForce(Vector2 origin, float radius, float strength)
{
    for (auto& body : bodies)
    {
        Vector2 direction = { body.position.x - origin.x, body.position.y - origin.y };
        float length = Vector2Length(direction);

        // Only push bodies that are close enough to the cursor
        if (length <= radius)
        {
            Vector2 normalized = Vector2Normalize(direction);
            body.addForce({ normalized.x * strength, normalized.y * strength });
        }
    }
}