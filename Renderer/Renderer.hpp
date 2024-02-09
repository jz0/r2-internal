#pragma once
#include "../Include.hpp"

namespace Renderer
{
	void DrawRect(int x, int y, int w, int h, Color color);
	void DrawOutlinedRect(int x, int y, int w, int h, Color color);
	void DrawLine(int x0, int y0, int x1, int y1, Color color);
	void DrawString(int x, int y, Color color, const char* str, ...);
}