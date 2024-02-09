#include "Utilities.hpp"

#define IN_RANGE(x, a, b) (x >= a && x <= b)
#define GET_BITS(x) (IN_RANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (IN_RANGE(x, '0', '9') ? x - '0': 0))
#define GET_BYTE(x) (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))

uintptr_t Utilities::FindPattern(uintptr_t Base, const char* Sig)
{
	const auto DOSHeader = PIMAGE_DOS_HEADER(Base);
	const auto NTHeaders = PIMAGE_NT_HEADERS(reinterpret_cast<std::uint8_t*>(Base) + DOSHeader->e_lfanew);
	uintptr_t Size = (Base + (uintptr_t)NTHeaders->OptionalHeader.SizeOfImage);

	const char* Pat = Sig;
	uintptr_t FirstMatch = 0;
	for (uintptr_t Cur = Base; Cur < Size; Cur++)
	{
		if (!*Pat)
			return FirstMatch;

		if (*(PBYTE)Pat == ('\?') || *(BYTE*)Cur == GET_BYTE(Pat))
		{
			if (!FirstMatch)
				FirstMatch = Cur;

			if (!Pat[2])
				return FirstMatch;

			if (*(PWORD)Pat == ('\?\?') || *(PBYTE)Pat != ('\?'))
				Pat += 3;

			else Pat += 2;
		}
		else
		{
			Pat = Sig;
			FirstMatch = 0;
		}
	}

	return NULL;
}