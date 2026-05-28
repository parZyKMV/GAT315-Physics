#include "Spring.h"

void Spring::Apply(float multiplier)
{

}

void Spring::Draw()
{

}

Vector2 Spring::GetSpringForce(Vector2 positionA, Vector2 positionB, float restlenght, float stiffness)
{
	Vector2 direction = positionA - positionB;
	float length = Vector2Length(direction);
	float displacement = length - restlenght;
	float magnitude = (displacement) * stiffness;

	return Vector2Normalize(direction) * magnitude;
}