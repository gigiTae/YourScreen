// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

// 여기에 미리 컴파일하려는 헤더 추가
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifndef IMP_TOOL_MODILE_PCH
#define IMP_TOOL_MODILE_PCH

#include <d3d11.h>
#include <DirectXMath.h>

#include <ostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <filesystem>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuizmo.h"
#include "ImSequencer.h"
#include "ImCurveEdit.h"
#include "GraphEditor.h"

#include "../ImpEngineModule/EngineModuleHeader.h"
#include "../ImpReflection/ImpReflectionHeader.h"

#pragma comment(lib, "../ImpLib/ImpGraphicsEngine.lib")
#include "../ImpGraphicsEngine/ImpGraphicsEngine.h"

#include "Save.h"
//#include "NameCheck.h"
#include "ImGuiColor.h"

#endif // !IMP_TOOL_MODULE_PCH
