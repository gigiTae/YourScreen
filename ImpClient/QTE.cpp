#include "ImpClientPCH.h"
#include "QTE.h"
#include "HackableObject.h"
#include "PlayerState.h"
#include "GamePad.h"
#include "PlayerMovement.h"
#include "QTEButtonEffect.h"
#include "QTETimeBar.h"
#include "HackingScreen.h"
#include "ReadyHacking.h"

ImpClient::QTE::QTE()
    : _currentIndex(0)
    , _maxButton(10)
    , _qteButtonQueue{}
    , _timeLimit(15.f)
    , _padID(-1)
    , _elapsedTime(0.f)
    , _hackingObject(nullptr)
    , _player(nullptr)
    , _uiButtonIndex(0)
    , _timebar(nullptr)
    , _hackingScreenPrefab{}
    , _timebarPrefab{}
    , _hackingScreen(nullptr)
    , _isRedBar(false)
    , _redBarElapsedTime(0.f)
    , _state(QTEState::HackingQTE)
    , _readyHacking(nullptr)
{
}

ImpClient::QTE::~QTE()
{
}


void ImpClient::QTE::Start()
{
    MakeRandomButton();

    _elapsedTime  = 0.f;
    _currentIndex = 0;

    MakeTimbarUI();

    if (_state == QTEState::SturnQTE)
    {
        MakeHackingScreen();
    }

    if (_padID == ImpClientDefine::rightPlayer)
    {
        auto ui   = GetComponent<UI>();
        auto path = ui->GetTexturePath();
        path      = PathManager::ChangeFileName(path, L"QTEBarR");
        ui->SetTexturePath(path);
    }
}


void ImpClient::QTE::Update(float dt)
{
    if (_state == QTEState::HackingQTE)
    {
        UpdateHacknigQTE(dt);
    }
    else
    {
        UpdateStrunQTE(dt);
    }
}

ImpEngineModule::Component* ImpClient::QTE::Clone(Component* clone) const
{
    QTE* qte = static_cast<QTE*>(clone);

    if (qte == nullptr) // 새로 생성해서 복사본을 준다
    {
        qte = new QTE(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *qte = *this;
    }

    qte->_player = nullptr;

    return qte;
}

void ImpClient::QTE::UpdateHacknigQTE(float dt)
{
    _elapsedTime += dt;
    UpdateBarUI(dt);

    if (_timeLimit <= _elapsedTime)
    {
        // 해킹 실패 사운드 재생
        auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

        size_t audioIndex = audioClip->GetAudioIndex(L"SE_HakingFail");
        audioClip->PlaySound(audioIndex, false, audioIndex);

        FailQTE();
        return;
    }

    QTEButton currentButton = GetCurrentQTEButton();
    CheckButton(currentButton);

    UpdateQTEButtonUI();
    UpdateTimeBarUI();
}


void ImpClient::QTE::UpdateStrunQTE(float dt)
{
    _elapsedTime += dt;
    UpdateBarUI(dt);

    if (_timeLimit <= _elapsedTime)
    {

        // 해킹 탈출
        auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

        size_t audioIndex = audioClip->GetAudioIndex(L"SE_Noise");
        SoundManager::GetInstance()->StopChannel(audioIndex);

        ExitQTE();
        return;
    }

    QTEButton currentButton = GetCurrentQTEButton();
    CheckButton(currentButton);

    UpdateQTEButtonUI();
    UpdateTimeBarUI();
}

ImpClient::QTE::QTEButton ImpClient::QTE::GetCurrentQTEButton()
{
    auto input = GetWorld()->GetInputManager();

    QTEButton currentButton = QTEButton::End;

    // up
    if (input->IsPadKeyState(_padID, PadKey::DpadUp, KeyState::Tap))
    {
        currentButton = QTEButton::DpadUp;
    }
    // down
    else if (input->IsPadKeyState(_padID, PadKey::DpadDown, KeyState::Tap))
    {
        currentButton = QTEButton::DpadDown;
    }
    // left
    else if (input->IsPadKeyState(_padID, PadKey::DpadLeft, KeyState::Tap))
    {
        currentButton = QTEButton::DpadLeft;
    }
    // right
    else if (input->IsPadKeyState(_padID, PadKey::DpadRight, KeyState::Tap))
    {
        currentButton = QTEButton::DpadRight;
    }
    // A
    else if (input->IsPadKeyState(_padID, PadKey::A, KeyState::Tap))
    {
        currentButton = QTEButton::A;
    }
    // B
    else if (input->IsPadKeyState(_padID, PadKey::B, KeyState::Tap))
    {
        currentButton = QTEButton::B;
    }
    // X
    else if (input->IsPadKeyState(_padID, PadKey::X, KeyState::Tap))
    {
        currentButton = QTEButton::X;
    }
    // Y
    else if (input->IsPadKeyState(_padID, PadKey::Y, KeyState::Tap))
    {
        currentButton = QTEButton::Y;
    }

    return currentButton;
}

void ImpClient::QTE::MakeRandomButton()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(
        static_cast<int>(QTEButton::DpadUp), static_cast<int>(QTEButton::Y));
    _qteButtonQueue.clear();
    // maxButton개의 랜덤 버튼 생성
    for (unsigned int i = 0; i < _maxButton; ++i)
    {
        _qteButtonQueue.push_back(static_cast<QTEButton>(dis(gen)));
    }
}


void ImpClient::QTE::CheckButton(QTEButton button)
{
    constexpr float timePenalty = 2.f;
    // 아무것도 누르지 않은 경우
    if (button == QTEButton::End)
    {
        return;
    }

    // Sound 재생
    auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

    if (_state == QTEState::HackingQTE)
    {
        // 일치하게 누른 경우
        if (button == _qteButtonQueue[_currentIndex])
        {
            ++_currentIndex;

            // ButtonUI 삭제
            constexpr float upSpeed = -200.f;

            _qteUIButtons.front()->GetComponent<QTEButtonEffect>()->Destory(
                0.5f, upSpeed);
            _qteUIButtons.pop_front();

            // QTE 성공 Sound 재생
            size_t audioIndex = audioClip->GetAudioIndex(L"SE_QTESuccess");
            audioClip->PlaySound(audioIndex, false, audioIndex);
        }
        else // 틀린 경우
        {
            _elapsedTime += timePenalty;
            _timebar->GetComponent<QTETimeBar>()->SetRed();
            SetQTEBarRed();

            GetWorld()->GetInputManager()->SetVibration(
                _padID, VibrationMode::Left, 10000, 0.25f);

            // QTE 실패 Sound 재생
            size_t audioIndex = audioClip->GetAudioIndex(L"SE_QTEFail");
            audioClip->PlaySound(audioIndex, false, audioIndex);
        }

        if (_currentIndex == _maxButton)
        {
            ClearQTE();

            // 해킹 성공 사운드 재생
            auto audioClip = GetWorld()
                                 ->GetEntity("AudioManager")
                                 ->GetComponent<AudioClip>();

            size_t audioIndex = audioClip->GetAudioIndex(L"SE_HakingSuccess");
            audioClip->PlaySound(audioIndex, false, audioIndex);
        }
    }
    else if (_state == QTEState::SturnQTE)
    {
        // 일치하게 누른 경우
        if (button == _qteButtonQueue[_currentIndex])
        {
            // 시간 감소
            _elapsedTime = std::clamp(_elapsedTime + timePenalty, 0.f, _timeLimit);

            ++_currentIndex;

            // ButtonUI 삭제
            constexpr float upSpeed = -200.f;

            _qteUIButtons.front()->GetComponent<QTEButtonEffect>()->Destory(
                0.5f, upSpeed);
            _qteUIButtons.pop_front();

            // QTE 성공 Sound 재생
            size_t audioIndex = audioClip->GetAudioIndex(L"SE_QTESuccess");
            audioClip->PlaySound(audioIndex, false, audioIndex);
        }
        else // 틀린 경우
        {
            _elapsedTime = std::clamp(_elapsedTime - timePenalty, 0.f, _timeLimit);

            _timebar->GetComponent<QTETimeBar>()->SetRed();
            SetQTEBarRed();

            GetWorld()->GetInputManager()->SetVibration(
                _padID, VibrationMode::Left, 10000, 0.25f);

            // QTE 실패 Sound 재생
            size_t audioIndex = audioClip->GetAudioIndex(L"SE_QTEFail");
            audioClip->PlaySound(audioIndex, false, audioIndex);
        }

        // 탈출
        if (_currentIndex == _maxButton)
        {
            ExitQTE();
        }
    }
}


void ImpClient::QTE::MakeHackingScreen()
{
    if (_hackingScreenPrefab.empty()) return;

    _hackingScreen = GetWorld()->RegisterEntity(
        GetWorld()
            ->GetResourceManager()
            ->GetEntityResource(_hackingScreenPrefab)
            ->GetEntity());

    GetComponent<Transform>()->AddChild(_hackingScreen->GetComponent<Transform>());

    // HackingScreen 위치,사이즈 결정
    auto input = GetWorld()->GetInputManager();
    auto rect  = input->GetScreenRect();

    float screenWidth  = static_cast<float>(rect.right - rect.left);
    float screenHeight = static_cast<float>(rect.bottom - rect.top);
    auto ui            = GetComponent<UI>();
    auto screenUI      = _hackingScreen->GetComponent<UI>();

    if (_padID == ImpClientDefine::rightPlayer)
    {
        auto path = screenUI->GetTexturePath();
        path      = PathManager::ChangeFileName(path, L"HackingScreenR1");
        screenUI->SetTexturePath(path);
        _hackingScreen->GetComponent<HackingScreen>()->SetIsLeft(false);
    }

    screenUI->SetnNoise(true);

    auto uiPos = ui->GetUIPosition();

    screenUI->SetUISize(screenWidth * 0.5f, screenHeight);

    if (_padID == ImpClientDefine::leftPlayer)
    {
        screenUI->SetUIPostition(-uiPos.x, -uiPos.y);
    }
    else if (_padID == ImpClientDefine::rightPlayer)
    {
        screenUI->SetUIPostition(-uiPos.x + screenWidth * 0.5f, -uiPos.y);
    }
}

void ImpClient::QTE::ExitQTE()
{
    _player->GetComponent<PlayerState>()->SetActionState(
        PlayerState::ActionState::Idle);

    auto aniClip = _player->GetComponent<AnimationClip>();
    aniClip->PlayAnimation(
        aniClip->GetAnimations()[static_cast<int>(PlayerState::AnimClipIndex::Idle)]);

    if (_padID == ImpClientDefine::leftPlayer)
    {
        _player->GetComponent<SkeletalMesh>()->SetRenderType(
            SkeletalMesh::RenderType::Left);
    }
    else
    {
        _player->GetComponent<SkeletalMesh>()->SetRenderType(
            SkeletalMesh::RenderType::Right);
    }

    _player->GetComponent<PlayerMovement>()->SetQTE(nullptr);

    GetWorld()->DestroyEntity(GetEntity());

    if (_readyHacking)
    {
        _readyHacking->FailQTE(_padID);
    }
}


void ImpClient::QTE::Set(ImpEngineModule::Entity* player,
    HackableObject* hackingObject,
    ReadyHacking* readyHack)
{
    using namespace ImpEngineModule;

    _readyHacking  = readyHack;
    _player        = player;
    _hackingObject = hackingObject;
    _padID         = _player->GetComponent<GamePad>()->GetPadID();

    if (_readyHacking != nullptr)
    {
        _state == QTEState::HackingQTE;
        _maxButton = 5;
        _timeLimit = 20.f;
    }
    else if (_hackingObject == nullptr)
    {
        _state     = QTEState::SturnQTE;
        _maxButton = 1000;
        _timeLimit = 30.f;
    }
    else
    {
        _state = QTEState::HackingQTE;
    }

    // QTE UI 위치 설정
    auto input = GetWorld()->GetInputManager();
    RECT rect  = input->GetScreenRect();

    long width  = rect.right - rect.left;
    long height = rect.bottom - rect.top;

    auto ui    = GetComponent<UI>();
    auto uiPos = ui->GetUIPosition();

    if (_padID == ImpClientDefine::leftPlayer)
    {
        if (_state == QTEState::HackingQTE)
        {
            // 왼쪽중앙
            ui->SetUIPostition(
                width * 0.25f - uiPos.z * 0.5f, height * 0.3f - uiPos.w);
        }
        else
        {
            ui->SetUIPostition(
                width * 0.25f - uiPos.z * 0.5f, height * 0.8f - uiPos.w);
        }
    }
    else if (_padID == ImpClientDefine::rightPlayer)
    {
        if (_state == QTEState::HackingQTE)
        {
            ui->SetUIPostition(
                width * 0.75f - uiPos.z * 0.5f, height * 0.3f - uiPos.w);
        }
        else
        {
            ui->SetUIPostition(
                width * 0.75f - uiPos.z * 0.5f, height * 0.8f - uiPos.w);
        }
    }
}

void ImpClient::QTE::ClearQTE()
{
    _player->GetComponent<PlayerMovement>()->SetQTE(nullptr);
    GetWorld()->DestroyEntity(GetEntity());
    
    if (_readyHacking)
    {
        _readyHacking->ClearQTE(_padID);
        return;
    }

    // 해킹 성공
    _hackingObject->HackObject(_player);
    _player->GetComponent<PlayerState>()->SetActionState(
        PlayerState::ActionState::Idle);
}


void ImpClient::QTE::MakeButtonUI(QTEButton button)
{
    using namespace ImpEngineModule;

    auto buttonRes = GetWorld()->GetResourceManager()->GetEntityResource(
        _buttonPrefab);

    Entity* ent = GetWorld()->RegisterEntity(buttonRes->GetEntity());

    ent->GetComponent<Transform>()->SetParent(GetComponent<Transform>());
    auto ui = ent->GetComponent<UI>();
    _qteUIButtons.push_back(ui);

    std::wstring path = ui->GetTexturePath();
    int index         = 0;

    // 경로를 QTEButton에 해당하는 값으로 변경
    switch (button)
    {
        case ImpClient::QTE::QTEButton::DpadUp:
        {
            index = 0;
        }
        break;
        case ImpClient::QTE::QTEButton::DpadDown:
        {
            index = 1;
        }
        break;
        case ImpClient::QTE::QTEButton::DpadLeft:
        {
            index = 2;
        }
        break;
        case ImpClient::QTE::QTEButton::DpadRight:
        {
            index = 3;
        }
        break;
        case ImpClient::QTE::QTEButton::A:
        {
            index = 4;
        }
        break;
        case ImpClient::QTE::QTEButton::B:
        {
            index = 5;
        }
        break;
        case ImpClient::QTE::QTEButton::X:
        {
            index = 6;
        }
        break;
        case ImpClient::QTE::QTEButton::Y:
        {
            index = 7;
        }
        break;
        default:
            break;
    }

    // 노란색UI
    if (_padID == ImpClientDefine::rightPlayer)
    {
        index += 10;
    }

    path = PathManager::ChangeFileName(path, std::to_wstring(index));

    ui->SetTexturePath(path);
    ui->SetUISize(70.f, 70.f);
}


void ImpClient::QTE::MakeTimbarUI()
{
    if (_timebarPrefab.empty()) return;

    _timebar = GetWorld()->RegisterEntity(
        GetWorld()->GetResourceManager()->GetEntityResource(_timebarPrefab)->GetEntity());

    _timebar->GetComponent<QTETimeBar>()->SetPadID(_padID);

    // 부모 자신관계 연결
    _timebar->GetComponent<ImpEngineModule::Transform>()->SetParent(
        GetComponent<ImpEngineModule::Transform>());

    auto ui = _timebar->GetComponent<ImpEngineModule::UI>();
    ui->SetUIPostition(0.f, -50.f);
}


void ImpClient::QTE::UpdateBarUI(float dt)
{
    if (!_isRedBar) return;

    _redBarElapsedTime += dt;

    constexpr float redBarTime = 0.25f;

    if (redBarTime <= _redBarElapsedTime)
    {
        auto ui           = GetComponent<ImpEngineModule::UI>();
        std::wstring path = ui->GetTexturePath();

        std::filesystem::path filePath(path);
        std::wstring normalBarPath = filePath.parent_path();

        if (_padID == ImpClientDefine::leftPlayer)
        {
            normalBarPath += L"\\QTEBar.png";
        }
        else
        {
            normalBarPath += L"\\QTEBarR.png";
        }

        ui->SetTexturePath(normalBarPath);
        _redBarElapsedTime = 0.f;

        _isRedBar = true;
    }
}

void ImpClient::QTE::UpdateQTEButtonUI()
{
    // 새로운 버튼을 생성한다

    while (_qteUIButtons.size() < 5 && _uiButtonIndex < _qteButtonQueue.size())
    {
        MakeButtonUI(_qteButtonQueue[_uiButtonIndex]);
        ++_uiButtonIndex;
    }

    int i = 0;
    for (auto ui : _qteUIButtons)
    {
        auto uiPos = ui->GetUIPosition();

        constexpr float gap = 100.f;

        ui->SetUIPostition(15.f + gap * i, 15.f);
        ++i;
    }
}


void ImpClient::QTE::UpdateTimeBarUI()
{
    float ratio = 1.f - _elapsedTime / _timeLimit;

    _timebar->GetComponent<QTETimeBar>()->Set(ratio);
}


void ImpClient::QTE::SetQTEBarRed()
{
    auto ui           = GetComponent<ImpEngineModule::UI>();
    std::wstring path = ui->GetTexturePath();

    std::filesystem::path filePath(path);
    std::wstring redBarPath = filePath.parent_path();
    redBarPath += L"\\QTEBarRed.png";

    ui->SetTexturePath(redBarPath);
    _redBarElapsedTime = 0.f;

    _isRedBar = true;
}

void ImpClient::QTE::FailQTE()
{
    // 해킹 실패
    _player->GetComponent<PlayerState>()->SetActionState(
        PlayerState::ActionState::Idle);

    _player->GetComponent<PlayerMovement>()->SetQTE(nullptr);
    GetWorld()->DestroyEntity(GetEntity());

    if (_readyHacking)
    {
        _readyHacking->FailQTE(_padID);
    }
}
