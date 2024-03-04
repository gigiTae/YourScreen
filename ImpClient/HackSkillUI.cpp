#include "ImpClientPCH.h"
#include "HackSkillUI.h"
#include "PlayerAttack.h"
#include "PlayerState.h"
#include "GamePad.h"

ImpClient::HackSkillUI::HackSkillUI()
{
}

ImpClient::HackSkillUI::~HackSkillUI()
{
}

ImpEngineModule::Component* ImpClient::HackSkillUI::Clone(
    Component* clone /* = nullptr */) const
{
    HackSkillUI* skill = static_cast<HackSkillUI*>(clone);

    if (skill == nullptr) // 새로 생성해서 복사본을 준다
    {
        skill = new HackSkillUI(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *skill = *this;
    }

    return skill;
}

void ImpClient::HackSkillUI::Update(float dt)
{
    //// 플레이어의 공격상태에 따라서 UI를 바꾼다
    float elasedTime   = _playerAttack->GetHackingElapsedTime();
    float hackCoolTime = _playerAttack->GetHackingCoolTime();

    if (elasedTime != hackCoolTime)
    {
        if (_blackScreenUI->GetAlpha() == 0.f)
        {
            // BlackScreen을 사용해서 쿨다운 이펙트
            _blackScreenUI->SetAlpha(1.f);
        }

        auto uiPos = _ui->GetUIPosition();

        float width  = uiPos.z;
        float hegiht = uiPos.z * (hackCoolTime - elasedTime) / hackCoolTime;

        _blackScreenUI->SetUIPostition(0.f, 44.f + (60.f - hegiht * 0.5f));

        _blackScreenUI->SetUISize(width, hegiht);
    }
    else if (_blackScreenUI->GetAlpha() != 0.f)
    {
        _blackScreenUI->SetAlpha(0.f);
    }
}

void ImpClient::HackSkillUI::Set(ImpEngineModule::Entity* player)
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
    _blackScreenUI->GetComponent<UI>()->SetUIPostition(0.f, startY);

    _blackScreenUI->SetAlpha(0.f);

    _ui = GetComponent<UI>();

    if (_padID == ImpClientDefine::rightPlayer)
    {
        auto path = _ui->GetTexturePath();
        path      = PathManager::ChangeFileName(path, L"HackingSkillR");
        _ui->SetTexturePath(path);
    }
}
