#pragma once

namespace ImpGraphics
{
	class ImpDevice;

	// 포워드 렌더링 용 렌더 타겟이랑 디퍼드 렌더링용 렌더 타겟을 구별하는게 좋을
	// 것 같은데 

	//2024.02.08 현재 렌더 순서
	// First - 불투명 디퍼드로 그리기 out 5, Second - 디퍼드 연산 및 디퍼드, 밝은 부분 추출 out 2, DownSampling - threePass
	// second - downsampling 사이에 포워드를 그릴 예정
	// unbind는 개수 받아와서 unbind하게 바꿔야 겠다
	class ImpRenderTarget
	{
	public:
		ImpRenderTarget(ImpDevice* device);
		~ImpRenderTarget();

		void Initialize(int width, int height);
		void OnResize(int width, int height);

		void FirstPassClear();
        void FirstPassBind();
        void FirstPassUnbind();

		void SecondPassClear(); // 2pass
		void SecondPassBind(); // 2pass
		void SecondPassUnbind(); // 2pass

		void ForwardPassClear(); // Forward 첫번 째 패스
		void ForwardPassBind();
		void ForwardPassUnbind();

		void ForwardOITPassClear(); // Forward 첫번 째 패스
        void ForwardOITPassBind();
        void ForwardOITPassUnbind();

		void DownSamplingPassClear(size_t index);
        void DownSamplingPassBind(size_t index);
        void DownSamplingPassUnbind(size_t index);

		void ThreePassClear();
		void ThreePassBind();
		void ThreePassUnbind();

		std::vector<ID3D11ShaderResourceView*> GetDeferredShaderResourceViews()
		{ return _deferredShaderResourceView; }
        std::vector<ID3D11ShaderResourceView*> GetBloomShaderResourceViews() {return _bloomShaderResourceView;}
        std::vector<ID3D11DepthStencilView*> GetBloomDepthStencilViews() {return _depthStencilViews;}

		// X값을 가져다 써야 Depth임
        ID3D11ShaderResourceView* GetDeferredDepthShaderResourceView() {return _deferredShaderResourceView[4];}

		float _bloomRatio;
	private:
		ImpDevice* _device;

		ID3D11RenderTargetView* _renderTargetView;		// 랜더 타겟 뷰

		// 디퍼드 용s
		size_t _deferredViewSize;
		std::vector<ID3D11RenderTargetView*> _deferredRenderTargetView;
		std::vector<ID3D11ShaderResourceView*> _deferredShaderResourceView;

		// 블러 용
        size_t _bloomViewSize;
        std::vector<ID3D11RenderTargetView*> _bloomRenderTargetView; // 0, 1 은 원본, 밝은 거 2, 3, 4 는 줄인 걸로 만들어야 겠다
        std::vector<ID3D11ShaderResourceView*> _bloomShaderResourceView;
        std::vector <ID3D11DepthStencilView*> _depthStencilViews; // width / 2 /2 /2

		ID3D11DepthStencilView* _depthStencilView;		// 뎁스 스탠실 뷰
	};
}