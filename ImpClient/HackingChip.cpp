#include "ImpClientPCH.h"
#include "HackingChip.h"

ImpClient::HackingChip::HackingChip()
    : _isChanged(false)
    , _isLeft(true)
{
}

ImpClient::HackingChip::~HackingChip()
{
}

ImpEngineModule::Component* ImpClient::HackingChip::Clone(
    Component* clone /* = nullptr */) const
{
    HackingChip* chip = static_cast<HackingChip*>(clone);

    if (chip == nullptr) // 새로 생성해서 복사본을 준다
    {
        chip = new HackingChip(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *chip = *this;
    }

    return chip;
}

void ImpClient::HackingChip::ChangeChip()
{
    auto ui           = GetComponent<UI>();
    std::wstring path = ui->GetTexturePath();

    if (_isLeft)
    {
        if (_chipType == ChipType::QuickEMP)
        {
            path = PathManager::ChangeFileName(path, L"QuickHackingChip");
        }
        else if (_chipType == ChipType::ScaleEMP)
        {
            path = PathManager::ChangeFileName(path, L"ScaleHackingChip");
        }
        else if (_chipType == ChipType::Victory)
        {
            path = PathManager::ChangeFileName(path, L"VictoryHackingChip");
        }
    }
    else
    {
        if (_chipType == ChipType::QuickEMP)
        {
            path = PathManager::ChangeFileName(path, L"QuickHackingChipR");
        }
        else if (_chipType == ChipType::ScaleEMP)
        {
            path = PathManager::ChangeFileName(path, L"ScaleHackingChipR");
        }
        else if (_chipType == ChipType::Victory)
        {
            path = PathManager::ChangeFileName(path, L"VictoryHackingChipR");
        }
    }

    ui->SetTexturePath(path);


    _isChanged = true;
}
