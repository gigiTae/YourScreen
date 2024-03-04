#include "ImpClientPCH.h"
#include "CameraWalk.h"

#include "PlayerState.h"
#include "FadeIn.h"

ImpClient::CameraWalk::CameraWalk()
    : _points()
    , _pointsPlayTime()
    , _cameraWalkStartTime(1.0f)
    , _playerAnimationStartTime(5.0f)
    , _gameStartTime(20.0f)
    , _playTime(0.0f)
    , _fadeStartTime(0.0f)
    , _isFade(false)
{
}

ImpClient::CameraWalk::~CameraWalk()
{
}

void ImpClient::CameraWalk::Start()
{
    // ī�޶� 1�� �����ؾ���
    GetWorld()->GetEntity("SecondCamera")->GetComponent<Camera>()->OffSecondCamera();
    
    // �÷��̾� �ִϸ��̼��� L""�� ����ϸ� ������
    // �ƹ�ư �÷��̾� �Ⱥ��̰� ����
    GetWorld()->GetEntity("Player1")->GetComponent<AnimationClip>()->PlayAnimation(
        L"");
    GetWorld()->GetEntity("Player2")->GetComponent<AnimationClip>()->PlayAnimation(
        L"");
}

void ImpClient::CameraWalk::Update(float dt)
{
    _playTime += dt;

    // start���� ī�޶� 1���� ����

    if (_playTime > _gameStartTime)
    {
        if (_isFade && (_playTime > _fadeStartTime + 1.2f))
        {
            GetWorld()->GetEventManager()->ChangeWorld(L"Stage01");
        }
        // ���� ������ ��ȯ
        if (!_isFade)
        {
            _fadeStartTime = _playTime;
            _isFade        = true;
            GetWorld()->GetEntity("FadeIn")->GetComponent<FadeIn>()->StartFadeOut();
        }
    }
    if (_playTime > _playerAnimationStartTime)
    {
        if (!_isPlay)
        {
            // idle���� �÷��̾� ������? �÷��̾� �ִϸ��̼� ���
            int swingToLandIdx = (int)ImpClient::PlayerState::AnimClipIndex::SwingToLand;
            int idleIdx = (int)ImpClient::PlayerState::AnimClipIndex::Idle;

            GetWorld()->GetEntity("Player1")->GetComponent<AnimationClip>()->PlayAnimationNoneLoop(
                swingToLandIdx, idleIdx);
            GetWorld()->GetEntity("Player2")->GetComponent<AnimationClip>()->PlayAnimationNoneLoop(
                swingToLandIdx, idleIdx);

            Vector3 player1P = GetWorld()->GetEntity("Player1")->GetComponent<Transform>()->GetLocalPosition();
            Vector3 player2P = GetWorld()->GetEntity("Player2")->GetComponent<Transform>()->GetLocalPosition();

            GetWorld()->GetEntity("Player1")->GetComponent<Transform>()->SetPosition(
                { player1P.x, 10.0f, player1P.z});
            GetWorld()->GetEntity("Player2")->GetComponent<Transform>()->SetPosition(
                { player2P.x, 10.0f, player2P.z });

            //         GetWorld()->GetEntity("Player1")->GetComponent<AnimationClip>()->PlayAnimation(
            //             swingToLandIdx);
            //         GetWorld()->GetEntity("Player2")->GetComponent<AnimationClip>()->PlayAnimation(
            //             swingToLandIdx);

//             GetWorld()->GetEntity("Player1")->GetComponent<PlayerState>()->CheckOff();
//             GetWorld()->GetEntity("Player2")->GetComponent<PlayerState>()->CheckOff();

//             GetWorld()->GetEntity("Player1")->GetComponent<AnimationClip>()->SetMoveState(
//                 PlayerState::MoveState::Swing);
//             GetWorld()->GetEntity("Player2")->GetComponent<AnimationClip>()->SetMoveState(
//                 PlayerState::MoveState::Swing);
        }
        _isPlay = true;
    }
    else
    {
        Vector3 player1P = GetWorld()
                               ->GetEntity("Player1")
                               ->GetComponent<Transform>()
                               ->GetLocalPosition();
        Vector3 player2P = GetWorld()
                               ->GetEntity("Player2")
                               ->GetComponent<Transform>()
                               ->GetLocalPosition();

        GetWorld()->GetEntity("Player1")->GetComponent<Transform>()->SetPosition(
            { player1P.x, 10000.0f, player1P.z });
        GetWorld()->GetEntity("Player2")->GetComponent<Transform>()->SetPosition(
            { player2P.x, 10000.0f, player2P.z });
    }
    if (_playTime > _cameraWalkStartTime)
    {
        // �ð� ���� ����Ʈ���� ������ ī�޶� ��ġ��
        for (int i = _points.size() - 1; i >= 0; i--)
        {
            size_t lastRSlash = _pointsPlayTime[i].find_last_of(L"\\");
            if (_playTime > stof(_pointsPlayTime[i].substr(lastRSlash + 1,
                                _pointsPlayTime[i].size() - lastRSlash)))
            {
                _curPlayPointIndex = i;
                break;
            }
        }

        ImpMath::Vector3 cameraPosition;
        ImpMath::Quaternion cameraRotation;

        ImpMath::Vector3 nextCameraPosition;
        ImpMath::Quaternion nextCameraRotation;

        // ��ΰ� resource���� �Ȱ��Ƽ� next�� �׳��̳� �Ȱ���
        size_t lastTimeSlash      = _pointsPlayTime[_curPlayPointIndex].find_last_of(L"\\");
        size_t lastPointSlash = _points[_curPlayPointIndex].find_last_of(L"\\");
        std::wstring tempPoint = _points[_curPlayPointIndex].substr(lastPointSlash + 1, _points[_curPlayPointIndex].size() - lastPointSlash);
        std::wstring tempNextPoint = _points[_curPlayPointIndex + 1].substr(
            lastPointSlash + 1, _points[_curPlayPointIndex + 1].size() - lastPointSlash);
        for (size_t i = 0; i < 7; i++)
        {
            size_t firstSlash = tempPoint.find_first_of(L"@");
            size_t firstSlashNext = tempNextPoint.find_first_of(L"@");
            if (i == 0)
            {
                cameraPosition.x = stof(
                    tempPoint.substr(0, tempPoint.size() - firstSlash));
                nextCameraPosition.x = stof(tempNextPoint.substr(
                    0, tempNextPoint.size() - firstSlashNext));
            }
            else if (i == 1)
            {
                cameraPosition.y = stof(
                    tempPoint.substr(0, tempPoint.size() - firstSlash));
                nextCameraPosition.y = stof(tempNextPoint.substr(
                    0, tempNextPoint.size() - firstSlashNext));
            }
            else if (i == 2)
            {
                cameraPosition.z = stof(
                    tempPoint.substr(0, tempPoint.size() - firstSlash));
                nextCameraPosition.z = stof(tempNextPoint.substr(
                    0, tempNextPoint.size() - firstSlashNext));
            }
            else if (i == 3)
            {
                cameraRotation.x = stof(
                    tempPoint.substr(0, tempPoint.size() - firstSlash));
                nextCameraRotation.x = stof(tempNextPoint.substr(
                    0, tempNextPoint.size() - firstSlashNext));
            }
            else if (i == 4)
            {
                cameraRotation.y = stof(
                    tempPoint.substr(0, tempPoint.size() - firstSlash));
                nextCameraRotation.y = stof(tempNextPoint.substr(
                    0, tempNextPoint.size() - firstSlashNext));
            }
            else if (i == 5)
            {
                cameraRotation.z = stof(
                    tempPoint.substr(0, tempPoint.size() - firstSlash));
                nextCameraRotation.z = stof(tempNextPoint.substr(
                    0, tempNextPoint.size() - firstSlashNext));
            }
            else if (i == 6)
            {
                cameraRotation.w = stof(
                    tempPoint.substr(0, tempPoint.size() - firstSlash));
                nextCameraRotation.w = stof(tempNextPoint.substr(
                    0, tempNextPoint.size() - firstSlashNext));
            }
            tempPoint = tempPoint.substr(firstSlash + 1, tempPoint.size() - 1);
            tempNextPoint = tempNextPoint.substr(
                firstSlashNext + 1, tempNextPoint.size() - 1);
        }

        float curPointPlayTime = stof(
            _pointsPlayTime[_curPlayPointIndex].substr(lastTimeSlash + 1,
                _pointsPlayTime[_curPlayPointIndex].size() - lastTimeSlash));
        float nextPointPlayTime = stof(
            _pointsPlayTime[_curPlayPointIndex + 1].substr(lastTimeSlash + 1,
                _pointsPlayTime[_curPlayPointIndex + 1].size() - lastTimeSlash));

        float ratio = (_playTime - curPointPlayTime) /
                      (nextPointPlayTime - curPointPlayTime);

        ImpMath::Vector3 finalCameraPosition = cameraPosition * (1 - ratio) +
                                               nextCameraPosition * ratio;
        //ImpMath::Quaternion finalCameraRotation = 
        //    (cameraRotation * (1 - ratio)) *= (nextCameraRotation * ratio);
        ImpMath::Quaternion finalCameraRotation = ImpMath::Quaternion::QuaternionSlerp(
            cameraRotation, nextCameraRotation, ratio);

        GetWorld()
            ->GetEntity("MainCamera")
            ->GetComponent<Transform>()
            ->SetPosition(finalCameraPosition);
        GetWorld()
            ->GetEntity("MainCamera")
            ->GetComponent<Transform>()
            ->SetRotation(finalCameraRotation);
        // ���� + �ؽ�Ʈ �ð� ���� �����ͼ� ��ġ �����ϱ�
        // 
        // ī�޶� �������ֱ�
    }
}

ImpEngineModule::Component* ImpClient::CameraWalk::Clone(Component* clone) const
{
    CameraWalk* emp = static_cast<CameraWalk*>(clone);

    if (emp == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        emp = new CameraWalk(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *emp = *this;
    }

    return emp;
}