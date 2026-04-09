#pragma once
#include "Body.h"
#include <vector>

struct Contact
{
	Body* bodyA;
	Body* bodyB;

	float restitution;
	float depth;
	Vector2 normal;

};

void CreateContact(std::vector<Body>& bodies, std::vector<Contact>& contacts);
void SeperateContacts(std::vector<Contact>& contacts);