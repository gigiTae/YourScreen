#include "ImpClientPCH.h"
#include "ReadyHacking.h"
#include "LobyManager.h"
#include "PlayerState.h"
#include "PlayerMovement.h"
#include "PlayerAttack.h"
#include "GamePad.h"
#include "QTE.h"

ImpClient::ReadyHacking::ReadyHacking()
    : _lobyManager(nullptr)
    , _state(State::None)
    , _leftQTE(nullptr)
    , _rightQTE(nullptr)
{
}

ImpClient::ReadyHacking::~ReadyHacking()
{
}

ImpEngineModule::Component* ImpClient::ReadyHacking::Clone(
    Component* clone /* = nullptr */) const
{
    ReadyHacking* hackableObject = static_cast<ReadyHacking*>(clone);

    if (hackableObject == nullptr) // 새로 생성해서 복사본을 준다
    {
        hackableObject = new ReadyHacking(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *hackableObject = *this;
    }

    return hackableObject;
}


void ImpClient::ReadyHacking::Update(float dt)
{
}

void ImpClient::ReadyHacking::OnTriggerStay(const Physics::Collision& collision)
{
    if (_lobyManager == nullptr)
    {
        _lobyManager = GetWorld()->GetEntity("LobyManager")->GetComponent<LobyManager>();
    }

    ImpEngineModule::Collider* othetCollider = collision.GetOtherCollider(
        GetEntity());

    if (othetCollider->GetTag() != Physics::Tag::Player)
    {
        return;
    }

    bool isInteractable = IsInteractable(othetCollider->GetEntity());

    // 해킹 가능한 조건
    auto input        = GetWorld()->GetInputManager();
    auto padID        = othetCollider->GetComponent<GamePad>()->GetPadID();
    auto playerAttack = othetCollider->GetComponent<PlayerAttack>();

    // 해킹버튼 클릭했으므로 해킹스킬을 쓴것으로 판단
    if (input->IsPadKeyState(padID, PadKey::X, KeyState::Tap))
    {
        if (playerAttack->CanUseHackSkill())
        {
            playerAttack->UseHackSkill();
        }
        else
        {
            // 공격 불가 && 상호작용 불가능한 오브젝트 해킹 시 사운드 재생
            // 이미 QTE 중일 때는 재생 하면 안됨!
            if (isInteractable)
            {
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

bool ImpClient::ReadyHacking::IsInteractable(ImpEngineModule::Entity* ent) const
{
    // 4. ActionState == Idle
    if (ent->GetComponent<PlayerState>()->GetActionState() !=
        PlayerState::ActionState::Idle)
    {
        return false;
    }

    if (ent->GetComponent<GamePad>()->GetPadID() == ImpClientDefine::leftPlayer &&
        _lobyManager->IsLeftReady())
    {
        return false;
    }
    else if (ent->GetComponent<GamePad>()->GetPadID() == ImpClientDefine::rightPlayer &&
             _lobyManager->IsRightReady())
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


bool ImpClient::ReadyHacking::IsHacakable(ImpEngineModule::Entity* ent) const
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

    return true;
}

void ImpClient::ReadyHacking::CreateQTE(ImpEngineModule::Entity* player)
{
    auto res = GetWorld()->GetResourceManager()->GetEntityResource(_qtePrefab);

    Entity* qteEntity = GetWorld()->RegisterEntity(res->GetEntity());

    if (player->GetComponent<GamePad>()->GetPadID() == 0)
    {
        _leftQTE = qteEntity->GetComponent<QTE>();
        _leftQTE->Set(player, nullptr, this);
        player->GetComponent<PlayerMovement>()->SetQTE(_leftQTE);
    }
    else
    {
        _rightQTE = qteEntity->GetComponent<QTE>();
        _rightQTE->Set(player, nullptr, this);
        player->GetComponent<PlayerMovement>()->SetQTE(_rightQTE);
    }

    player->GetComponent<PlayerState>()->SetActionState(
        PlayerState::ActionState::Hack);
}


void ImpClient::ReadyHacking::FailQTE(unsigned int padID)
{
    if (padID == ImpClientDefine::leftPlayer)
    {
        _leftQTE = nullptr;
    }
    else
    {
        _rightQTE = nullptr;
    }
}


void ImpClient::ReadyHacking::ClearQTE(unsigned int padID)
{
    if (padID == ImpClientDefine::leftPlayer)
    {
        _lobyManager->SetLeftReady();
    }
    else
    {
        _lobyManager->SetRightReady();
    }
}

void ImpClient::ReadyHacking::OnTriggerExit(
    const ImpEngineModule::Physics::Collision& collision)
{

    // 해킹하는 상태인 경우 해킹을 취소한다.
    if (_leftQTE)
    {
        _leftQTE->ExitQTE();
        _leftQTE = nullptr;
    }

    if (_rightQTE)
    {
        _rightQTE->ExitQTE();
        _rightQTE = nullptr;
    }
}
