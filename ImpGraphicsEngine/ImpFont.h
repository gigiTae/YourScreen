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
/// DXTK�� �̿��� �ؽ�Ʈ ���
/// 
/// �ϼ����ۿ� �ȵȴٴµ�..
/// DirectWrite������ �����ؾ߰ڴ�.
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


	// �׳� ����ϸ� ���� ������ �����.
	ID3D11RasterizerState* m_RasterizerState;
	ID3D11DepthStencilState* m_DepthStencilState;
};*/