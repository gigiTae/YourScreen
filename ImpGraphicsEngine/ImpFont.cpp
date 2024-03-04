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

	// SpriteBatch�� ���� ���� ������Ʈ�� ������ �� �ִ�.
	// ������, �׳� Begin�� �ϸ� �������Ľǹ��� �ɼ��� D3D11_DEPTH_WRITE_MASK_ZERO�� �Ǵ� ��. DSS�� �ٷ��� �ʴ� ���ݿ��� ������ �� �� �ִ�.
	// �Ʒ�ó�� ���⿡ �ɼ��� ALL�� �־��༭ ZERO�� �Ǵ� ���� ���� ���� �ְ�, �ٸ� �� ������Ʈ�� �׸� �� ����������Ʈ�� ���� �� �� ���� �ִ�.
	// DX12���� ����������Ʈ���� �׷��� �������� �͵� ���ش� ����. ���� ���� �ȵ������..
	///m_pSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, m_DepthStencilState/*, m_RasterizerState*/);
	_spriteBatch->Begin();
	DirectX::XMVECTOR colorVec = DirectX::XMLoadFloat4(&color);
	_spriteFont->DrawString(_spriteBatch, _buffer, DirectX::XMFLOAT2((float)x, (float)y), colorVec);
	_spriteBatch->End();
}