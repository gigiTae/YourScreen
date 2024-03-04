#include "ImpClientPCH.h"
#include "QTETimeBar.h"

ImpClient::QTETimeBar::QTETimeBar()
    : _colorBar(nullptr)
    , _ui(nullptr)
    , _colorTimeBarPrefab{}
    , _colorBarRatio(1.f)
    , _elpasedTime(0.f)
    , _isRed(false)
    , _colorBarUI(nullptr)
    , _padID(0)
{
}

ImpClient::QTETimeBar::~QTETimeBar()
{
}

ImpEngineModule::Component* ImpClient::QTETimeBar::Clone(Component* clone) const
{
    QTETimeBar* qteTimeBar = static_cast<QTETimeBar*>(clone);

    if (qteTimeBar == nullptr) // 새로 생성해서 복사본을 준다
    {
        qteTimeBar = new QTETimeBar(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *qteTimeBar = *this;
    }

    qteTimeBar->_colorBar = nullptr;

    return qteTimeBar;
}


void ImpClient::QTETimeBar::Start()
{
    using namespace ImpEngineModule;

    _ui = GetComponent<UI>();

    if (_colorTimeBarPrefab.empty())
    {
        return;
    }

    // ColorBar 생성
    _colorBar = GetWorld()->RegisterEntity(
        GetWorld()->GetResourceManager()->GetEntityResource(_colorTimeBarPrefab)->GetEntity());

    // TimeBar를 부모로 설정
    _colorBar->GetComponent<Transform>()->SetParent(GetComponent<Transform>());
    _colorBarUI = _colorBar->GetComponent<UI>();

    // 오른쪽 UI 색깔변경
    if (_padID == ImpClientDefine::rightPlayer)
    {
        auto path = _ui->GetTexturePath();
        path      = PathManager::ChangeFileName(path, L"GrayTimeBarR");
        _ui->SetTexturePath(path);
        _greenTexture = path;

        auto colorPath = _colorBarUI->GetTexturePath();
        colorPath      = PathManager::ChangeFileName(colorPath, L"YellowBar");
        _colorBarUI->SetTexturePath(colorPath);
    }

    _corlorBarTexture = _colorBarUI->GetTexturePath();
}

void ImpClient::QTETimeBar::Update(float dt)
{
    constexpr float RedTime = 0.25f;

    if (_isRed)
    {
        _elpasedTime += dt;

        if (RedTime <= _elpasedTime)
        {
            _colorBarUI->SetTexturePath(_corlorBarTexture);
            _isRed = false;
        }
    }
}

void ImpClient::QTETimeBar::Set(float ratio)
{
    _colorBarUI->SetXRatio(ratio);
}

void ImpClient::QTETimeBar::SetRed()
{
    _elpasedTime = 0.f;
    _isRed       = true;
    _colorBarUI->SetTexturePath(_redTextrue);
}
