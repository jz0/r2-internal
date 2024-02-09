#pragma once

namespace Memory
{
	template<unsigned int IIdx, typename TRet, typename ... TArgs>
	static auto CallVirtual(void* ThisPtr, TArgs ... ArgList) -> TRet
	{
		using Fn = TRet(__thiscall*)(void*, decltype(ArgList)...);
		return (*static_cast<Fn**>(ThisPtr))[IIdx](ThisPtr, ArgList...);
	}
}