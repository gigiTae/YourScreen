#pragma once
#include "Component.h"

namespace ImpEngineModule
{

/// <summary>
/// �ִϸ��̼� ���� �޽�
/// </summary>
class SkeletalMesh: public Component
{
public:
    enum class RenderType
    {
        Default, // ���� ������
        Left,    // ���ʸ� ������
        Right,   // �����ʸ� ������
        Clear    // �Ѵ� �ȱ׸�
    };

    SkeletalMesh();
    ~SkeletalMesh();

    Component* Clone(Component* clone = nullptr) const override;

    std::wstring GetPixelShaderPath() const
    {
        return _pixelShader;
    }
    void SetPixelShaderPath(std::wstring path)
    {
        _pixelShader = path;
    }
    std::wstring GetVertexShaderPath() const
    {
        return _vertexShader;
    }
    void SetVertexShaderPath(std::wstring path)
    {
        _vertexShader = path;
    }
    std::wstring GetFBXPath() const
    {
        return _fbx;
    }
    void SetFBXPath(std::wstring val)
    {
        _fbx = val;
    }

    // ���� �׷��Ƚ��� ����Ǿ��ִ���
    bool IsBindGraphics() const
    {
        return _isBindGraphics;
    }

    // �׷��Ƚ��� ������¸� �����Ѵ�.
    void SetBindGraphics(bool isBindGraphics)
    {
        _isBindGraphics = isBindGraphics;
    }

    ImpEngineModule::SkeletalMesh::RenderType GetRenderType() const
    {
        return _renderType;
    }
    void SetRenderType(ImpEngineModule::SkeletalMesh::RenderType val)
    {
        _renderType = val;
    }

    std::wstring GetBaseColor() const
    {
        return _baseColor;
    }
    void SetBaseColor(std::wstring val)
    {
        _baseColor = val;
    }

    std::wstring GetMetalic() const
    {
        return _metalic;
    }
    void SetMetalic(std::wstring val)
    {
        _metalic = val;
    }

    std::wstring GetEmission() const
    {
        return _emission;
    }
    void SetEmission(std::wstring val)
    {
        _emission = val;
    }

    std::wstring GetNormal() const
    {
        return _normal;
    }
    void SetNormal(std::wstring val)
    {
        _normal = val;
    }

    bool IsOpaque() const
    {
        return _isOpaque;
    }
    void SetOpaque(bool val)
    {
        _isOpaque = val;
    }

    bool IsCullNone() const
    {
        return _isCullNone;
    }
    void SetCullNone(bool val)
    {
        _isCullNone = val;
    }

    float GetAlpha() const
    {
        return _alpha;
    }
    void SetAlpha(float alpha);

    float GetEmissionIntensity() const
    {
        return _emissionIntensity;
    }
    void SetEmissionIntensity(float intensity);

    // �׷��Ƚ� ���ε����Լ�
    bool IsChangedEmissionIntensity() const
    {
        return _isChangedEmissionIntensity;
    }
    // �׷��Ƚ� ���ε����Լ�
    void SetIsChangedEmissionIntensity(bool val)
    {
        _isChangedEmissionIntensity = val;
    }
    // �׷��Ƚ� ���ε����Լ�
    bool IsChangedAlpha() const
    {
        return _isChangedAlpha;
    }
    // �׷��Ƚ� ���ε����Լ�
    void SetIsChangedAlpha(bool val)
    {
        _isChangedAlpha = val;
    }

private:
    RenderType _renderType;

    std::wstring _pixelShader;
    std::wstring _vertexShader;
    std::wstring _fbx;

    std::wstring _baseColor;
    std::wstring _normal;
    std::wstring _metalic;
    std::wstring _emission;

    bool _isOpaque;           // false �� transparent true�� ������
    bool _isCullNone = false; // ��� �׸���
    bool _isBindGraphics;

    bool _isChangedAlpha;
    bool _isChangedEmissionIntensity;

    float _alpha;
    float _emissionIntensity;

    REFLECTION_FRIEND(ImpEngineModule::SkeletalMesh);
};

} // namespace ImpEngineModule