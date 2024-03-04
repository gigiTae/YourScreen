#pragma once

namespace ImpClient
{

class HackingChip: public ImpEngineModule::Component
{
public:
    enum class ChipType
    {
        QuickEMP, // Enp ��Ÿ�� ����
        ScaleEMP, // Emp ���� ����
        Victory,  // �¸� Ĩ
    };

    HackingChip();
    ~HackingChip();

    Component* Clone(Component* clone /* = nullptr */) const override;

    void ChangeChip();

    bool IsChanged() const
    {
        return _isChanged;
    }

    ImpClient::HackingChip::ChipType GetChipType() const
    {
        return _chipType;
    }
    void SetChipType(ImpClient::HackingChip::ChipType val)
    {
        _chipType = val;
    }

    void SetIsLeft(bool isLeft)
    {
        _isLeft = isLeft;
    }

private:
    bool _isLeft;
    bool _isChanged;

    ChipType _chipType;
};

} // namespace ImpClient