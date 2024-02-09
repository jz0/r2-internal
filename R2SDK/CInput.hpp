#pragma once
#include "CUserCmd.hpp"

class CInput
{
public:
	CUserCmd* GetUserCmd(int slot, int sequence_number)
	{
		return Memory::CallVirtual<9, CUserCmd*>(this, slot, sequence_number);
	}
};

inline CInput* Input;