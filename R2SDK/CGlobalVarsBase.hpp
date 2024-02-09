#pragma once

class CGlobalVarsBase
{
public:
	double realtime; //0x0000
	int framecount; //0x0008
	float absoluteframetime; //0x000C
	float curtime; //0x0010
	float curtime1; //0x0014
	float curtime2; //0x0018
	float curtime3; //0x001C
	float frametime; //0x0020
	float curtime4; //0x0024
	float curtime5; //0x0028
	char pad_002C[12]; //0x002C
	int maxClients; //0x0038
	int tickcount; //0x003C
	float interval_per_tick; //0x0040
}; //Size: 0x0044

inline CGlobalVarsBase* Globals;