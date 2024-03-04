#include "ImpClientPCH.h"
#include "HackingScreen.h"

ImpClient::HackingScreen::HackingScreen()
    : _elapsedTime(0.f)
    , _isSet(false)
    , _isLeft(true)
    , _onSecondScreen(false)
    , _onNoise(false)
    , _hackingScreenUI1(nullptr)
    , _hackingScreenUI2(nullptr)
{
}

ImpClient::HackingScreen::~HackingScreen()
{
}

ImpEngineModule::Component* ImpClient::HackingScreen::Clone(
    Component* clone /*= nullptr*/) const
{
    HackingScreen* screen = static_cast<HackingScreen*>(clone);

    if (screen == nullptr) // 새로 생성해서 복사본을 준다
    {
        screen = new HackingScreen(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *screen = *this;
    }

    screen->_hackingScreenUI1 = nullptr;
    screen->_hackingScreenUI2 = nullptr;

    return screen;
}

void ImpClient::HackingScreen::Update(float dt)
{
    SetUp();

    _elapsedTime += dt;

    constexpr float changeTime = 0.35f;

    // ui를 교환한다
    if (changeTime <= _elapsedTime)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 7);

        _onNoise = static_cast<bool>(dis(gen));
        _onNoise = !_onNoise;

        if (_onSecondScreen)
        {
            _hackingScreenUI1->SetAlpha(1.f);
            _hackingScreenUI1->SetnNoise(_onNoise);

            _hackingScreenUI2->SetAlpha(0.f);
            _hackingScreenUI2->SetnNoise(false);
        }
        else
        {
            _hackingScreenUI1->SetAlpha(0.f);
            _hackingScreenUI1->SetnNoise(false);

            _hackingScreenUI2->SetAlpha(1.f);
            _hackingScreenUI2->SetnNoise(_onNoise);
        }

        _elapsedTime -= changeTime;
        _onSecondScreen = !_onSecondScreen;
    }
}

void ImpClient::HackingScreen::SetUp()
{
    if (_isSet) return;

    auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

    size_t audioIndex = audioClip->GetAudioIndex(L"SE_Noise");
    audioClip->PlaySound(audioIndex, true, audioIndex);

    auto res = GetWorld()->GetResourceManager()->GetEntityResource(
        _hackingScreen2Prefab);

    auto ent = GetWorld()->RegisterEntity(res->GetEntity());

    _hackingScreenUI2 = ent->GetComponent<UI>();
    _hackingScreenUI2->GetComponent<Transform>()->SetParent(
        GetComponent<Transform>());

    _hackingScreenUI1 = GetComponent<UI>();

    auto uiPos = _hackingScreenUI1->GetUIPosition();

    _hackingScreenUI2->SetUISize(uiPos.z, uiPos.w);

    if (!_isLeft)
    {
        auto ui2path = _hackingScreenUI2->GetTexturePath();
        ui2path      = PathManager::ChangeFileName(ui2path, L"HackingScreenR2");
        _hackingScreenUI2->SetTexturePath(ui2path);

        auto ui1Path = PathManager::ChangeFileName(ui2path, L"HackingScreenR1");
        _hackingScreenUI1->SetTexturePath(ui1Path);
    }

    _isSet = true;
}
