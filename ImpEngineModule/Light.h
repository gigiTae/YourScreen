#pragma once
#include "Component.h"

namespace ImpEngineModule
{
/// <summary>
/// Light ������Ʈ
/// </summary>
class Light: public Component
{
public:
    enum class LightType
    {
        Point,
        Spot,
        Directional,
    };

public:
    Light();
    ~Light();

    Component* Clone(Component* clone = nullptr) const override;

    void Update(float dt) override;

    // �׷��Ƚ� ���ε�����
    bool IsBindGraphics() const
    {
        return _isBindGraphics;
    }

    // �׷��Ƚ� ���ε� ����
    void SetBindGraphics(bool isBinded)
    {
        _isBindGraphics = isBinded;
    }

    float GetIntensity() const
    {
        return _intensity;
    }
    void SetIntensity(float intensity)
    {
        _intensity = intensity;
    }

    LightType GetLightType() const
    {
        return _type;
    }
    void SetLightType(LightType type)
    {
        _type = type;
    }

    ImpStructure::ImpColor GetColor() const
    {
        return _color;
    }
    void SetColor(ImpStructure::ImpColor color)
    {
        _color = color;
    }

    float GetRange() const
    {
        return _range;
    }
    void SetRange(float range)
    {
        _range = range;
    }

    float GetSpotRadius() const
    {
        return _spotRadius;
    }
    void SetSpotRadius(float val)
    {
        _spotRadius = val;
    }


private:
    LightType _type;
    float _intensity;
    ImpStructure::ImpColor _color; // ���� ����

    float _range;      // ���� ���� (PointLight)
    float _spotRadius; // SpotLight

    bool _isBindGraphics;

    REFLECTION_FRIEND(Light);
};


} // namespace ImpEngineModule