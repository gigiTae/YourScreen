#include "pch.h"
#include "ImpFont.h"

#include "ImpDevice.h"
#include "ImpRenderState.h"

#include <tchar.h>

ImpGraphics::ImpFont::ImpFont()
{

}

ImpGraphics::ImpFont::~ImpFont()
{
	//SafeDelete(m_pSpriteFont);
	//SafeDelete(m_pSpriteBatch);

	delete _spriteBatch;
	delete _spriteFont;
}

void ImpGraphics::ImpFont::Initialize(ImpDevice* device, ImpRenderState* renderState)
{
	ID3D11DeviceContext* dc = nullptr;
	device->GetDevice()->GetImmediateContext(&dc);

	_spriteBatch = new DirectX::SpriteBatch(dc);

	TCHAR* _fileName = (TCHAR*)L"Resources/Font/gulim9k.spritefont";

	_spriteFont = new DirectX::SpriteFont(device->GetDevice(), _fileName);
	_spriteFont->SetLineSpacing(14.0f);
	//m_pSpriteFont->SetDefaultCharacter(' ');

	dc->Release();

	_renderState = renderState;
	//_RasterizerState = rs;
	//_DepthStencilState = ds;
}

void ImpGraphics::ImpFont::DrawTextColor(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text, ...)
{
	TCHAR _buffer[1024] = L"";
	va_list vl;
	va_start(vl, text);
	_vstprintf(_buffer, 1024, text, vl);
	va_end(vl);

	// SpriteBatch에 여러 렌더 스테이트를 지정할 수 있다.
	// 문제는, 그냥 Begin만 하면 뎁스스탠실버퍼 옵션이 D3D11_DEPTH_WRITE_MASK_ZERO가 되는 듯. DSS를 다루지 않는 포반에는 문제가 될 수 있다.
	// 아래처럼 여기에 옵션을 ALL로 넣어줘서 ZERO가 되는 것을 막을 수도 있고, 다른 쪽 오브젝트를 그릴 때 렌더스테이트를 지정 해 줄 수도 있다.
	// DX12에서 렌더스테이트등을 그렇게 가져가는 것도 이해는 간다. 별로 맘에 안들었겠지..
	///m_pSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, m_DepthStencilState/*, m_RasterizerState*/);
	_spriteBatch->Begin();
	DirectX::XMVECTOR colorVec = DirectX::XMLoadFloat4(&color);
	_spriteFont->DrawString(_spriteBatch, _buffer, DirectX::XMFLOAT2((float)x, (float)y), colorVec);
	_spriteBatch->End();
}