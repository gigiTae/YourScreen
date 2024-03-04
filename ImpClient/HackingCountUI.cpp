#include "ImpClientPCH.h"
#include "HackingCountUI.h"
#include "HackableObject.h"
#include "GamePad.h"
#include "AttackUI.h"
#include "HackSkillUI.h"
#include "HackingChip.h"

ImpClient::HackingCountUI::HackingCountUI()
    : _hackingObjects{}
    , _isLeftPlayer(false)
    , _isLastPoint(false)
    , _hackSkillUIEntity(nullptr)
    , _attackUIEntity(nullptr)
    , _hackSkillUIPrefab{}
    , _attackUIPrefab{}
{
}


ImpClient::HackingCountUI::~HackingCountUI()
{
}

ImpEngineModule::Component* ImpClient::HackingCountUI::Clone(Component* clone) const
{
    HackingCountUI* hackingCount = static_cast<HackingCountUI*>(clone);

    if (hackingCount == nullptr) // 새로 생성해서 복사본을 준다
    {
        hackingCount = new HackingCountUI(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *hackingCount = *this;
    }

    return hackingCount;
}


void ImpClient::HackingCountUI::SetHackingUI(ImpEngineModule::Entity* player,
    const std::vector<ImpEngineModule::Entity*>& hackingObject)
{
    unsigned int padID = player->GetComponent<GamePad>()->GetPadID();

    _hackingObjects = hackingObject;

    _isLeftPlayer = (padID == ImpClientDefine::leftPlayer) ? true : false;

    if (!_isLeftPlayer)
    {
        auto uiComponet = GetComponent<UI>();
        auto path       = uiComponet->GetTexturePath();
        path = PathManager::ChangeFileName(path, L"HackingCountBarR");
        uiComponet->SetTexturePath(path);
    }


    // HackingChipUI를 생성한다.
    ImpEngineModule::EntityResource* res = GetWorld()->GetResourceManager()->GetEntityResource(
        _hackingChipPrefab);

    constexpr float beginPosX = 25.f;
    constexpr float beginPosY = 25.f;

    // 해킹칩을 4개 생성한다.
    for (int i = 0; i < 4; ++i)
    {
        ImpEngineModule::Entity* ent = GetWorld()->RegisterEntity(res->GetEntity());
        ent->GetComponent<Transform>()->SetParent(GetComponent<Transform>());
        auto ui = ent->GetComponent<ImpEngineModule::UI>();

        ent->GetComponent<HackingChip>()->SetIsLeft(_isLeftPlayer);
        // UI 색깔변경
        if (!_isLeftPlayer)
        {
            auto path = ui->GetTexturePath();
            path      = PathManager::ChangeFileName(path, L"UnHackingChipR");
            ui->SetTexturePath(path);
        }

        if (i == 0 || i == 3)
        {
            ent->GetComponent<HackingChip>()->SetChipType(
                HackingChip::ChipType::QuickEMP);
        }
        else
        {
            ent->GetComponent<HackingChip>()->SetChipType(
                HackingChip::ChipType::ScaleEMP);
        }

        float x = beginPosX + 100.f * (i % 2);
        float y = beginPosY + 100.f * (i / 2);

        // 위치 설정
        ui->SetUIPostition(x, y);
        _hackingChips.push_back(ui);
    }

    //  == Attack UI 생성 ==
    ImpEngineModule::EntityResource* atkRes = GetWorld()->GetResourceManager()->GetEntityResource(
        _attackUIPrefab);

    // ui 위치
    _attackUIEntity = GetWorld()->RegisterEntity(atkRes->GetEntity());
    _attackUIEntity->GetComponent<Transform>()->SetParent(
        GetComponent<Transform>());

    auto attackUI       = _attackUIEntity->GetComponent<UI>();
    Vector4 attackUIPos = attackUI->GetUIPosition();
    attackUI->SetUIPostition(attackUIPos.z, -attackUIPos.w);

    // 플레이어 컴포넌트 연결
    _attackUIEntity->GetComponent<AttackUI>()->Set(player);

    // == HackSkill UI 생성 ==
    ImpEngineModule::EntityResource* skillRes = GetWorld()->GetResourceManager()->GetEntityResource(
        _hackSkillUIPrefab);

    _hackSkillUIEntity = GetWorld()->RegisterEntity(skillRes->GetEntity());
    _hackSkillUIEntity->GetComponent<Transform>()->SetParent(
        GetComponent<Transform>());

    auto hackUI        = _hackSkillUIEntity->GetComponent<UI>();
    Vector4 hasckUIPos = hackUI->GetUIPosition();
    hackUI->SetUIPostition(0.f, -hasckUIPos.w);

    // 플레이어 컴포넌트 연결
    _hackSkillUIEntity->GetComponent<HackSkillUI>()->Set(player);
}

void ImpClient::HackingCountUI::Update(float dt)
{
    if (_isLastPoint)
    {
        UpdateBigHackingChip();
    }
    else
    {
        UpdateSmallHackingChip();
    }
}

void ImpClient::HackingCountUI::MakeBigHackingChip()
{
}

void ImpClient::HackingCountUI::UpdateSmallHackingChip()
{
    int count = 0;

    for (size_t i = 0; i < 4; ++i)
    {
        auto state = _hackingObjects[i]->GetComponent<HackableObject>()->GetState();
        auto chip = _hackingChips[i]->GetComponent<HackingChip>();

        // 이미 변경된 경우
        if (chip->IsChanged())
        {
            ++count;
            continue;
        }

        if (_isLeftPlayer)
        {
            if (state == HackableObject::State::LeftPlayerHacked ||
                state == HackableObject::State::AllPlayerHacked)
            {
                chip->ChangeChip();
            }
        }
        else if (state == HackableObject::State::RightPlayerHacked ||
                 state == HackableObject::State::AllPlayerHacked)
        {
            chip->ChangeChip();
        }
    }

    if (count == 4)
    {
        _isLastPoint = true;
        ChangeToBigChip();
    }
}

void ImpClient::HackingCountUI::UpdateBigHackingChip()
{
    auto state = _hackingObjects[4]->GetComponent<HackableObject>()->GetState();
    auto chip  = _hackingChips[0]->GetComponent<HackingChip>();

    if (chip->IsChanged())
    {
        return;
    }

    if (_isLeftPlayer)
    {
        if (state == HackableObject::State::LeftPlayerHacked ||
            state == HackableObject::State::AllPlayerHacked)
        {
            chip->ChangeChip();
        }
    }
    else if (state == HackableObject::State::RightPlayerHacked ||
             state == HackableObject::State::AllPlayerHacked)
    {
        chip->ChangeChip();
    }
}

void ImpClient::HackingCountUI::ChangeToBigChip()
{
    for (int i = 0; i < 4; ++i)
    {
        Entity* ent = _hackingChips[i]->GetEntity();
        GetWorld()->DestroyEntity(ent);
    }

    _hackingChips.clear();

    // 커다란 칩을 생성한다.

    // HackingChipUI를 생성한다.
    ImpEngineModule::EntityResource* res = GetWorld()->GetResourceManager()->GetEntityResource(
        _bigHackingChipPrefab);

    constexpr float beginPosX = 25.f;
    constexpr float beginPosY = 25.f;

    Entity* ent = GetWorld()->RegisterEntity(res->GetEntity());

    ent->GetComponent<Transform>()->SetParent(GetComponent<Transform>());

    ent->GetComponent<UI>()->SetUISize(200.f, 200.f);
    ent->GetComponent<UI>()->SetUIPostition(beginPosX, beginPosY);
    ent->GetComponent<HackingChip>()->SetChipType(HackingChip::ChipType::Victory);
    ent->GetComponent<HackingChip>()->SetIsLeft(_isLeftPlayer);

    // UI 색깔변경
    if (!_isLeftPlayer)
    {
        auto ui   = ent->GetComponent<UI>();
        auto path = ui->GetTexturePath();
        path      = PathManager::ChangeFileName(path, L"UnHackingChipR");
        ui->SetTexturePath(path);
        ui->SetAlpha(1.f);
    }

    _hackingChips.push_back(ent->GetComponent<UI>());
}
