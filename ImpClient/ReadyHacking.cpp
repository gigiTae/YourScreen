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

    if (hackableObject == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        hackableObject = new ReadyHacking(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
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

    // ��ŷ ������ ����
    auto input        = GetWorld()->GetInputManager();
    auto padID        = othetCollider->GetComponent<GamePad>()->GetPadID();
    auto playerAttack = othetCollider->GetComponent<PlayerAttack>();

    // ��ŷ��ư Ŭ�������Ƿ� ��ŷ��ų�� �������� �Ǵ�
    if (input->IsPadKeyState(padID, PadKey::X, KeyState::Tap))
    {
        if (playerAttack->CanUseHackSkill())
        {
            playerAttack->UseHackSkill();
        }
        else
        {
            // ���� �Ұ� && ��ȣ�ۿ� �Ұ����� ������Ʈ ��ŷ �� ���� ���
            // �̹� QTE ���� ���� ��� �ϸ� �ȵ�!
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

    // 5. ��ŷ�ϴ� ������Ʈ�� �ٶ�����Ѵ�.
    ImpMath::Vector3 objectPos = GetComponent<ImpEngineModule::Transform>()
                                     ->GetWorldPostiion();

    ImpMath::Vector3 playerPos = ent->GetComponent<ImpEngineModule::Transform>()
                                     ->GetWorldPostiion();

    ImpMath::Vector3 playerToObject = (objectPos - playerPos).Normalize();

    ImpMath::Vector3 playerLookDir = ent->GetComponent<PlayerMovement>()
                                         ->GetPlayerDirection();

    float radian = acos(playerToObject.Dot(playerLookDir));

    // �ٶ󺸴� ������ 90��
    if (radian >= ImpMath::HalfPI)
    {
        return false;
    }

    return true;
}


bool ImpClient::ReadyHacking::IsHacakable(ImpEngineModule::Entity* ent) const
{
    auto padID = ent->GetComponent<GamePad>()->GetPadID();
    // 2. ��ŷ�� ������Ʈ���� �Ǵ�

    // 0 : ���� �÷��̾�
    if (padID == ImpClientDefine::leftPlayer &&
        (_state == State::AllPlayerHacked || _state == State::LeftPlayerHacked))
    {
        return false;
    }

    // 1: ������ �÷��̾�
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

    // ��ŷ�ϴ� ������ ��� ��ŷ�� ����Ѵ�.
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
