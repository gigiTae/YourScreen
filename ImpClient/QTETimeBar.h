#pragma once

namespace ImpClient
{

/// <summary>
///  QTE �����ð��� ǥ���ϴ� UI
/// </summary>
class QTETimeBar: public ImpEngineModule::Component
{
public:
    QTETimeBar();
    ~QTETimeBar();

    Component* Clone(Component* clone) const override;

    void Start() override;
    void Update(float dt) override;

    // 0.f~1.f �� QTEColor Bar�� ���̸� �����Ѵ�.o
    void Set(float ratio);
    void SetRed();

    void SetPadID(unsigned int padID)
    {
        _padID = padID;
    }

private:
    unsigned int _padID;

    bool _isRed;
    float _colorBarRatio;
    float _elpasedTime;

    ImpEngineModule::UI* _ui;
    ImpEngineModule::UI* _colorBarUI;

    ImpEngineModule::Entity* _colorBar;
    std::wstring _colorTimeBarPrefab;
    std::wstring _greenTexture;
    std::wstring _redTextrue;
    std::wstring _corlorBarTexture;

    REFLECTION_FRIEND(QTETimeBar);
};

} // namespace ImpClient