#pragma once

namespace ImpGraphics
{
	class ImpDevice;

	// ������ ������ �� ���� Ÿ���̶� ���۵� �������� ���� Ÿ���� �����ϴ°� ����
	// �� ������ 

	//2024.02.08 ���� ���� ����
	// First - ������ ���۵�� �׸��� out 5, Second - ���۵� ���� �� ���۵�, ���� �κ� ���� out 2, DownSampling - threePass
	// second - downsampling ���̿� �����带 �׸� ����
	// unbind�� ���� �޾ƿͼ� unbind�ϰ� �ٲ�� �ڴ�
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

		void ForwardPassClear(); // Forward ù�� ° �н�
		void ForwardPassBind();
		void ForwardPassUnbind();

		void ForwardOITPassClear(); // Forward ù�� ° �н�
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

		// X���� ������ ��� Depth��
        ID3D11ShaderResourceView* GetDeferredDepthShaderResourceView() {return _deferredShaderResourceView[4];}

		float _bloomRatio;
	private:
		ImpDevice* _device;

		ID3D11RenderTargetView* _renderTargetView;		// ���� Ÿ�� ��

		// ���۵� ��s
		size_t _deferredViewSize;
		std::vector<ID3D11RenderTargetView*> _deferredRenderTargetView;
		std::vector<ID3D11ShaderResourceView*> _deferredShaderResourceView;

		// �� ��
        size_t _bloomViewSize;
        std::vector<ID3D11RenderTargetView*> _bloomRenderTargetView; // 0, 1 �� ����, ���� �� 2, 3, 4 �� ���� �ɷ� ������ �ڴ�
        std::vector<ID3D11ShaderResourceView*> _bloomShaderResourceView;
        std::vector <ID3D11DepthStencilView*> _depthStencilViews; // width / 2 /2 /2

		ID3D11DepthStencilView* _depthStencilView;		// ���� ���Ľ� ��
	};
}