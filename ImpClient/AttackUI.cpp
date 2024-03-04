#include "ImpClientPCH.h"
#include "AttackUI.h"
#include "GamePad.h"
#include "PlayerAttack.h"
#include "PlayerState.h"

ImpClient::AttackUI::AttackUI()
    : _playerAttack(nullptr)
    , _padID(-1)
    , _blackScreenUI(nullptr)
    , _whiteScreenUI(nullptr)
    , _whiteScreenPrefab{}
    , _blackScreenPrefab{}
    , _ui(nullptr)
    , _playerState(nullptr)
{
}


ImpClient::AttackUI::~AttackUI()
{
}

ImpEngineModule::Component* ImpClient::AttackUI::Clone(
    Component* clone /* = nullptr */) const
{
    AttackUI* attackUI = static_cast<AttackUI*>(clone);

    if (attackUI == nullptr) // 새로 생성해서 복사본을 준다
    {
        attackUI = new AttackUI(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *attackUI = *this;
    }

    return attackUI;
}

void ImpClient::AttackUI::Update(float dt)
{
    // 플레이어의 공격상태에 따라서 UI를 바꾼다
    float elasedTime     = _playerAttack->GetAttackElapsedTime();
    float attackCoolTime = _playerAttack->GetAttackCoolTime();

    if (elasedTime == attackCoolTime)
    {
        if (_blackScreenUI->GetAlpha() == 1.f)
        {
            _blackScreenUI->SetAlpha(0.f);
        }

        // WhiteScreen을 사용해서 Hold 이펙트
        auto input = GetWorld()->GetInputManager();
        if (input->IsPadKeyState(_padID, PadKey::Y, KeyState::Hold) &&
            _playerState->GetActionState() == PlayerState::ActionState::Idle)
        {
            if (_whiteScreenUI->GetAlpha() == 0.f)
            {
                _whiteScreenUI->SetAlpha(1.f);
            }
        }
        else if (_whiteScreenUI->GetAlpha() == 1.f)
        {
            _whiteScreenUI->SetAlpha(0.f);
        }
    }
    else
    {
        _whiteScreenUI->SetAlpha(0.f);

        // BlackScreen을 사용해서 쿨다운 이펙트
        _blackScreenUI->SetAlpha(1.f);

        auto uiPos = _ui->GetUIPosition();

        float width  = uiPos.z;
        float hegiht = uiPos.z * (attackCoolTime - elasedTime) / attackCoolTime;

        _blackScreenUI->SetUIPostition(0.f, 44.f + (60.f - hegiht * 0.5f));

        _blackScreenUI->SetUISize(width, hegiht);
    }
}

void ImpClient::AttackUI::Set(ImpEngineModule::Entity* player)
{
    _playerAttack = player->GetComponent<PlayerAttack>();
    _playerState  = player->GetComponent<PlayerState>();

    assert(_playerAttack);

    _padID = player->GetComponent<GamePad>()->GetPadID();

    // BlackScreen 생성
    auto blackRes = GetWorld()->GetResourceManager()->GetEntityResource(
        _blackScreenPrefab);
    _blackScreenUI = GetWorld()->RegisterEntity(blackRes->GetEntity())->GetComponent<UI>();

    _blackScreenUI->GetComponent<Transform>()->SetParent(GetComponent<Transform>());
    _blackScreenUI->GetComponent<UI>()->SetUIPostition(0.f, 44.f);

    // WhiteScreen 생성
    auto whiterRes = GetWorld()->GetResourceManager()->GetEntityResource(
        _whiteScreenPrefab);
    _whiteScreenUI = GetWorld()
                         ->RegisterEntity(whiterRes->GetEntity())
                         ->GetComponent<UI>();

    _whiteScreenUI->GetComponent<Transform>()->SetParent(GetComponent<Transform>());
    _whiteScreenUI->GetComponent<UI>()->SetUIPostition(0.f, 44.f);


    _blackScreenUI->SetAlpha(0.f);
    _whiteScreenUI->SetAlpha(0.f);

    _ui = GetComponent<UI>();

    if (_padID == ImpClientDefine::rightPlayer)
    {
        auto path = _ui->GetTexturePath();
        path      = PathManager::ChangeFileName(path, L"AttackUIR");
        _ui->SetTexturePath(path);
    }
}
