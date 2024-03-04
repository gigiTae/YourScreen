#pragma once
#include <DirectXMath.h>

namespace ImpMath { struct Matrix; }

namespace ImpGraphics
{
	struct LightInfo;

	struct DirectionalLightData
	{
		DirectX::XMFLOAT4 _color; // Ambient
		DirectX::XMFLOAT3 _direction; // ���� ���� ����
		float _intensity;
	};

	struct PointLightData
	{
		DirectX::XMFLOAT4 _color; // Ambient
		DirectX::XMFLOAT3 _position; // ���� ���� ����
		float _intensity;
		DirectX::XMFLOAT3 _attenuation; // ���� ��� 3��
		float _range; // ���� ����
	};

	struct SpotLightData
    {
        DirectX::XMFLOAT4 _color;    // Ambient
        DirectX::XMFLOAT3 _position; // ���� ���� ����
        float _intensity;
        DirectX::XMFLOAT3 _attenuation; // ���� ��� 3��
        float _range;                   // ���� ����
        DirectX::XMFLOAT3 _direction;   // ���� ���� ����
        float _spot;
    };

	class LightBase
	{
	public:
		LightBase() : _lightID(-1), _lightType(-1) {};
		virtual ~LightBase() {};

		virtual void SetLight(const LightInfo& lightInfo) abstract;
		virtual void Update(ImpMath::Matrix transformMatrix) abstract;

		size_t GetLightID() { return _lightID; };
		size_t GetLightType() { return _lightType; } // 0 Directional, 1 Spot, 2 Point

	protected:
		size_t _lightID;
		size_t _lightType; // 0 Directional, 1 Spot, 2 Point
	};
}