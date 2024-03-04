#include "EngineModulePCH.h"
#include "UI.h"
#include "Transform.h"
#include "Entity.h"

ImpEngineModule::UI::UI()
    : _UIPosition{}
    , _layer(0)
    , _texturePath{}
    , _isChangedInfomation(false)
    , _isBindGraphics(false)
    , _alpha(1.f)
    , _isChangedXRatio(false)
    , _xRatio(1.f)
    , _onNoise(false)
    , _isChangedNoise(false)
{
}

ImpEngineModule::UI::~UI()
{
}

ImpEngineModule::Component* ImpEngineModule::UI::Clone(
    Component* clone /*= nullptr*/) const
{
    UI* ui = static_cast<UI*>(clone);

    if (ui == nullptr) // 새로 생성해서 복사본을 준다
    {
        ui = new UI(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *ui = *this;
    }

    return ui;
}


void ImpEngineModule::UI::Update(float dt)
{
    auto transform = GetComponent<ImpEngineModule::Transform>();

    auto parentT = transform->GetParent();

    if (parentT == nullptr)
    {
        _finalUIPosition = _UIPosition;
    }
    else
    {
        auto parentUI = parentT->GetComponent<UI>();

        if (parentUI == nullptr)
        {
            _finalUIPosition = _UIPosition;
        }
        else
        {
            ImpMath::Vector4 parentUIPos = parentUI->GetFinalUIPosition();

            _finalUIPosition = _UIPosition;
            _finalUIPosition.x += parentUIPos.x;
            _finalUIPosition.y += parentUIPos.y;
        }
    }
}


void ImpEngineModule::UI::SetLayer(unsigned int layer)
{
    _layer               = layer;
    _isChangedInfomation = true;
}

void ImpEngineModule::UI::SetUIPostition(float x, float y)
{
    _UIPosition.x = x;
    _UIPosition.y = y;
}

void ImpEngineModule::UI::SetUISize(float width, float height)
{
    _UIPosition.z = width;
    _UIPosition.w = height;

    _isChangedInfomation = true;
}


void ImpEngineModule::UI::SetTexturePath(std::wstring path)
{
    _texturePath         = path;
    _isChangedInfomation = true;
}

void ImpEngineModule::UI::SetAlpha(float alpha)
{
    _alpha = std::clamp(alpha, 0.f, 1.f);
}

void ImpEngineModule::UI::SetnNoise(bool onNoise)
{
    _isChangedNoise = true;
    _onNoise = onNoise;
}
