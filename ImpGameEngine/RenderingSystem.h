#pragma once

namespace ImpGraphics
{
class IImpGraphicsEngine;
}

namespace ImpGameEngine
{

/// <summary>
/// ���Ӹ��� �׷��Ƚ������� ������ ���ο��� ������ �����ϴ� ����
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

    // �ʱ�ȭ�� �׷��Ƚ������� ��������� �����´�.
    void Initialize(ImpGraphics::IImpGraphicsEngine* graphics,
        ImpEngineModule::EngineModule* module);

    void UnloadEntityResources();

    // �������� �������ִ� ���ҽ����� �ε��Ѵ�.
    void LoadEntityResources();

    // �������� �׷��Ƚ��� �����ؾ��ϴ� ������ �����Ѵ�.
    void Update();

    // ���� ������ ������Ʈ
    void UpdateLeft();

    // ������ ������ ������Ʈ
    void UpdateRight();

    // ����
    void Finalize();

#pragma region Event
    // Entity ���������� �̺�Ʈ�� ó���Ѵ�.
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::OnEntityCreated& event) override;

    // Entity Start�� ���� �̺�Ʈ�� ó���Ѵ�.
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::OnEntityStarted& event) override;

    // Entity �Ҹ꿡���� �̺�Ʈ�� ó���Ѵ�.
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::OnEntityDestroyed& event) override;

    // CubeMap�� �ε��Ѵ�.
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::LoadCubeMap& event) override;

    // ��ƼƼ ���ҽ����� �ٽ� ���ε��Ѵ�
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