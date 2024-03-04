#include "ImpClientPCH.h"
#include "GameManager.h"
#include "HackableObject.h"
#include "HackingArrow.h"
#include "HackingCountUI.h"

// 게임 승리 시 캐릭터 애니메이션을 위해 추가 (2024-02-21 김주영)
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
        // 해킹 가능한 오브젝트들을 월드에서 가져온다.
        if (ent->HasComponent<HackableObject>())
        {
            _hackableObjects.push_back(ent.get());
        }

        // 왼쪽 플레이어를 가져온다
        if (ent->GetName() == _leftPlayerName)
        {
            _leftPlayer = ent.get();
        }

        // 오른쪽 플레이어를 가져온다
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

    // 왼쪽플레이어가 4개를 해킹한경우 오른쪽 플레이어의 화살표를 활성화하고 해킹가능으로 설정한다.
    if (_leftHackingCount == 4)
    {
        _hackingArrows[5]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
            ImpEngineModule::Mesh::RenderType::Right);

        // 해킹 포인트를 활성화한다.
        _hackingPoints[4]->GetComponent<HackableObject>()->SetHackingPoint(true);
    }

    // 왼쪽 플레이어 승리
    if (_leftHackingCount == 5)
    {
        EndGame();
    }
}


void ImpClient::GameManager::AddRightPlayerHackingCount()
{
    ++_rightHackingCount;

    // 오른쪽 플레이어가 4개를 해킹한 경우
    if (_rightHackingCount == 4)
    {
        _hackingArrows[4]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
            ImpEngineModule::Mesh::RenderType::Left);

        // 해킹 포인트를 활성화 한다.
        _hackingPoints[4]->GetComponent<HackableObject>()->SetHackingPoint(true);
    }

    // 오른쪽 플레이어 승리
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
    // 해킹 가능한 오브젝트들중에서 랜덤으로 5개를 선택한다.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(
        0, static_cast<int>(_hackableObjects.size() - 1));

    int count      = 0;
    int errorCount = 0;
    while (count != 5)
    {
        // 초기 설정 에러 방지용 코드
        ++errorCount;
        if (errorCount >= 10000000)
        {
            assert(
                !"해킹포인트를 Quick 2개 Scale 2개 Victory 1개를 설정해야합니다");
        }

        size_t randomIndex  = dis(gen);
        auto hackalbeObject = _hackableObjects[randomIndex]
                                  ->GetComponent<HackableObject>();

        // 이미 해킹포인트로 선택된경우
        if (hackalbeObject->IsHackingPoint())
        {
            continue;
        }

        // 0,3번은 Qucik 고정한다
        if (count == 0 || count == 3)
        {
            if (hackalbeObject->GetAbility() != HackableObject::SkillAbility::Qucik)
            {
                continue;
            }
            hackalbeObject->SetHackingPoint(true);
        }
        // 1,2번은 Scale 고정한다
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
            // 마지막 해킹포인트는 Victory로 성정해야한다.
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

    // 리소스 가져오기
    _hackingArrowResource = _world->GetResourceManager()->GetEntityResource(
        _hackingArrowPrefab);

    // 해킹화살표 생성
    for (int i = 0; i < 6; ++i)
    {
        _hackingArrows.push_back(
            _world->RegisterEntity(_hackingArrowResource->GetEntity()));
    }

    // 왼쪽 플레어 0,1 연결
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

    // 오른쪽 플레이어 2,3 연결
    _hackingArrows[2]->GetComponent<HackingArrow>()->SetTargetName(
        _hackingPoints[2]->GetName());

    // BaseColor변경
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

    // 마지막을 Mesh 비활성화하고 플레이어1,2에게 장착한다.

    // 4번이 왼쪽이다.
    _hackingArrows[4]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
        ImpEngineModule::Mesh::RenderType::Clear);
    _hackingArrows[4]->GetComponent<ImpEngineModule::Transform>()->SetParent(
        GetLeftPlayer()->GetComponent<ImpEngineModule::Transform>());

    // 5번이 오른쪽이다
    _hackingArrows[5]->GetComponent<ImpEngineModule::Mesh>()->SetRenderType(
        ImpEngineModule::Mesh::RenderType::Clear);
    _hackingArrows[5]->GetComponent<ImpEngineModule::Transform>()->SetParent(
        GetRightPlayer()->GetComponent<ImpEngineModule::Transform>());
    _hackingArrows[5]->GetComponent<Mesh>()->SetBaseColor(baseColor2);

    // 마지막은 같은 해킹포인트를 가리킨다.
    _hackingArrows[4]->GetComponent<HackingArrow>()->SetTargetName(
        _hackingPoints[4]->GetName());
    _hackingArrows[5]->GetComponent<HackingArrow>()->SetTargetName(
        _hackingPoints[4]->GetName());

    /// ========== 플레이어의 행렬에 영향을 받지않기 위해서 역 스케일값을 곱한다.
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

    // UI 위치 설정
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
    // 승리 사운드 재생
    auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

    size_t audioIndex = audioClip->GetAudioIndex(L"SE_Victory");
    audioClip->PlaySound(audioIndex, false, audioIndex);

    //
    //    캐릭터 연출
    //
    if (_leftHackingCount == 5)
    {
        Vicory(_leftPlayer);  // 승리 연출
        Defeat(_rightPlayer); // 패배 연출
    }
    else if (_rightHackingCount == 5)
    {
        Vicory(_rightPlayer); // 승리 연출
        Defeat(_leftPlayer);  // 패배 연출
    }

    // 화살표 지우기
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

    // 캐릭터 애니메이션
    PlayerState* state = winner->GetComponent<PlayerState>();
    state->SetActionState(PlayerState::ActionState::Idle);
    state->SetMoveState(PlayerState::MoveState::Dance);

    // 캐릭터 위치
    Transform* transform = winner->GetComponent<Transform>();
    transform->SetPosition(Vector3{ 6.f, 11.5f, -46.5f });
    transform->SetRotation(Quaternion{ 0.f, 0.5f, 0.0f, 1.f });

    // 캐릭터 보이는지 여부
    SkeletalMesh* mesh = winner->GetComponent<ImpEngineModule::SkeletalMesh>();
    mesh->SetRenderType(ImpEngineModule::SkeletalMesh::RenderType::Default);
}

void ImpClient::GameManager::Defeat(ImpEngineModule::Entity* loser)
{
    loser->GetComponent<PlayerMovement>()->SetStopInputTime(1000.f);

    // 캐릭터 애니메이션
    PlayerState* state = loser->GetComponent<PlayerState>();
    state->SetActionState(PlayerState::ActionState::Idle);
    state->SetMoveState(PlayerState::MoveState::Defeat);

    // 캐릭터 위치
    Transform* transform = loser->GetComponent<Transform>();
    transform->SetPosition(Vector3{ 3.f, 11.5f, -44.f });
    transform->SetRotation(Quaternion{ 0.f, 0.5f, 0.0f, 1.f });

    // 캐릭터 보이는지 여부
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
