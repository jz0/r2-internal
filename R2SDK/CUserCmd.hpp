#pragma once

class CUserCmd
{
public:
	int command_number;
	int tick_count;
	float command_time;
	Vector worldViewAngles;
	BYTE gap18[4];
	Vector localViewAngles;
	Vector attackangles;
	Vector move;
	int buttons;
	BYTE impulse;
	short weaponselect;
	int meleetarget;
	BYTE gap4C[24];
	char headoffset;
	BYTE gap65[11];
	Vector cameraPos;
	Vector cameraAngles;
	BYTE gap88[4];
	int tickSomething;
	int dword90;
	int predictedServerEventAck;
	int dword98;
	float frameTime;
};

typedef unsigned int CRC32_t;
class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	CRC32_t		m_crc;
};