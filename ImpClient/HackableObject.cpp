#include "ImpClientPCH.h"
#include "HackableObject.h"
#include "GamePad.h"
#include "GameManager.h"
#include "PlayerMovement.h"
#include "PlayerState.h"
#include "LobyManager.h"
#include "PlayerAttack.h"
#include "QTE.h"

ImpClient::HackableObject::HackableObject()
    : _isHackingPoint(false)
    , _state(State::None)
    , _hackableRadius(3.f)
    , _gameManager(nullptr)
    , _leftQTE(nullptr)
    , _rightQTE(nullptr)
    , _ability(SkillAbility::Victory)
    , _isLastPoint(false)
{
}

ImpClient::HackableObject::~HackableObject()
{
}

ImpEngineModule::Component* ImpClient::HackableObject::Clone(
    Component* clone /*= nullptr*/) const
{
    HackableObject* hackableObject = static_cast<HackableObject*>(clone);

    if (hackableObject == nullptr) // 새로 생성해서 복사본을 준다
    {
        hackableObject = new HackableObject(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *hackableObject = *this;
    }

    return hackableObject;
}


void ImpClient::HackableObject::HackObject(ImpEngineModule::Entity* player)
{
    if (player->GetComponent<GamePad>()->GetPadID() == ImpClientDefine::leftPlayer)
    {
        _leftQTE = nullptr;
    }
    else
    {
        _rightQTE = nullptr;
    }

    // 해킹 관련 처리를 한다.
    if (_state == State::AllPlayerHacked)
    {
        return;
    }

    // 왼쪽 플레이어가 해킹한 경우
    if (player == _gameManager->GetLeftPlayer())
    {
        if (_state == State::LeftPlayerHacked)
        {
            return;
        }
        else if (_state == State::None)
        {
            // 해킹성공
            IncreaseSkillAbility(player);
            _gameManager->AddLeftPlayerHackingCount();
            _state = State::LeftPlayerHacked;
        }
        else
        {
            // 해킹 성공
            IncreaseSkillAbility(player);
            _gameManager->AddLeftPlayerHackingCount();
            _state = State::AllPlayerHacked;
        }
    }
    // 오른쪽 플레이어가 해킹한 경우
    else if (player == _gameManager->GetRightPlayer())
    {
        if (_state == State::RightPlayerHacked)
        {
            return;
        }
        else if (_state == State::None)
        {
            // 해킹성공
            IncreaseSkillAbility(player);
            _gameManager->AddRightPlayerHackingCount();
            _state = State::RightPlayerHacked;
        }
        else
        {
            // 해킹성공
            IncreaseSkillAbility(player);
            _gameManager->AddRightPlayerHackingCount();
            _state = State::AllPlayerHacked;
        }
    }
}

void ImpClient::HackableObject::OnTriggerStay(
    const ImpEngineModule::Physics::Collision& collision)
{
    if (_gameManager == nullptr)
    {
        auto gameEnt = GetWorld()->GetEntity("GameManager");
        _gameManager = gameEnt->GetComponent<GameManager>();
    }

    ImpEngineModule::Collider* othetCollider = collision.GetOtherCollider(
        GetEntity());

    if (othetCollider->GetTag() != Physics::Tag::Player)
    {
        return;
    }

    bool isInteractable = IsInteractable(othetCollider->GetEntity());

    if (isInteractable)
    {
        auto padID = othetCollider->GetComponent<GamePad>()->GetPadID();

        if (padID == ImpClientDefine::leftPlayer)
        {
            _gameManager->GetLeftHackableUI()->GetComponent<UI>()->SetAlpha(0.9f);
        }
        else
        {
            _gameManager->GetRightHackableUI()->GetComponent<UI>()->SetAlpha(0.9f);
        }
    }
    else
    {
        auto padID = othetCollider->GetComponent<GamePad>()->GetPadID();

        if (padID == ImpClientDefine::leftPlayer)
        {
            _gameManager->GetLeftHackableUI()->GetComponent<UI>()->SetAlpha(0.f);
        }
        else
        {
            _gameManager->GetRightHackableUI()->GetComponent<UI>()->SetAlpha(0.f);
        }
    }

    // 해킹 가능한 조건
    auto input        = GetWorld()->GetInputManager();
    auto padID        = othetCollider->GetComponent<GamePad>()->GetPadID();
    auto playerAttack = othetCollider->GetComponent<PlayerAttack>();

    // 해킹버튼 클릭했으므로 해킹스킬을 쓴것으로 판단
    if (input->IsPadKeyState(padID, PadKey::X, KeyState::Tap))
    {
        if (playerAttack->CanUseHackSkill() && _isHackingPoint)
        {
            playerAttack->UseHackSkill();
        }
        else
        {
            // 공격 불가 && 상호작용 불가능한 오브젝트 해킹 시 사운드 재생
            // 이미 QTE 중일 때는 재생 하면 안됨!
            if (isInteractable)
            {
                if (playerAttack->CanUseHackSkill())
                {
                    playerAttack->UseHackSkill();
                }

                auto audioClip = GetWorld()
                                     ->GetEntity("AudioManager")
                                     ->GetComponent<AudioClip>();

                size_t audioIndex = audioClip->GetAudioIndex(L"SE_ActionFail");
                audioClip->PlaySound(audioIndex, false, audioIndex);
            }

            return;
        }
    }
    else
    {
        return;
    }

    if (IsHacakable(othetCollider->GetEntity()) && isInteractable)
    {
        CreateQTE(othetCollider->GetEntity());
    }
    else
    {
        auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

        size_t audioIndex = audioClip->GetAudioIndex(L"SE_ActionFail");
        audioClip->PlaySound(audioIndex, false, audioIndex);
    }
}

void ImpClient::HackableObject::OnTriggerExit(
    const ImpEngineModule::Physics::Collision& collision)
{
    ImpEngineModule::Collider* othetCollider = collision.GetOtherCollider(
        GetEntity());

    if (othetCollider->GetTag() != Physics::Tag::Player)
    {
        return;
    }

    if (othetCollider->GetComponent<PlayerState>()->GetActionState() !=
        PlayerState::ActionState::Hack)
    {
        return;
    }

    auto id = othetCollider->GetComponent<GamePad>()->GetPadID();

    // 해킹하는 상태인 경우 해킹을 취소한다.
    if (id == ImpClientDefine::leftPlayer)
    {
        _gameManager->GetLeftHackableUI()->GetComponent<UI>()->SetAlpha(0.f);
        if (_leftQTE)
        {
            _leftQTE->ExitQTE();
            _leftQTE = nullptr;
        }
    }
    else if (id == ImpClientDefine::rightPlayer)
    {
        _gameManager->GetRightHackableUI()->GetComponent<UI>()->SetAlpha(0.f);
        if (_rightQTE)
        {
            _rightQTE->ExitQTE();
            _rightQTE = nullptr;
        }
    }
}


bool ImpClient::HackableObject::IsInteractable(ImpEngineModule::Entity* ent) const
{
    // 4. ActionState == Idle
    if (ent->GetComponent<PlayerState>()->GetActionState() !=
        PlayerState::ActionState::Idle)
    {
        return false;
    }

    // 5. 해킹하는 오브젝트를 바라봐야한다.
    ImpMath::Vector3 objectPos = GetComponent<ImpEngineModule::Transform>()
                                     ->GetWorldPostiion();

    ImpMath::Vector3 playerPos = ent->GetComponent<ImpEngineModule::Transform>()
                                     ->GetWorldPostiion();

    ImpMath::Vector3 playerToObject = (objectPos - playerPos).Normalize();

    ImpMath::Vector3 playerLookDir = ent->GetComponent<PlayerMovement>()
                                         ->GetPlayerDirection();

    float radian = acos(playerToObject.Dot(playerLookDir));

    // 바라보는 각도는 90도
    if (radian >= ImpMath::HalfPI)
    {
        return false;
    }

    return true;
}

bool ImpClient::HackableObject::IsHacakable(ImpEngineModule::Entity* ent) const
{
    auto padID = ent->GetComponent<GamePad>()->GetPadID();
    // 2. 해킹한 오브젝트인지 판단

    // 0 : 왼쪽 플레이어
    if (padID == ImpClientDefine::leftPlayer &&
        (_state == State::AllPlayerHacked || _state == State::LeftPlayerHacked))
    {
        return false;
    }

    // 1: 오른쪽 플레이어
    else if (padID == ImpClientDefine::rightPlayer &&
             (_state == State::AllPlayerHacked || _state == State::RightPlayerHacked))
    {
        return false;
    }

    // 3. 마지막 해킹포인트인 경우 나머지를 4개를 해킹한상태여야 해킹이 가능하다.
    // TODO :: 플레이어에게 경고가 필요해보인다.
    if (_isLastPoint)
    {
        if (padID == ImpClientDefine::leftPlayer &&
            _gameManager->GetLeftHackingCount() < 4)
        {
            return false;
        }
        else if (padID == ImpClientDefine::rightPlayer &&
                 _gameManager->GetRighttHackingCount() < 4)
        {
            return false;
        }
    }

    return true;
}


void ImpClient::HackableObject::CreateQTE(ImpEngineModule::Entity* player)
{
    using namespace ImpEngineModule;

    // QTE를 생성한다.
    Entity* qteEntity = GetWorld()->RegisterEntity(
        _gameManager->GetQTEResource()->GetEntity());

    // leftPlayer
    if (player->GetComponent<GamePad>()->GetPadID() == 0)
    {
        _leftQTE = qteEntity->GetComponent<QTE>();
        _leftQTE->Set(player, this);
        player->GetComponent<PlayerMovement>()->SetQTE(_leftQTE);
    }
    else // rightPlayer
    {
        _rightQTE = qteEntity->GetComponent<QTE>();
        _rightQTE->Set(player, this);
        player->GetComponent<PlayerMovement>()->SetQTE(_rightQTE);
    }

    player->GetComponent<PlayerState>()->SetActionState(
        PlayerState::ActionState::Hack);
}

void ImpClient::HackableObject::IncreaseSkillAbility(ImpEngineModule::Entity* player)
{
    auto playerAttack = player->GetComponent<PlayerAttack>();

    if (_ability == SkillAbility::Qucik)
    {
        playerAttack->DecreaseHackingCoolTime(5.f);
    }
    else if (_ability == SkillAbility::Scale)
    {
        playerAttack->IncreaseHackingRange(2.5f);
    }
}
