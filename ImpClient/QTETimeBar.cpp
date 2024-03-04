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

    if (qteTimeBar == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        qteTimeBar = new QTETimeBar(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
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

    // ColorBar ����
    _colorBar = GetWorld()->RegisterEntity(
        GetWorld()->GetResourceManager()->GetEntityResource(_colorTimeBarPrefab)->GetEntity());

    // TimeBar�� �θ�� ����
    _colorBar->GetComponent<Transform>()->SetParent(GetComponent<Transform>());
    _colorBarUI = _colorBar->GetComponent<UI>();

    // ������ UI ���򺯰�
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
