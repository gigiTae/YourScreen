#pragma once

namespace ImpToolModule::Debug
{
class Mesh;
}

namespace ImpToolModule
{
class EntityViewer;
class WorldViewer;
class ImpEditor;

/// <summary>
/// ����Ű Ŭ����
/// </summary>
class Shortcut:
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::LoadedWorld>
{
public:
    Shortcut();
    ~Shortcut();

    void Initialize(ImpEngineModule::EngineModule* engineMoudle,
        ImpToolModule::EntityViewer* entViewer,
        ImpEngineModule::InputManager* toolInput,
        ImpGraphics::IImpGraphicsEngine* graphics,
        ImpToolModule::WorldViewer* worldViewer,
        ImpToolModule::ImpEditor* editor);

    void Finalize();

    void Update();

    bool IsShowCopyBoundingVolume() const
    {
        return _isShowCopyBoundingVolume;
    }
    void SetShowCopyBoundingVolume(bool val)
    {
        _isShowCopyBoundingVolume = val;
    }

private:
    // ����
    void Copy();

    // �ٿ��ֱ�
    void Paste();

    void ShowCopyBoundingVolume();

    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::LoadedWorld& event) override;

    void Delete();

private:
    ImpToolModule::EntityViewer* _entityViewer;
    ImpToolModule::WorldViewer* _worldViewer;
    ImpToolModule::ImpEditor* _editor;

    bool _isShowCopyBoundingVolume;

    ImpEngineModule::InputManager* _toolInputManager;
    ImpEngineModule::EngineModule* _engineModule;

    ImpGraphics::IImpGraphicsEngine* _graphics;
    ImpEngineModule::Entity* _copyEntity; // �����ϴ� Entity
    Debug::Mesh* _debugMesh = nullptr;
};

} // namespace ImpToolModule