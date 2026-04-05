#include "World.h"
#include "raymath.h"

void World::AddBody(const Body& body)
{
    bodies.push_back(body);
}

void World::Step(float dt)
{
    // Apply gravity to every body - we multiply by mass so all bodies
    // fall at the same rate regardless of how heavy they are
    for (auto& body : bodies)
        body.addForce({ gravity.x * body.mass, gravity.y * body.mass });

    // Now that all forces are applied (gravity + any radial force from input),
    // integrate each body forward one step
    for (auto& body : bodies)
        body.Step(dt);  // Step() also resets acceleration at the end

    // Check if any body has gone outside the window and bounce it back.
    // We flip the relevant velocity component and reduce it by restitution
    // to simulate energy loss on impact.
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

void World::Draw() const
{
    for (const Body& body : bodies)
        body.Draw();
}

void World::ApplyRadialForce(Vector2 origin, float radius, float strength)
{
    for (auto& body : bodies)
    {
        Vector2 direction = { body.position.x - origin.x, body.position.y - origin.y };
        float length = Vector2Length(direction);

        if (length <= radius)
        {
            Vector2 normalized = Vector2Normalize(direction);
            body.addForce({ normalized.x * strength, normalized.y * strength });
        }
    }
}