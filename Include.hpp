#pragma once
#include <Windows.h>
#include <intrin.h>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <algorithm>

inline FILE* IO;
inline uintptr_t BaseAddress;
inline uintptr_t ClientPtr;
inline uintptr_t EnginePtr;
inline uintptr_t MatSystemSurfacePtr;
inline uintptr_t VGUI2Ptr;
inline uintptr_t InputSystemPtr;
inline uintptr_t LocalPlayerPtr;
inline uintptr_t EntityListPtr;
inline uintptr_t ViewRenderPtr;

//Utilities
#include "Utilities/Memory.hpp"
#include "Utilities/Utilities.hpp"
#include "Utilities/LazyImporter.hpp"
#include "Utilities/MinHook/MinHook.h"

//SDK
#include "R2SDK/R2SDK.hpp"

//Hooks
#include "Hooks/Paint.hpp"
#include "Hooks/CreateMove.hpp"
#include "Hooks/Screenshot.hpp"