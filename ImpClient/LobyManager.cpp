#include "ImpClientPCH.h"
#include "LobyManager.h"

ImpClient::LobyManager::LobyManager()
    : _isLeftPlayerReady(false)
    , _isRightPlayerReady(false)
    , _leftReadyUI(nullptr)
    , _rightReadyUI(nullptr)
    , _isStart(false)
{
}

ImpClient::LobyManager::~LobyManager()
{
}

ImpEngineModule::Component* ImpClient::LobyManager::Clone(Component* clone) const
{
    LobyManager* lobyManager = static_cast<LobyManager*>(clone);

    if (lobyManager == nullptr) // 새로 생성해서 복사본을 준다
    {
        lobyManager = new LobyManager(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *lobyManager = *this;
    }

    return lobyManager;
}


void ImpClient::LobyManager::Update(float dt)
{
    if (!_isStart)
    {
        MakeUI();
        _isStart = true;
    }

    auto input = GetWorld()->GetInputManager();

    auto leftUI = _leftReadyUI->GetComponent<UI>();

    if (_isLeftPlayerReady)
    {
        leftUI->SetAlpha(1.f);
    }
    else
    {
        leftUI->SetAlpha(0.f);
    }

    auto rightUI = _rightReadyUI->GetComponent<UI>();

    if (_isRightPlayerReady)
    {
        rightUI->SetAlpha(1.f);
    }
    else
    {
        rightUI->SetAlpha(0.f);
    }

    // 왼쪽 플레이어와 오른쪽 플레이어가 준비완료, start 버튼 클릭
    if (_isLeftPlayerReady && _isRightPlayerReady)
    {
        StartGame();
    }

    // 게임 종료
    if (input->IsKeyState(Key::ESC, KeyState::Tap))
    {
        GetWorld()->GetEventManager()->ChangeWorld(L"Exit");
    }

}
void ImpClient::LobyManager::MakeUI()
{
    EntityResource* res = GetWorld()->GetResourceManager()->GetEntityResource(
        _readUIPrefab);

    if (res == nullptr) return;

    auto rect    = GetWorld()->GetInputManager()->GetScreenRect();
    float width  = static_cast<float>(rect.right - rect.left);
    float height = static_cast<float>(rect.bottom - rect.top);

    auto ui    = GetComponent<UI>();
    auto uiPos = ui->GetUIPosition();
    ui->SetUIPostition(width * 0.5f - uiPos.z * 0.5f, height * 0.9f - uiPos.w);

    // 왼쪽 해킹칩
    _leftReadyUI = GetWorld()->RegisterEntity(res->GetEntity());
    _leftReadyUI->GetComponent<Transform>()->SetParent(GetComponent<Transform>());
    auto leftUI = _leftReadyUI->GetComponent<UI>();

    leftUI->SetAlpha(0.f);
    leftUI->SetUIPostition(uiPos.z * 0.5f - 110.f, uiPos.w * 0.25f);

    // 오른쪽 해킹칩
    _rightReadyUI = GetWorld()->RegisterEntity(res->GetEntity());
    _rightReadyUI->GetComponent<Transform>()->SetParent(GetComponent<Transform>());

    ImpEngineModule::UI* rightUI = _rightReadyUI->GetComponent<UI>();
    rightUI->SetAlpha(0.f);
    rightUI->SetUIPostition(uiPos.z * 0.5f, uiPos.w * 0.25f);

    auto path = rightUI->GetTexturePath();
    path      = PathManager::ChangeFileName(path, L"Ready2");
    rightUI->SetTexturePath(path);
}

void ImpClient::LobyManager::StartGame()
{
    GetWorld()->GetEventManager()->ChangeWorld(L"StartAnimation");
}

void ImpClient::LobyManager::EndGame()
{
}

bool ImpClient::LobyManager::IsLeftReady()
{
    return _isLeftPlayerReady;
}

bool ImpClient::LobyManager::IsRightReady()
{
    return _isRightPlayerReady;
}

void ImpClient::LobyManager::SetLeftReady()
{
    _isLeftPlayerReady = true;
}

void ImpClient::LobyManager::SetRightReady()
{
    _isRightPlayerReady = true;
}
