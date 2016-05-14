#include <directxmath.h>
#include <directxcolors.h>

namespace ITP485
{

	enum EBindflags
	{
		EBF_VertexBuffer = D3D11_BIND_VERTEX_BUFFER,
		EBF_IndexBuffer = D3D11_BIND_INDEX_BUFFER,
		EBF_ConstantBuffer = D3D11_BIND_CONSTANT_BUFFER,
		EBF_ShaderResource = D3D11_BIND_SHADER_RESOURCE,
		EBF_RenderTarget = D3D11_BIND_RENDER_TARGET,
		EBF_DeptStencil = D3D11_BIND_DEPTH_STENCIL
	};

	enum EGraphicsBufferUsage
	{
		EGBU_Default = D3D11_USAGE_DEFAULT,
		EGBU_Immutable = D3D11_USAGE_IMMUTABLE,
		EGBU_Dynamic = D3D11_USAGE_DYNAMIC,
		EGBU_Staging = D3D11_USAGE_STAGING,
	};

	enum ECPUAccessFlags
	{
		ECPUAF_CanWrite = D3D11_CPU_ACCESS_WRITE,
		ECPUAF_CanRead = D3D11_CPU_ACCESS_READ,
	};

	enum EGFormat
	{
		EGF_R32G32B32A32_Float = DXGI_FORMAT_R32G32B32A32_FLOAT,
		EGF_R32G32B32_Float = DXGI_FORMAT_R32G32B32_FLOAT,
		EGF_R32G32_Float = DXGI_FORMAT_R32G32_FLOAT,
		EGF_R8G8B8A8_UNorm = DXGI_FORMAT_R8G8B8A8_UNORM,
		EGF_R32_Float = DXGI_FORMAT_R32_FLOAT,
	};

	enum EFillMode
	{
		EFM_Wireframe = D3D11_FILL_WIREFRAME,
		EFM_Solid = D3D11_FILL_SOLID,
	};

	enum EComparisonFunc
	{
		ECF_Never = D3D11_COMPARISON_NEVER,
		ECF_Less = D3D11_COMPARISON_LESS,
		ECF_Equal = D3D11_COMPARISON_EQUAL,
		ECF_LessEqual = D3D11_COMPARISON_LESS_EQUAL,
		ECF_Greater = D3D11_COMPARISON_GREATER,
		ECF_NotEqual = D3D11_COMPARISON_NOT_EQUAL,
		ECF_GreaterEqual = D3D11_COMPARISON_GREATER_EQUAL,
		ECF_Always = D3D11_COMPARISON_ALWAYS,
	};

	enum EInputClassificationType
	{
		EIC_PerVertex = D3D11_INPUT_PER_VERTEX_DATA,
		EIC_PerInstance = D3D11_INPUT_PER_INSTANCE_DATA
	};

	struct InputLayoutElement : D3D11_INPUT_ELEMENT_DESC
	{
		InputLayoutElement( const char* inSemanticName, uint32_t inSemanticIndex, EGFormat inFormat, uint32_t inInputSlot, uint32_t inByteOffset, EInputClassificationType inInputClassification, uint32_t inInstanceDataStepRate ) 
		{
			SemanticName = inSemanticName;
			SemanticIndex = inSemanticIndex;
			Format = static_cast< DXGI_FORMAT > ( inFormat );
			InputSlot = inInputSlot;
			AlignedByteOffset = inByteOffset;
			InputSlotClass = static_cast< D3D11_INPUT_CLASSIFICATION > (inInputClassification);
			InstanceDataStepRate = inInstanceDataStepRate;
		}
	};

	typedef shared_ptr< ID3D11Buffer >				GraphicsBufferPtr;
	typedef shared_ptr< ID3D11InputLayout >			InputLayoutPtr;
	typedef shared_ptr< ID3D11PixelShader >			PixelShaderPtr;
	typedef shared_ptr< ID3D11VertexShader >		VertexShaderPtr;
	typedef shared_ptr< ID3D11RenderTargetView >	RenderTargetPtr;
	typedef shared_ptr< ID3D11DepthStencilView >	DepthStencilPtr;
	typedef shared_ptr< ID3D11SamplerState >		SamplerStatePtr;
	typedef shared_ptr< ID3D11ShaderResourceView >	TexturePtr;
	typedef shared_ptr< ID3D11DepthStencilState >	DepthStencilStatePtr;
	typedef shared_ptr< ID3D11RasterizerState >		RasterizerStatePtr;
	typedef shared_ptr< ID3D11BlendState >			BlendStatePtr;


	class GraphicsDriver 
	{
	public:

		static void StaticInit( HWND inWindow );
		static void StaticDestroy();
		~GraphicsDriver();

		static GraphicsDriver* Get() { return sInstance; }

		RenderTargetPtr GetBackBufferRenderTarget() const
		{
			return mBackBufferRenderTarget;
		}

		HRESULT CompileShaderFromFile( const wchar_t* inFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, vector< char >& outCompiledShaderCode );
		VertexShaderPtr CreateVertexShader( const std::vector< char >& inCompiledShaderCode );
		PixelShaderPtr CreatePixelShader( const std::vector< char >& inCompiledShaderCode );
		InputLayoutPtr CreateInputLayout( const InputLayoutElement* inElements, int inNumElements, const vector< char >& inCompiledVertexShader );
		GraphicsBufferPtr CreateGraphicsBuffer( const void* inRawData, int inRawDataSize, int inBindFlags, int inCPUAccessFlags, EGraphicsBufferUsage inUsage );
		SamplerStatePtr CreateSamplerState();
		TexturePtr CreateTextureFromFile( const wchar_t* inFileName );
		DepthStencilPtr CreateDepthStencil( int inWidth, int inHeight );
		DepthStencilStatePtr CreateDepthStencilState( bool inDepthTestEnable, EComparisonFunc inDepthComparisonFunction );
		RasterizerStatePtr CreateRasterizerState( EFillMode inFillMode );
		BlendStatePtr CreateBlendState();

		void SetRenderTarget( RenderTargetPtr inRenderTarget );
		void SetDepthStencil( DepthStencilPtr inDepthStencil );
		void SetViewport( float inX, float inY, float inWidth, float inHeight );
		void SetInputLayout( InputLayoutPtr inLayout );
		void SetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY inTopology );
		void SetVertexBuffer( GraphicsBufferPtr inBuffer, uint32_t inVertexSize );
		void SetVertexBuffers( GraphicsBufferPtr firstBuffer, uint32_t inFirstVertexSize, GraphicsBufferPtr secondBuffer, uint32_t inSecondVertexSize );
		void SetIndexBuffer( GraphicsBufferPtr inBuffer );
		void SetVertexShader( VertexShaderPtr inVertexShader );
		void SetVSConstantBuffer( GraphicsBufferPtr inBuffer, int inStartSlot );
		void SetPixelShader( PixelShaderPtr inPixelShader );
		void SetPSConstantBuffer( GraphicsBufferPtr inBuffer, int inStartSlot );
		void SetPSSamplerState( SamplerStatePtr inSamplerState, int inStartSlot );
		void SetPSTexture( TexturePtr inTexture, int inStartSlot );
		void SetDepthStencilState( DepthStencilStatePtr inDepthStencilState );
		void SetRasterizerState( RasterizerStatePtr inRasterizerStatePtr );
		void SetBlendState( BlendStatePtr inBlendState );

		void* MapBuffer( GraphicsBufferPtr inBuffer );
		void UnmapBuffer( GraphicsBufferPtr inBuffer );

		template< typename T >
		T* MapBuffer( GraphicsBufferPtr inBuffer )
		{
			return static_cast< T* >( MapBuffer( inBuffer ) );
		}


		void ClearBackBuffer();
		void ClearRenderTarget( RenderTargetPtr inRenderTarget, const DirectX::XMVECTORF32& inColor );
		void ClearDepthStencil( DepthStencilPtr inDepthStencil, float inDepth );
		void Draw( int inVertexCount, int inStartVertexIndex );
		void DrawIndexed( int inIndexCount, int inStartIndexLocation, int inBaseVertexLocation );
		void DrawIndexedInstanced( int inIndexCountPerInstance, int inInstanceCount, int inStartIndexLocation, int inBaseVertexLocation, int inStartInstanceLocation );
		void Present();

		void SetPerObjectConstantBuffer( GraphicsBufferPtr inConstantBuffer ) { mPerObjectConstantBuffer = inConstantBuffer; }
		void SetPerCameraConstantBuffer( GraphicsBufferPtr inConstantBuffer ) { mPerCameraConstantBuffer = inConstantBuffer; }
		void SetMatrixPaletteConstantBuffer( GraphicsBufferPtr inConstantBuffer ) { mMatrixPaletteConstantBuffer = inConstantBuffer; }

		GraphicsBufferPtr GetPerObjectConstantBuffer() const { return mPerObjectConstantBuffer; }
		GraphicsBufferPtr GetPerCameraConstantBuffer() const { return mPerCameraConstantBuffer; }
		GraphicsBufferPtr GetMatrixPaletteConstantBuffer() const { return mMatrixPaletteConstantBuffer; }

		uint32_t GetWindowWidth() const { return mWindowWidth; }
		uint32_t GetWindowHeight() const { return mWindowHeight; }

	private:

		uint32_t mWindowWidth, mWindowHeight;

		GraphicsDriver( HWND inWindow );

		RenderTargetPtr mBackBufferRenderTarget;

		GraphicsBufferPtr mPerObjectConstantBuffer;
		GraphicsBufferPtr mPerCameraConstantBuffer;
		GraphicsBufferPtr mMatrixPaletteConstantBuffer;

		RenderTargetPtr	mCurrentRenderTarget;
		DepthStencilPtr mCurrentDepthStencil;

		static GraphicsDriver* sInstance;

	};

}

