#pragma once

struct Color
{
	Color() {}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { this->r = r; this->g = g; this->b = b; this->a = a; }
	uint8_t r, g, b, a;
};

class VPanelWrapper
{
public:

};

inline VPanelWrapper* PanelWrapper;

class CMatSystemSurface
{
public:
	uintptr_t GetEmbeddedPanel()
	{
		return Memory::CallVirtual<9, uintptr_t>(this);
	}

	void PushMakeCurrent(uintptr_t panel, bool useInSets)
	{
		return Memory::CallVirtual<11, void>(this, panel, useInSets);
	}

	void PopMakeCurrent(uintptr_t panel)
	{
		return Memory::CallVirtual<12, void>(this, panel);
	}

	void DrawSetColorRGBA(char r, char g, char b, int a)
	{
		return Memory::CallVirtual<14, void>(this, r, g, b, a);
	}	
	
	void DrawSetColor(Color color)
	{
		return Memory::CallVirtual<15, void>(this, color);
	}

	void DrawFilledRect(int x, int y, int w, int h)
	{
		return Memory::CallVirtual<16, void>(this, x, y, w, h);
	}
	
	void DrawOutlinedRect(int x, int y, int w, int h)
	{
		return Memory::CallVirtual<19, void>(this, x, y, w, h);
	}
		
	void DrawLine(int x, int y, int w, int h)
	{
		return Memory::CallVirtual<20, void>(this, x, y, w, h);
	}

	__int64 DrawColoredTextWrapper(int font, int x, int y, int r, int g, int b, int a, const char* fmt, ...)
	{
		return Memory::CallVirtual<171, __int64>(this, font, x, y, r, g, b, a, fmt);
	}

	int GetScreenSize(int* width, int* height)
	{
		return Memory::CallVirtual<48, int>(this, width, height);
	}
};

inline CMatSystemSurface* MatSystemSurface;