#pragma once

#include "../ImpLib/Inc/SpriteBatch.h"
#include "../ImpLib/Inc/SpriteFont.h"

namespace ImpGraphics
{
	class ImpDevice;
	class ImpRenderState;

	class ImpFont
	{
	public:
		ImpFont();
		~ImpFont();

		void Initialize(ImpDevice* device, ImpRenderState* renderState);

		void DrawTextColor(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text, ...);

	private:
		DirectX::SpriteBatch* _spriteBatch;
		DirectX::SpriteFont* _spriteFont;

		ImpRenderState* _renderState;
	};
}

/*
#include "SpriteFont.h"
#include "SpriteBatch.h"

/// <summary>
/// DXTK를 이용한 텍스트 출력
/// 
/// 완성형밖에 안된다는데..
/// DirectWrite버전도 제공해야겠다.
/// 
/// 2021.03.04 LeHideOffice
/// </summary>
class DXTKFont
{
public:
	DXTKFont();
	~DXTKFont();

	void Create(ID3D11Device* pDevice, ID3D11RasterizerState* rs, ID3D11DepthStencilState* ds);

	void DrawTest();
	void DrawTextColor(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text, ...);

private:
	DirectX::SpriteBatch* m_pSpriteBatch;
	DirectX::SpriteFont* m_pSpriteFont;


	// 그냥 사용하면 뎁스 문제가 생긴다.
	ID3D11RasterizerState* m_RasterizerState;
	ID3D11DepthStencilState* m_DepthStencilState;
};*/