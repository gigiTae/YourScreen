#pragma once

namespace ImpGraphics
{
class IImpGraphicsEngine;
}

namespace ImpGameEngine
{

/// <summary>
/// 게임모듈과 그래픽스엔진의 정보를 서로에게 정보를 전달하는 역할
///
/// </summary>
class RenderingSystem:
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::OnEntityCreated>,
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::OnEntityDestroyed>,
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::OnEntityStarted>,
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::LoadCubeMap>,
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::LoadedWorld>
{

public:
    RenderingSystem();
    ~RenderingSystem();

    // 초기화된 그래픽스엔진과 엔진모듈을 가져온다.
    void Initialize(ImpGraphics::IImpGraphicsEngine* graphics,
        ImpEngineModule::EngineModule* module);

    void UnloadEntityResources();

    // 프리팹이 가지고있는 리소스들을 로드한다.
    void LoadEntityResources();

    // 매프레임 그래픽스에 전달해야하는 정보를 전달한다.
    void Update();

    // 왼쪽 랜더링 업데이트
    void UpdateLeft();

    // 오른쪽 랜더링 업데이트
    void UpdateRight();

    // 종료
    void Finalize();

#pragma region Event
    // Entity 생성에대한 이벤트를 처리한다.
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::OnEntityCreated& event) override;

    // Entity Start에 대한 이벤트를 처리한다.
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::OnEntityStarted& event) override;

    // Entity 소멸에대한 이벤트를 처리한다.
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::OnEntityDestroyed& event) override;

    // CubeMap을 로드한다.
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::LoadCubeMap& event) override;

    // 엔티티 리소스들을 다시 바인딩한다
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::LoadedWorld& event) override;

#pragma endregion Event

private:
    void LoadAnimation(ImpEngineModule::Entity* ent);

private:
    const size_t entityResourcesID = 990000000000000000;
    std::vector<size_t> _bindMeshID;
    size_t _currenEntityID;

    ImpGraphics::IImpGraphicsEngine* _graphicsEngine;
    ImpEngineModule::EngineModule* _engineModule;

    ImpMath::Matrix _clearRenderMatrix;
};


} // namespace ImpGameEngine