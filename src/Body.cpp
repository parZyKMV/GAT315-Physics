#include "Body.h"
#include "raymath.h"

void Body::addForce(Vector2 force, ForceMode forceMode)
{
    // Only dynamic bodies react to forces - static and kinematic ignore them
    if (type != BodyType::Dynamic) return;

    switch (forceMode)
    {
    case ForceMode::Default:
    case ForceMode::Force:
        // Standard force: scales by inverse mass so heavier bodies accelerate less (F = ma)
        acceleration.x += force.x * inverseMass;
        acceleration.y += force.y * inverseMass;
        break;

    case ForceMode::Impulse:
        // Instantly changes velocity scaled by inverse mass - good for explosions or jumps
        velocity.x += force.x * inverseMass;
        velocity.y += force.y * inverseMass;
        break;

    case ForceMode::Acceleration:
        // Adds directly to acceleration regardless of mass - all bodies react equally
        acceleration.x += force.x;
        acceleration.y += force.y;
        break;

    case ForceMode::VelocityChange:
        // Instantly changes velocity regardless of mass - like teleporting the velocity
        velocity.x += force.x;
        velocity.y += force.y;
        break;
    }
    // NOTE: no default apply after the switch - each ForceMode handles it on its own
}

void Body::Step(float dt)
{
    // Semi-implicit Euler: update velocity first, then position with the new velocity.
    // This order keeps the simulation more stable than explicit Euler.
    velocity.x += acceleration.x * dt;
    velocity.y += acceleration.y * dt;

    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    // Clear acceleration so forces don't keep stacking between frames
    acceleration = { 0, 0 };
}

void Body::Draw() const
{
    DrawCircleV(position, size, color);
}