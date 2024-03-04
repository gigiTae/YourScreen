#pragma once

namespace ImpGraphics
{
class IImpGraphicsEngine;
}

// <summary>
// 게임엔진의 데이터에 수정사항이 생기거나 생성한 데이터를
// 그래픽스엔진에 바인딩하기위해서 필요한 함수 공간
//
// ToolModule에서 가지고 있기에는 애매하지만 현재로는 이거
// 한개때문에 라이브러리를 한개 더 만들기 힘드므로 ToolModule 에서 가지고있는다.
// </summary>
namespace ImpToolModule::Bind
{
// 엔티티를 그래픽스 엔진에 바인딩한다.
void Rebind(ImpEngineModule::Entity* ent,
            ImpGraphics::IImpGraphicsEngine* graphicsEngine);

// 메쉬오브젝트의 데이터에서 대해서 바인딩한다.
void RebindMeshObject(ImpEngineModule::Entity* ent,
                      ImpGraphics::IImpGraphicsEngine* graphicsEngine);

// 스켈레탈 메쉬 오브젝트의 데이터에서 대해서 바인딩한다.
void RebindSkelatalMeshObject(ImpEngineModule::Entity* ent,
    ImpGraphics::IImpGraphicsEngine* graphicsEngine);


// 라이트에 대해서 다시 바인딩한다.
void RebindLightObject(ImpEngineModule::Entity* ent,
                       ImpGraphics::IImpGraphicsEngine* graphicsEngine);

// UI 바인딩
void RebindUIObject(ImpEngineModule::Entity* ent,
                    ImpGraphics::IImpGraphicsEngine* graphicsEngine);


} // namespace ImpToolModule::Bind