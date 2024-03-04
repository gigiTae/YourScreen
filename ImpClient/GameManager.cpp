#include "ImpClientPCH.h"
#include "GameManager.h"
#include "HackableObject.h"
#include "HackingArrow.h"
#include "HackingCountUI.h"

// ���� �¸� �� ĳ���� �ִϸ��̼��� ���� �߰� (2024-02-21 ���ֿ�)
#include "PlayerState.h"
#include "PlayerMovement.h"

ImpClient::GameManager::GameManager()
    : _leftPlayerName{ "Player1" }
    , _leftPlayer(nullptr)
    , _rightPlayer(nullptr)
    , _rightPlayerName{ "Player2" }
    , _world(nullptr)
    , _hackableObjects{}
    , _hackingArrowPrefab()
    , _hackingArrows{}
    , _hackingArrowResource(nullptr)
    , _isStart(false)
    , _leftHackingCount(0)
    , _rightHackingCount(0)
    , _qteResource(nullptr)
    , _hackingCountPrefab{}
    , _leftHackingCountUI(nullptr)
    , _rightHackingCountUI(nullptr)
    , _leftHackableUI(nullptr)
    , _rightHackableUI(nullptr)
    , _hackableUIPrefab{}
    , _isEnd(false)
{
}

ImpClient::GameManager::~GameManager()
{
}

ImpEngineModule::Component* ImpClient::GameManager::Clone(Component* clone) const
{
    GameManager* animation = static_cast<GameManager*>(clone);

    if (animation == nullptr)
    {
        animation = new GameManager(*this);
    }
    else
    {
        *animation = *this;
    }


    return animation;
}

void ImpClient::GameManager::Start()
{
    _world = GetWorld();

    auto& entities = _world->GetEntities();

    for (auto& ent : entities)
    {
        // ��ŷ ������ ������Ʈ���� ���忡�� �����´�.
        if (ent->HasComponent<HackableObject>())
        {
            _hackableObjects.push_back(ent.get());
        }

        // ���� �÷��̾ �����´�
        if (ent->GetName() == _leftPlayerName)
        {
            _leftPlayer = ent.get();
        }

        // ������ �÷��̾ �����´�
        if (ent->GetName() == _rightPlayerName)
        {
            _rightPlayer = ent.get();
        }
    }
}

void ImpClient::GameManager::Update(float dt)
{
    assert(_leftPlayer);
    assert(_rightPlayer);

    if (!_isStart)
    {
        SelectHackingPoints();
        AddHackingArrow();
        LoadQTEResource();
        LoadHackingCountUI();
        LoadHackableUI();
    }


    if (_isEnd)
    {
        auto input = GetWorld()->GetInputManager();

        if (_leftHackingCount == 5 &&
            input->IsPadKeyState(
                ImpClientDefine::leftPlayer, PadKey::Start, KeyState::Tap))
        {
            GetWorld()->GetEventManager()->ChangeWorld(L"Loby");
        }
        else if (_rightHackingCount == 5 &&
                 input->IsPadKeyState(
                     ImpClientDefine::rightPlayer, PadKey::Start, KeyState::Tap))
        {
            GetWorld()->GetEventManager()->ChangeWorld(L"Loby");
        }
    }
}


void ImpClient::GameManager::AddLeftPlayerHackingCount()
{
    ++_leftHackingCount;

    // �����÷��̾ 4���� ��ŷ�Ѱ�� ������ �÷��̾��� ȭ��ǥ�� Ȱ��ȭ�ϰ� ��ŷ�������� �����Ѵ�.
    if (_leftHackingCount == 4)
    {
        _hackingArrows[5]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
            ImpEngineModule::Mesh::RenderType::Right);

        // ��ŷ ����Ʈ�� Ȱ��ȭ�Ѵ�.
        _hackingPoints[4]->GetComponent<HackableObject>()->SetHackingPoint(true);
    }

    // ���� �÷��̾� �¸�
    if (_leftHackingCount == 5)
    {
        EndGame();
    }
}


void ImpClient::GameManager::AddRightPlayerHackingCount()
{
    ++_rightHackingCount;

    // ������ �÷��̾ 4���� ��ŷ�� ���
    if (_rightHackingCount == 4)
    {
        _hackingArrows[4]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
            ImpEngineModule::Mesh::RenderType::Left);

        // ��ŷ ����Ʈ�� Ȱ��ȭ �Ѵ�.
        _hackingPoints[4]->GetComponent<HackableObject>()->SetHackingPoint(true);
    }

    // ������ �÷��̾� �¸�
    if (_rightHackingCount == 5)
    {
        EndGame();
    }
}


ImpEngineModule::EntityResource* ImpClient::GameManager::GetQTEResource()
{
    return _qteResource;
}

void ImpClient::GameManager::SelectHackingPoints()
{
    // ��ŷ ������ ������Ʈ���߿��� �������� 5���� �����Ѵ�.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(
        0, static_cast<int>(_hackableObjects.size() - 1));

    int count      = 0;
    int errorCount = 0;
    while (count != 5)
    {
        // �ʱ� ���� ���� ������ �ڵ�
        ++errorCount;
        if (errorCount >= 10000000)
        {
            assert(
                !"��ŷ����Ʈ�� Quick 2�� Scale 2�� Victory 1���� �����ؾ��մϴ�");
        }

        size_t randomIndex  = dis(gen);
        auto hackalbeObject = _hackableObjects[randomIndex]
                                  ->GetComponent<HackableObject>();

        // �̹� ��ŷ����Ʈ�� ���õȰ��
        if (hackalbeObject->IsHackingPoint())
        {
            continue;
        }

        // 0,3���� Qucik �����Ѵ�
        if (count == 0 || count == 3)
        {
            if (hackalbeObject->GetAbility() != HackableObject::SkillAbility::Qucik)
            {
                continue;
            }
            hackalbeObject->SetHackingPoint(true);
        }
        // 1,2���� Scale �����Ѵ�
        else if (count == 1 || count == 2)
        {
            if (hackalbeObject->GetAbility() != HackableObject::SkillAbility::Scale)
            {
                continue;
            }
            hackalbeObject->SetHackingPoint(true);
        }
        else
        {
            // ������ ��ŷ����Ʈ�� Victory�� �����ؾ��Ѵ�.
            if (hackalbeObject->GetAbility() != HackableObject::SkillAbility::Victory)
            {
                continue;
            }
            hackalbeObject->SetLastPoint(true);
        }

        hackalbeObject->SetGameManager(this);
        _hackingPoints.push_back(_hackableObjects[randomIndex]);
        ++count;
    }
}

void ImpClient::GameManager::AddHackingArrow()
{
    _isStart = true;

    // ���ҽ� ��������
    _hackingArrowResource = _world->GetResourceManager()->GetEntityResource(
        _hackingArrowPrefab);

    // ��ŷȭ��ǥ ����
    for (int i = 0; i < 6; ++i)
    {
        _hackingArrows.push_back(
            _world->RegisterEntity(_hackingArrowResource->GetEntity()));
    }

    // ���� �÷��� 0,1 ����
    _hackingArrows[0]->GetComponent<HackingArrow>()->SetTargetName(
        _hackingPoints[0]->GetName());

    _hackingArrows[0]->GetComponent<ImpEngineModule::Transform>()->SetParent(
        GetLeftPlayer()->GetComponent<ImpEngineModule::Transform>());
    _hackingArrows[0]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
        ImpEngineModule::Mesh::RenderType::Left);

    _hackingArrows[1]->GetComponent<HackingArrow>()->SetTargetName(
        _hackingPoints[1]->GetName());

    _hackingArrows[1]->GetComponent<ImpEngineModule::Transform>()->SetParent(
        GetLeftPlayer()->GetComponent<ImpEngineModule::Transform>());
    _hackingArrows[1]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
        ImpEngineModule::Mesh::RenderType::Left);

    // ������ �÷��̾� 2,3 ����
    _hackingArrows[2]->GetComponent<HackingArrow>()->SetTargetName(
        _hackingPoints[2]->GetName());

    // BaseColor����
    auto baseColor2 = _hackingArrows[2]->GetComponent<Mesh>()->GetBaseColor();
    baseColor2 = PathManager::ChangeFileName(baseColor2, L"RightBaseColor");
    _hackingArrows[2]->GetComponent<Mesh>()->SetBaseColor(baseColor2);

    _hackingArrows[2]->GetComponent<ImpEngineModule::Transform>()->SetParent(
        GetRightPlayer()->GetComponent<ImpEngineModule::Transform>());
    _hackingArrows[2]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
        ImpEngineModule::Mesh::RenderType::Right);

    _hackingArrows[3]->GetComponent<HackingArrow>()->SetTargetName(
        _hackingPoints[3]->GetName());
    _hackingArrows[3]->GetComponent<Mesh>()->SetBaseColor(baseColor2);

    _hackingArrows[3]->GetComponent<ImpEngineModule::Transform>()->SetParent(
        GetRightPlayer()->GetComponent<ImpEngineModule::Transform>());
    _hackingArrows[3]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
        ImpEngineModule::Mesh::RenderType::Right);

    // �������� Mesh ��Ȱ��ȭ�ϰ� �÷��̾�1,2���� �����Ѵ�.

    // 4���� �����̴�.
    _hackingArrows[4]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
        ImpEngineModule::Mesh::RenderType::Clear);
    _hackingArrows[4]->GetComponent<ImpEngineModule::Transform>()->SetParent(
        GetLeftPlayer()->GetComponent<ImpEngineModule::Transform>());

    // 5���� �������̴�
    _hackingArrows[5]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
        ImpEngineModule::Mesh::RenderType::Clear);
    _hackingArrows[5]->GetComponent<ImpEngineModule::Transform>()->SetParent(
        GetRightPlayer()->GetComponent<ImpEngineModule::Transform>());
    _hackingArrows[5]->GetComponent<Mesh>()->SetBaseColor(baseColor2);

    // �������� ���� ��ŷ����Ʈ�� ����Ų��.
    _hackingArrows[4]->GetComponent<HackingArrow>()->SetTargetName(
        _hackingPoints[4]->GetName());
    _hackingArrows[5]->GetComponent<HackingArrow>()->SetTargetName(
        _hackingPoints[4]->GetName());

    /// ========== �÷��̾��� ��Ŀ� ������ �����ʱ� ���ؼ� �� �����ϰ��� ���Ѵ�.
    Vector3 scale = _leftPlayer->GetComponent<Transform>()->GetScale();
    Vector3 arrowScale = _hackingArrows[0]->GetComponent<Transform>()->GetScale();

    arrowScale.x /= scale.x;
    arrowScale.y /= scale.y;
    arrowScale.z /= scale.z;

    for (int i = 0; i < 6; ++i)
    {
        _hackingArrows[i]->GetComponent<Transform>()->SetScale(arrowScale);
    }
}


void ImpClient::GameManager::LoadQTEResource()
{
    _qteResource = _world->GetResourceManager()->GetEntityResource(_qtePrefab);
}


void ImpClient::GameManager::LoadHackableUI()
{
    auto res = _world->GetResourceManager()->GetEntityResource(_hackableUIPrefab);

    if (res == nullptr) return;

    auto rect    = GetWorld()->GetInputManager()->GetScreenRect();
    float width  = static_cast<float>(rect.right - rect.left);
    float height = static_cast<float>(rect.bottom - rect.top);

    // UI ��ġ ����
    _leftHackableUI = GetWorld()->RegisterEntity(res->GetEntity());
    auto leftUI     = _leftHackableUI->GetComponent<UI>();
    leftUI->SetUIPostition(width * 0.25f - 20.f, height * 0.3f - 20.f);
    leftUI->SetAlpha(0.f);

    _rightHackableUI = GetWorld()->RegisterEntity(res->GetEntity());
    auto rightUI     = _rightHackableUI->GetComponent<UI>();
    auto path        = rightUI->GetTexturePath();
    path             = PathManager::ChangeFileName(path, L"Interact2");
    rightUI->SetTexturePath(path);

    rightUI->SetUIPostition(width * 0.75f - 20.f, height * 0.3f - 20.f);
    rightUI->SetAlpha(0.f);
}

void ImpClient::GameManager::EndGame()
{
    // �¸� ���� ���
    auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

    size_t audioIndex = audioClip->GetAudioIndex(L"SE_Victory");
    audioClip->PlaySound(audioIndex, false, audioIndex);

    //
    //    ĳ���� ����
    //
    if (_leftHackingCount == 5)
    {
        Vicory(_leftPlayer);  // �¸� ����
        Defeat(_rightPlayer); // �й� ����
    }
    else if (_rightHackingCount == 5)
    {
        Vicory(_rightPlayer); // �¸� ����
        Defeat(_leftPlayer);  // �й� ����
    }

    // ȭ��ǥ �����
    for (auto& arrow : _hackingArrows)
    {
        arrow->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
            ImpEngineModule::Mesh::RenderType::Clear);
    }

    //GetWorld()->Each<Transform, Light>(
    //    [this](Entity* ent,
    //        ComponentHandle<Transform> transform,
    //        ComponentHandle<Light> light)
    //    {
    //        if (light->GetLightType() == Light::LightType::Spot))
    //            {
    //            }

    //        light->SetIntensity(0.f);
    //    },
    //    false);
    //

    _isEnd = true;
}

void ImpClient::GameManager::LoadHackingCountUI()
{
    assert(_leftPlayer);
    assert(_rightPlayer);

    auto rect    = GetWorld()->GetInputManager()->GetScreenRect();
    float width  = static_cast<float>(rect.right - rect.left);
    float height = static_cast<float>(rect.bottom - rect.top);
    auto res     = GetWorld()->GetResourceManager()->GetEntityResource(
        _hackingCountPrefab);

    // leftPlayer
    _leftHackingCountUI = GetWorld()->RegisterEntity(res->GetEntity());

    auto leftHackingCount = _leftHackingCountUI->GetComponent<HackingCountUI>();
    leftHackingCount->SetHackingUI(_leftPlayer, _hackingPoints);
    auto leftUI = _leftHackingCountUI->GetComponent<UI>();

    Vector4 uiPos = leftUI->GetUIPosition();

    leftUI->SetUIPostition(width * 0.5f - uiPos.w, height * 0.74f);

    // rightPlayer
    _rightHackingCountUI = GetWorld()->RegisterEntity(res->GetEntity());
    auto rightHackinCount = _rightHackingCountUI->GetComponent<HackingCountUI>();
    rightHackinCount->SetHackingUI(_rightPlayer, _hackingPoints);
    auto rightUI = _rightHackingCountUI->GetComponent<UI>();

    rightUI->SetUIPostition(width * 0.5f, height * 0.74f);
}

void ImpClient::GameManager::Vicory(ImpEngineModule::Entity* winner)
{
    winner->GetComponent<PlayerMovement>()->SetStopInputTime(1000.f);

    // ĳ���� �ִϸ��̼�
    PlayerState* state = winner->GetComponent<PlayerState>();
    state->SetActionState(PlayerState::ActionState::Idle);
    state->SetMoveState(PlayerState::MoveState::Dance);

    // ĳ���� ��ġ
    Transform* transform = winner->GetComponent<Transform>();
    transform->SetPosition(Vector3{ 6.f, 11.5f, -46.5f });
    transform->SetRotation(Quaternion{ 0.f, 0.5f, 0.0f, 1.f });

    // ĳ���� ���̴��� ����
    SkeletalMesh* mesh = winner->GetComponent<ImpEngineModule::SkeletalMesh>();
    mesh->SetRenderType(ImpEngineModule::SkeletalMesh::RenderType::Default);
}

void ImpClient::GameManager::Defeat(ImpEngineModule::Entity* loser)
{
    loser->GetComponent<PlayerMovement>()->SetStopInputTime(1000.f);

    // ĳ���� �ִϸ��̼�
    PlayerState* state = loser->GetComponent<PlayerState>();
    state->SetActionState(PlayerState::ActionState::Idle);
    state->SetMoveState(PlayerState::MoveState::Defeat);

    // ĳ���� ��ġ
    Transform* transform = loser->GetComponent<Transform>();
    transform->SetPosition(Vector3{ 3.f, 11.5f, -44.f });
    transform->SetRotation(Quaternion{ 0.f, 0.5f, 0.0f, 1.f });

    // ĳ���� ���̴��� ����
    SkeletalMesh* mesh = loser->GetComponent<ImpEngineModule::SkeletalMesh>();
    mesh->SetRenderType(ImpEngineModule::SkeletalMesh::RenderType::Default);
}

ImpEngineModule::Entity* ImpClient::GameManager::GetLeftHackableUI()
{
    return _leftHackableUI;
}

ImpEngineModule::Entity* ImpClient::GameManager::GetRightHackableUI()
{
    return _rightHackableUI;
}
