#include "Body.h"
#include "raymath.h"

void Body::addForce(Vector2 force)
{
    // Newton's second law: a = F / m
    // Heavier bodies accelerate less from the same force
    acceleration.x += force.x / mass;
    acceleration.y += force.y / mass;
}

void Body::Step(float dt)
{
    // Semi-implicit Euler: we update velocity first, then use that
    // updated velocity to move the position. This order matters -
    // it keeps the simulation more stable than doing it the other way around.
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