#pragma once

struct ID3D11Device;

namespace ImpGraphics
{
	class ImpRenderState
	{
	public:
		ImpRenderState();
		~ImpRenderState();

		void Initialize(ID3D11Device* device);

		void SetWireFrame(bool isWireFrame) { _isWireFrame = isWireFrame; };

		ID3D11RasterizerState* GetCurrentRS() { return _isWireFrame ? _wireframeRS : _solidRS; };
		ID3D11RasterizerState* GetSolidRS() { return _solidRS; };
		ID3D11RasterizerState* GetSolidCullBackRS() { return _solidCullBackRS; };
		
		ID3D11DepthStencilState* GetNormalDSS() { return _normalDSS; }

		//temp
	public:
		ID3D11DepthStencilState* _normalDSS;		// 폰트때문에 뎁스스탠실 스테이트가 강제가 됐다.
		ID3D11DepthStencilState* _cubeMapDSS;
        ID3D11DepthStencilState* _noDepthTestDSS;

	private:
		bool _isWireFrame;

		// Rasterizer State
		ID3D11RasterizerState* _wireframeRS;
		ID3D11RasterizerState* _solidRS;
		ID3D11RasterizerState* _solidCullBackRS;
	};
}