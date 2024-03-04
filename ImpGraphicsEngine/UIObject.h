#pragma once

#include "IUIObject.h"
#include <memory>

namespace ImpGraphics
{
	class GeometryBuffer;
	class Shader;
	class Texture;

	class UIObject : public IUIObject
	{
    public:
        UIObject(std::shared_ptr<GeometryBuffer> vbib, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, size_t layer);
        virtual ~UIObject();

        virtual void Update(float deltaTime) override;
		virtual void Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget) override;
        virtual size_t GetLayer() override { return _layer; };

        virtual void SetXY(float x, float y, float ratio) override;
        virtual void SetGeometryBuffer(std::shared_ptr<GeometryBuffer> vbib) override;
        virtual void SetLayer(size_t layer) override;
        virtual void SetTexture(std::shared_ptr<Texture> texture) override;
        virtual void SetAlpha(float alpha) override;
        virtual void SetOffset(float x, float y) override;
        virtual void GetOffset(float& x, float& y) override;

        virtual void SetWidth(float width) override;
        virtual void SetHeight(float height) override;
        virtual void SetScreenWidth(float width) override;
        virtual void SetScreenHeight(float height) override;

        virtual void SetXSize(float ratio) override;

        virtual void SetNoise(bool isOn) override;

    private:
        std::shared_ptr<GeometryBuffer> _geometryBuffer;
        std::shared_ptr<Shader> _shader;
        std::shared_ptr<Texture> _texture;

        std::shared_ptr<Texture> _noiseTexture;

        std::size_t _layer;

        float _x;
        float _y;
        float _ratio;

        float _offsetX;
        float _offsetY;

        float _alpha;

        float _noiseTime;

        float _width;
        float _height;

        float _screenWidth;
        float _screenHeight;

        float _xRatio;

        bool _isNoise;
	};
}