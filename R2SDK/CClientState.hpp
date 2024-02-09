#pragma once

class CClientState
{
public:
	char pad_0000[152]; //0x0000
	int SignonState; //0x0098
}; //Size: 0x009C

inline CClientState* ClientState;