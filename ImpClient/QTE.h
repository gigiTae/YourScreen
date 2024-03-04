#pragma once


namespace ImpClient
{
class HackableObject;
class ReadyHacking;

/// <summary>
/// QTE 해킹 이벤트를 처리한다.
/// </summary>
class QTE: public ImpEngineModule::Component
{
    enum class QTEState
    {
        SturnQTE,   // 피격당할때 QTE
        HackingQTE, // 해킹할때 QTE
    };

    enum class QTEButton
    {
        DpadUp,
        DpadDown,
        DpadLeft,
        DpadRight,
        A,
        B,
        X,
        Y,
        End,
    };

public:
    QTE();
    ~QTE();

    void Start() override;
    void Update(float dt) override;

    Component* Clone(Component* clone) const override;
    // D-Pad + ABXY 버튼 8개

    // QTE 이벤트중에 범위밖으로 나가는 경우
    void ExitQTE();

    // 플레이어와 해킹 오브젝트 연결
    void Set(ImpEngineModule::Entity* player,
        HackableObject* hackingObject,
        ReadyHacking* readyHack = nullptr);

private:
    void UpdateHacknigQTE(float dt);
    void UpdateStrunQTE(float dt);

    QTEButton GetCurrentQTEButton();
    void CheckButton(QTEButton button);

    void MakeHackingScreen();
    void MakeRandomButton();
    void MakeButtonUI(QTEButton button);
    void MakeTimbarUI();

    void FailQTE();
    void ClearQTE();

    void UpdateBarUI(float dt);
    void UpdateQTEButtonUI();
    void UpdateTimeBarUI();
    void SetQTEBarRed();

private:
    QTEState _state;

    unsigned int _maxButton;
    unsigned int _currentIndex;
    unsigned int _padID;

    float _timeLimit;   // 제한 시간
    float _elapsedTime; // 경과 시간

    std::vector<QTEButton> _qteButtonQueue;        // 버튼 큐
    std::list<ImpEngineModule::UI*> _qteUIButtons; // 버튼UI
    unsigned int _uiButtonIndex;

    ImpEngineModule::Entity* _player; // 현재 해킹하고 있는 플레이어
    HackableObject* _hackingObject;   // 해킹하는 대상 오브젝트

    std::wstring _buttonPrefab;
    std::wstring _hackingScreenPrefab;
    ImpEngineModule::Entity* _hackingScreen;

    std::wstring _timebarPrefab;
    ImpEngineModule::Entity* _timebar;
    float _redBarElapsedTime;
    bool _isRedBar;

    ReadyHacking* _readyHacking;

    REFLECTION_FRIEND(QTE);
};

} // namespace ImpClient