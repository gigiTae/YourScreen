#pragma once


namespace ImpToolModule
{
/// <summary>
/// Entity의 컴포넌트 정보를 표시하는 GUI
/// </summary>
class ComponentViewer:
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::LoadedWorld>
{
public:
    ComponentViewer();
    ~ComponentViewer()
    {
    }

public:
    // GrapichEngine 가져온다.
    void Initialize(ImpGraphics::IImpGraphicsEngine* grahphicsEngnie,
        ImpEngineModule::EngineModule* engineMdule,
        ImpEngineModule::InputManager* toolInput);

    // Entity의 컴포넌트 정보를 표시한다.
    void Update(ImpEngineModule::Entity* ent);

private:
    // Component의 정보를 표시한다.
    void ViewComponentInfomation(void* object,
        const std::string& name,
        ImpReflection::Type* type);

    // PrimitiveType에 대한 정보를 표시한다.
    void ViewPrimitiveType(void* object,
        const std::string& name,
        ImpReflection::Type* type);

    // std::vector
    void ViewVectorType(void* object,
        const std::string& name,
        ImpReflection::Type* type);

    // std::list
    void ViewListType(void* object,
        const std::string& name,
        ImpReflection::Type* type);

    // std::map
    void ViewMapType(void* object, const std::string& name, ImpReflection::Type* type);

    // std::unordered_map
    void ViewUnorderedMapType(void* object,
        const std::string& name,
        ImpReflection::Type* type);

    // 컴포넌트를 추가하는 GUI
    void ViewAddComponent(ImpEngineModule::Entity* ent);

private:
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::LoadedWorld& event) override;


private:
    int _addComponentIndex = 0;
    std::vector<std::string> _reflectComponents;
    ImpGraphics::IImpGraphicsEngine* _graphicsEngine;
    ImpEngineModule::EngineModule* _engineModule;
    ImpEngineModule::InputManager* _toolInput;


    ImpEngineModule::Entity* _viewEntity;
    ImpEngineModule::Component* _viewComponent;

    bool _isRebindEntity;


    Vector3 _currentAxis = { 0.f, 0.f, 0.f };
    Vector3 _prevAxis    = { 0.f, 0.f, 0.f };
};


} // namespace ImpToolModule