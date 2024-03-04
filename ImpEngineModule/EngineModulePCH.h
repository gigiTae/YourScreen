

#ifndef IMP_ENGINE_MODULE_PCH
#define IMP_ENGINE_MODULE_PCH

#define NOMINMAX
#include <Windows.h>

/// XInput
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

/// STL
#include <vector>
#include <unordered_map>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
#include <queue>
#include <bitset>
#include <limits>

#include <filesystem>
#include <fstream>

/// RTTI
#include <typeinfo>
#include <typeindex>
#include <type_traits>

/// ImpReflection
#include "../ImpReflection/ImpReflectionHeader.h"

/// EngineModule Define
#include "TypeIndex.h"
#include "InputEnum.h"
#include "ResourceEnum.h"
#include "ColliderTypeEnum.h"
#include "Tag.h"

/// 수학라이브러리
#include "../ImpStructure/ImpMath.h"
#include "../ImpStructure/ImpColor.h"

// FMOD
#include "../ImpLib/SoundInc/fmod.hpp"

#endif
