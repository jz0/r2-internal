#include "Renderer.hpp"

void Renderer::DrawRect(int x, int y, int w, int h, Color color)
{
	MatSystemSurface->DrawSetColorRGBA(color.r, color.g, color.b, color.a);
	MatSystemSurface->DrawFilledRect(x, y, x + w, y + h);
}

void Renderer::DrawOutlinedRect(int x, int y, int w, int h, Color color)
{
	MatSystemSurface->DrawSetColorRGBA(color.r, color.g, color.b, color.a);
	MatSystemSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Renderer::DrawLine(int x0, int y0, int x1, int y1, Color color)
{
	MatSystemSurface->DrawSetColorRGBA(color.r, color.g, color.b, color.a);
	MatSystemSurface->DrawLine(x0, y0, x1, y1);
}

void Renderer::DrawString(int x, int y, Color color, const char* str, ...)
{
	MatSystemSurface->DrawColoredTextWrapper(5, x, y, color.r, color.g, color.b, color.a, str);
}