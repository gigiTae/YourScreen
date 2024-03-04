#pragma once


namespace ImpClient
{
class HackableObject;
class ReadyHacking;

/// <summary>
/// QTE ��ŷ �̺�Ʈ�� ó���Ѵ�.
/// </summary>
class QTE: public ImpEngineModule::Component
{
    enum class QTEState
    {
        SturnQTE,   // �ǰݴ��Ҷ� QTE
        HackingQTE, // ��ŷ�Ҷ� QTE
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
    // D-Pad + ABXY ��ư 8��

    // QTE �̺�Ʈ�߿� ���������� ������ ���
    void ExitQTE();

    // �÷��̾�� ��ŷ ������Ʈ ����
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

    float _timeLimit;   // ���� �ð�
    float _elapsedTime; // ��� �ð�

    std::vector<QTEButton> _qteButtonQueue;        // ��ư ť
    std::list<ImpEngineModule::UI*> _qteUIButtons; // ��ưUI
    unsigned int _uiButtonIndex;

    ImpEngineModule::Entity* _player; // ���� ��ŷ�ϰ� �ִ� �÷��̾�
    HackableObject* _hackingObject;   // ��ŷ�ϴ� ��� ������Ʈ

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