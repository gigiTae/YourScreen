#pragma once



#ifdef IMP_GAME_ENGINE_DLL_EXPORTS
#define IMP_ENGNIE_API __declspec(dllexport)
#else
#define IMP_ENGNIE_API __declspec(dllimport)
#endif

#ifdef IMP_GAME_ENGINE_DLL_EXPORTS
#define IMP_ENGNIE_TEMPLATE_API __declspec(dllexport)
#else
#define IMP_ENGNIE_TEMPLATE_API __declspec(dllimport)
#endif
