#pragma once
#include "../Include.hpp"

#define METERS_TO_INCHES 39.3701f
#define INCHES_TO_METERS 0.0254f

namespace ESP
{
	bool WorldToScreen(Vector Src, Vector& Dst);
	void Run();
}