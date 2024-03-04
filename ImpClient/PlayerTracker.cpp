#include "ImpClientPCH.h"
#include "PlayerTracker.h"
#include "GamePad.h"
#include "PlayerState.h"

ImpClient::PlayerTracker::PlayerTracker()
    : _playerTransform(nullptr)
    , _playerName{}
    , _elapsedTime(0.f)
    , _currentIndex(0)
    , _startGame(false)
    , _onTracking(false)
    , _playerState(nullptr)
{
}

ImpClient::PlayerTracker::~PlayerTracker()
{
}

ImpEngineModule::Component* ImpClient::PlayerTracker::Clone(
    Component* clone /*= nullptr*/) const
{
    PlayerTracker* camera = static_cast<PlayerTracker*>(clone);

    if (camera == nullptr)
    {
        camera = new PlayerTracker(*this);
    }
    else
    {
        *camera = *this;
    }

    return camera;
}

void ImpClient::PlayerTracker::Make()
{
    // 추적 점들을 생성
    auto res = GetWorld()->GetResourceManager()->GetEntityResource(
        _trackingPointPrefab);

    if (res == nullptr)
    {
        return;
    }

    Transform* transform = GetComponent<Transform>();

    size_t size = static_cast<size_t>(trackingTime / timeStep);

    for (size_t i = 0; i < size; ++i)
    {
        Entity* point     = GetWorld()->RegisterEntity(res->GetEntity());
        Transform* pointT = point->GetComponent<Transform>();
        pointT->SetParent(transform);

        // 처음에는 보이지 않는다.
        Mesh* mesh = point->GetComponent<Mesh>();
        mesh->SetRenderType(Mesh::RenderType::Clear);

        _trackingPoints.push_back(point);
    }

    Entity* player = GetWorld()->GetEntity(_playerName);
    assert(player);

    _playerTransform = player->GetComponent<Transform>();

    if (player->GetComponent<GamePad>()->GetPadID() == ImpClientDefine::leftPlayer)
    {
        _playerState = GetWorld()->GetEntity("Player2")->GetComponent<PlayerState>();
    }
    else
    {
        _playerState = GetWorld()->GetEntity("Player1")->GetComponent<PlayerState>();
    }

    _startGame = true;
}

void ImpClient::PlayerTracker::Update(float dt)
{
    if (_startGame == false)
    {
        Make();
    }

    _elapsedTime += dt;

    while (_elapsedTime >= timeStep)
    {
        _elapsedTime -= timeStep;

        Entity* point = _trackingPoints[_currentIndex];

        Vector3 playerPos = _playerTransform->GetWorldPostiion();

        point->GetComponent<Transform>()->SetPosition(playerPos);
        ++_currentIndex;

        if (_currentIndex >= _trackingPoints.size())
        {
            _currentIndex = 0;
        }
    }

    auto moveState   = _playerState->GetMoveState();
    auto actionState = _playerState->GetActionState();

    if (moveState == PlayerState::MoveState::Foucs &&
        actionState == PlayerState::ActionState::Idle && !_onTracking)
    {
        On();
    }
    else if ((moveState != PlayerState::MoveState::Foucs ||
                 actionState != PlayerState::ActionState::Idle) &&
             _onTracking)
    {
        Off();
    }
}

void ImpClient::PlayerTracker::On()
{
    _onTracking = true;

    for (Entity* point : _trackingPoints)
    {
        auto mesh = point->GetComponent<Mesh>();

        auto padID = _playerTransform->GetComponent<GamePad>()->GetPadID();

        // 반대편에 정보 표시
        if (padID == ImpClientDefine::leftPlayer)
        {
            mesh->SetRenderType(Mesh::RenderType::Right);
        }
        else
        {
            mesh->SetRenderType(Mesh::RenderType::Left);
        }
    }
}

void ImpClient::PlayerTracker::Off()
{
    _onTracking = false;

    for (Entity* point : _trackingPoints)
    {
        auto mesh = point->GetComponent<Mesh>();
        mesh->SetRenderType(Mesh::RenderType::Clear);
    }
}
