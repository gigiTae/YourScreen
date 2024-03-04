#pragma once
#include "Component.h"

namespace ImpEngineModule
{

/// <summary>
/// 애니메이션 전용 메쉬
/// </summary>
class SkeletalMesh: public Component
{
public:
    enum class RenderType
    {
        Default, // 양쪽 랜더링
        Left,    // 왼쪽만 랜더링
        Right,   // 오른쪽만 랜더링
        Clear    // 둘다 안그림
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

    // 현재 그래픽스에 연결되어있는지
    bool IsBindGraphics() const
    {
        return _isBindGraphics;
    }

    // 그래픽스와 연결상태를 지정한다.
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

    // 그래픽스 바인딩용함수
    bool IsChangedEmissionIntensity() const
    {
        return _isChangedEmissionIntensity;
    }
    // 그래픽스 바인딩용함수
    void SetIsChangedEmissionIntensity(bool val)
    {
        _isChangedEmissionIntensity = val;
    }
    // 그래픽스 바인딩용함수
    bool IsChangedAlpha() const
    {
        return _isChangedAlpha;
    }
    // 그래픽스 바인딩용함수
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

    bool _isOpaque;           // false 면 transparent true면 불투명
    bool _isCullNone = false; // 양면 그리기
    bool _isBindGraphics;

    bool _isChangedAlpha;
    bool _isChangedEmissionIntensity;

    float _alpha;
    float _emissionIntensity;

    REFLECTION_FRIEND(ImpEngineModule::SkeletalMesh);
};

} // namespace ImpEngineModule