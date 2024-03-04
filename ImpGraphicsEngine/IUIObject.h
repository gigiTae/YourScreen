#pragma once

namespace ImpGraphics
{
	class ImpDevice;
	class ImpRenderTarget;
	class ImpRenderState;

	class GeometryBuffer;
    class Texture;

	class IUIObject
	{
    public:
        IUIObject() {};
        virtual ~IUIObject() {};

        virtual void Update(float deltaTime) abstract;
		virtual void Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget) abstract;
		virtual size_t GetLayer() abstract;

		virtual void SetXY(float x, float y, float ratio) abstract;
        virtual void SetGeometryBuffer(std::shared_ptr<GeometryBuffer> vbib) abstract;
        virtual void SetLayer(size_t layer) abstract;
        virtual void SetTexture(std::shared_ptr<Texture> texture) abstract;
        virtual void SetAlpha(float alpha) abstract;
        virtual void SetOffset(float x, float y) abstract;
        virtual void GetOffset(float& x, float& y) abstract;

        virtual void SetWidth(float width) abstract;
        virtual void SetHeight(float height) abstract;
        virtual void SetScreenWidth(float width) abstract;
        virtual void SetScreenHeight(float height) abstract;

        virtual void SetXSize(float ratio) abstract;
        virtual void SetNoise(bool isOn) abstract;
    };
}