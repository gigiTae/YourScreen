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

    if (skill == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        skill = new HackSkillUI(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *skill = *this;
    }

    return skill;
}

void ImpClient::HackSkillUI::Update(float dt)
{
    //// �÷��̾��� ���ݻ��¿� ���� UI�� �ٲ۴�
    float elasedTime   = _playerAttack->GetHackingElapsedTime();
    float hackCoolTime = _playerAttack->GetHackingCoolTime();

    if (elasedTime != hackCoolTime)
    {
        if (_blackScreenUI->GetAlpha() == 0.f)
        {
            // BlackScreen�� ����ؼ� ��ٿ� ����Ʈ
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

    // BlackScreen ����
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
