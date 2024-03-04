#pragma once
#include "Component.h"

namespace ImpEngineModule
{
/// <summary>
///  UI 컴포넌트
/// </summary>
class UI: public Component
{
public:
    UI();
    ~UI();

    Component* Clone(Component* clone = nullptr) const override;

    void Update(float dt) override;

    bool IsBindGraphics() const
    {
        return _isBindGraphics;
    }
    void SetBindGraphics(bool isBind)
    {
        _isBindGraphics = isBind;
    }

    unsigned int GetLayer() const
    {
        return _layer;
    }
    void SetLayer(unsigned int layer);

    // x: startX, y: startY, z: width, w: height
    ImpMath::Vector4 GetUIPosition() const
    {
        return _UIPosition;
    }

    ImpMath::Vector4 GetFinalUIPosition() const
    {
        return _finalUIPosition;
    }

    void SetUIPostition(float x, float y);

    void SetUISize(float width, float height);

    std::wstring GetTexturePath() const
    {
        return _texturePath;
    }
    void SetTexturePath(std::wstring path);

	bool IsChangedUIInfomation() const
    {
        return _isChangedInfomation;
    }

    void ChangeUIInfomation(bool isChanged)
    {
        _isChangedInfomation = isChanged;
    }

	float GetAlpha() const
    {
        return _alpha;
    }

    // 0.f~ 1.f
    void SetAlpha(float alpha);   

	float GetXRatio() const
    {
        return _xRatio;
    }
    void SetXRatio(float val)
    {
        _isChangedXRatio = true;
        _xRatio = val;
    }

	bool IsChangedXRatio() const
    {
        return _isChangedXRatio;
    }

    void SetIsChangedXRatio(bool val)
    {
        _isChangedXRatio = val;
    }

	bool OnNoise() const
    {
        return _onNoise;
    }

    void SetnNoise(bool onNoise);

    // 그래픽스 바인딩용 함수 
    bool IsChangedNoise() const
    {
        return _isChangedNoise;
    }

    // 그래픽스 바인딩용도 함수
    void SetIsChangedNoise(bool val)
    {
        _isChangedNoise = val;
    }

private:
    bool _isBindGraphics;
    bool _isChangedInfomation;
    bool _onNoise;
    bool _isChangedNoise;

    std::wstring _texturePath;
    float _alpha;
    ImpMath::Vector4 _UIPosition; // x: startX, y: startY, z: width, w: height
    ImpMath::Vector4 _finalUIPosition;
    unsigned int _layer;

    bool _isChangedXRatio;
    float _xRatio;

    REFLECTION_FRIEND(UI);
};

} // namespace ImpEngineModule