#pragma once

#include "raylib.h"

// Basic 0-1 random float, used as the base for the other two
inline float GetRandomFloat()
{
	return GetRandomValue(0, 10000) / (float)10000;
}

// Handy when you just need a value between 0 and some max
inline float GetRandomFloat(float max)
{
	return GetRandomFloat() * max;
}

// Most flexible version - give it a range and it picks something inside it
inline float GetRandomFloat(float min, float max)
{
	return min + GetRandomFloat() * (max - min); // must add min so range starts at min, not 0
}