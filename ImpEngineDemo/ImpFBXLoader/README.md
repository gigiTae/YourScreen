# ImpFBXLoader

## 프로젝트 설정 확인 할 것
1. [ C/C++ > 일반 > 추가 포함 디렉터리 ] `C:\Program Files\Autodesk\FBX\FBX SDK\2020.3.4\include`, `$(SolutionDir)\ImpStructure`
2. [ 일반 > 출력 디렉터리 ] `$(SolutionDir)$(Platform)\$(Configuration)\`
3. [ 라이브러리 관리자 > 일반 > 추가 종속성 ] `libfbxsdk-md.lib`, `libxml2-md.lib`, `zlib-md.lib`, `ImpStructure.lib`
4. [ 라이브러리 관리자 > 일반 > 추가 라이브러리 디렉터리 ] 
`C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/$debug/$(Configuration)`, 
`$(SolutionDir)$(Platform)\$(Configuration)`

## 종속성
- ImpStructe : `"ImpMesh.h"`
- FBX SDK : `<fbxsdk.h>`
- 디버깅용 : `<cassert>`