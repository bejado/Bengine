#include <PrecompiledHeader.h>
#include "DDSTextureLoader.h"


ID3D11Device*           g_pd3dDevice = nullptr;
ID3D11Device1*          g_pd3dDevice1 = nullptr;
ID3D11DeviceContext*    g_pImmediateContext = nullptr;
ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
IDXGISwapChain*         g_pSwapChain = nullptr;
IDXGISwapChain1*        g_pSwapChain1 = nullptr;
ID3D11Debug*			g_d3dDebug = nullptr;
ID3D11RasterizerState*	g_RasterizerState = nullptr;

using namespace ITP485;
using namespace DirectX;

//needs to be manually initialized, so not a typical instance...
GraphicsDriver* GraphicsDriver::sInstance;;


namespace
{

	void AutoReleaseD3D( ID3D11DeviceChild* inResource )
	{
		inResource->Release();
	}


	void CreateInternalDevice()
	{
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE( featureLevels );

		D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

		HRESULT hr = S_OK;
		for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			driverType = driverTypes[ driverTypeIndex ];
			hr = D3D11CreateDevice( nullptr, driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
									D3D11_SDK_VERSION, &g_pd3dDevice, &featureLevel, &g_pImmediateContext );

			if( hr == E_INVALIDARG )
			{
				// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
				hr = D3D11CreateDevice( nullptr, driverType, nullptr, createDeviceFlags, &featureLevels[ 1 ], numFeatureLevels - 1,
										D3D11_SDK_VERSION, &g_pd3dDevice, &featureLevel, &g_pImmediateContext );
			}

			if( SUCCEEDED( hr ) )
			{
				/*
				#ifdef _DEBUG
								g_pd3dDevice->QueryInterface( __uuidof( ID3D11Debug ), reinterpret_cast< void** >( &g_d3dDebug ) );
				#endif
				*/
				//yay!
				return;
			}
		}

		//uhoh
		Dbg_Assert( hr == S_OK, "Problem Creating Graphics Device" );
	}

	HRESULT CreateSwapChain( UINT inBackBufferWidth, UINT inBackBufferHeight, HWND inWindow )
	{
		HRESULT hr;

		// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
		IDXGIFactory1* dxgiFactory = nullptr;
		{
			IDXGIDevice* dxgiDevice = nullptr;
			hr = g_pd3dDevice->QueryInterface( __uuidof( IDXGIDevice ), reinterpret_cast<void**>( &dxgiDevice ) );
			if( SUCCEEDED( hr ) )
			{
				IDXGIAdapter* adapter = nullptr;
				hr = dxgiDevice->GetAdapter( &adapter );
				if( SUCCEEDED( hr ) )
				{
					hr = adapter->GetParent( __uuidof( IDXGIFactory1 ), reinterpret_cast<void**>( &dxgiFactory ) );
					adapter->Release();
				}
				dxgiDevice->Release();
			}
		}
		if( FAILED( hr ) )
		{
			return hr;
		}

		// Create swap chain
		IDXGIFactory2* dxgiFactory2 = nullptr;
		hr = dxgiFactory->QueryInterface( __uuidof( IDXGIFactory2 ), reinterpret_cast<void**>( &dxgiFactory2 ) );
		if( dxgiFactory2 )
		{
			// DirectX 11.1 or later
			hr = g_pd3dDevice->QueryInterface( __uuidof( ID3D11Device1 ), reinterpret_cast<void**>( &g_pd3dDevice1 ) );
			if( SUCCEEDED( hr ) )
			{
				( void ) g_pImmediateContext->QueryInterface( __uuidof( ID3D11DeviceContext1 ), reinterpret_cast<void**>( &g_pImmediateContext1 ) );
			}

			DXGI_SWAP_CHAIN_DESC1 sd;
			ZeroMemory( &sd, sizeof( sd ) );
			sd.Width = inBackBufferWidth;
			sd.Height = inBackBufferHeight;
			sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = 1;

			hr = dxgiFactory2->CreateSwapChainForHwnd( g_pd3dDevice, inWindow, &sd, nullptr, nullptr, &g_pSwapChain1 );
			if( SUCCEEDED( hr ) )
			{
				hr = g_pSwapChain1->QueryInterface( __uuidof( IDXGISwapChain ), reinterpret_cast<void**>( &g_pSwapChain ) );
			}

			dxgiFactory2->Release();
		}
		else
		{
			// DirectX 11.0 systems
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory( &sd, sizeof( sd ) );
			sd.BufferCount = 1;
			sd.BufferDesc.Width = inBackBufferWidth;
			sd.BufferDesc.Height = inBackBufferHeight;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = inWindow;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = TRUE;

			hr = dxgiFactory->CreateSwapChain( g_pd3dDevice, &sd, &g_pSwapChain );
		}

		dxgiFactory->Release();

		if( FAILED( hr ) )
		{
			return hr;
		}

		return S_OK;
	}

	RenderTargetPtr CreateBackBufferRenderTargetView()
	{
		HRESULT hr;
		// Create a render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
		if( FAILED( hr ) )
		{
			return nullptr;
		}

		ID3D11RenderTargetView* toRet;
		hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &toRet );
		pBackBuffer->Release();
		if( FAILED( hr ) )
		{
			return nullptr;
		}

		return RenderTargetPtr( toRet, AutoReleaseD3D );
	}



}

void GraphicsDriver::StaticInit( HWND inWindow  )
{
	sInstance = new GraphicsDriver( inWindow  );
}

void GraphicsDriver::StaticDestroy()
{
	delete sInstance;
	sInstance = nullptr;
}

GraphicsDriver::GraphicsDriver( HWND inWindow  ) :
mCurrentRenderTarget( nullptr ),
mCurrentDepthStencil( nullptr )
{
	RECT rc;
	GetClientRect( inWindow, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	
	CreateInternalDevice();
	CreateSwapChain( width, height, inWindow );
	mBackBufferRenderTarget = CreateBackBufferRenderTargetView();

	mWindowWidth = width;
	mWindowHeight = height;
	SetViewport( 0.f, 0.f, static_cast< float >( width ), static_cast< float >( height ) );

	SetRasterizerState( CreateRasterizerState( EFM_Wireframe ) );

	SetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	SetRenderTarget( GetBackBufferRenderTarget() );
}

void GraphicsDriver::SetRenderTarget( RenderTargetPtr inRenderTarget )
{
	mCurrentRenderTarget = inRenderTarget;

	auto renderTarget = mCurrentRenderTarget.get();
	g_pImmediateContext->OMSetRenderTargets( 1, &renderTarget, mCurrentDepthStencil.get() );
}

void GraphicsDriver::SetDepthStencil( DepthStencilPtr inDepthStencil )
{
	mCurrentDepthStencil = inDepthStencil;

	auto renderTarget = mCurrentRenderTarget.get();
	g_pImmediateContext->OMSetRenderTargets( 1, &renderTarget, mCurrentDepthStencil.get() );
}


void GraphicsDriver::SetViewport( float inX, float inY, float inWidth, float inHeight )
{
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.TopLeftX = inX;
	vp.TopLeftY = inY;
	vp.Width = inWidth;
	vp.Height = inHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pImmediateContext->RSSetViewports( 1, &vp );
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT GraphicsDriver::CompileShaderFromFile( const wchar_t* inFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, vector< char >& outCompiledShaderCode )
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	ID3DBlob* pBlobOut = nullptr;
	hr = D3DCompileFromFile( inFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
							 dwShaderFlags, 0, &pBlobOut, &pErrorBlob );
	if( FAILED( hr ) )
	{
		if( pErrorBlob )
		{
			wstring message = StringUtils::Sprintf( L"%hs", pErrorBlob->GetBufferPointer() );
			OutputDebugStringW( message.c_str() );
			MessageBox( nullptr, message.c_str(), L"Error", MB_OK );
			pErrorBlob->Release();
			Dbg_Assert( hr == S_OK, "Shader Compilation Failed" );
		}
		return hr;
	}
	if( pErrorBlob )
	{
		pErrorBlob->Release();
	}

	//now copy to vector if we like it...
	if( pBlobOut )
	{
		int compiledCodeSize = pBlobOut->GetBufferSize();
		outCompiledShaderCode.resize( compiledCodeSize );
		std::memcpy( outCompiledShaderCode.data(), pBlobOut->GetBufferPointer(), compiledCodeSize );

		pBlobOut->Release();
	}

	return S_OK;
}


VertexShaderPtr GraphicsDriver::CreateVertexShader( const std::vector< char >& inCompiledShaderCode )
{
	ID3D11VertexShader* toRet = nullptr;
	HRESULT hr = g_pd3dDevice->CreateVertexShader( inCompiledShaderCode.data(), inCompiledShaderCode.size(), nullptr, &toRet );
	Dbg_Assert( hr == S_OK, "Failure Creating Vertex Shader From Compiled Shader Code" );

	return VertexShaderPtr( toRet, AutoReleaseD3D );
}


PixelShaderPtr GraphicsDriver::CreatePixelShader( const std::vector< char >& inCompiledShaderCode )
{
	ID3D11PixelShader* toRet = nullptr;
	HRESULT hr = g_pd3dDevice->CreatePixelShader( inCompiledShaderCode.data(), inCompiledShaderCode.size(), nullptr, &toRet );
	Dbg_Assert( hr == S_OK, "Failure Creating Pixel Shader From Compiled Shader Code" );
	return PixelShaderPtr( toRet, AutoReleaseD3D );
}

InputLayoutPtr GraphicsDriver::CreateInputLayout( const InputLayoutElement* inElements, int inNumElements, const vector< char >& inCompiledVertexShader )
{
	ID3D11InputLayout* toRet = nullptr;
	HRESULT hr = g_pd3dDevice->CreateInputLayout( 
			inElements, inNumElements, 
			inCompiledVertexShader.data(), inCompiledVertexShader.size(), 
			 &toRet );
	Dbg_Assert( hr == S_OK, "Failure Creating Vertex Layout" );

	return InputLayoutPtr( toRet, AutoReleaseD3D );
}

SamplerStatePtr GraphicsDriver::CreateSamplerState()
{
	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof( sampDesc ) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ID3D11SamplerState* toRet = nullptr;
	HRESULT hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &toRet );
	Dbg_Assert( hr == S_OK, "Failure Creating Sampler State" );

	return SamplerStatePtr( toRet, AutoReleaseD3D );
}


void GraphicsDriver::SetInputLayout( InputLayoutPtr inLayout )
{
	g_pImmediateContext->IASetInputLayout( inLayout.get() );
}

void GraphicsDriver::SetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY inTopology )
{
	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology( inTopology );
}

void GraphicsDriver::SetVertexBuffer( GraphicsBufferPtr inBuffer, uint32_t inVertexSize )
{
	auto buffer = inBuffer.get();
	uint32_t offset = 0;
	g_pImmediateContext->IASetVertexBuffers( 0, 1, &buffer, &inVertexSize, &offset );
}

void GraphicsDriver::SetVertexBuffers( GraphicsBufferPtr firstBuffer, uint32_t inFirstVertexSize, GraphicsBufferPtr secondBuffer, uint32_t inSecondVertexSize )
{
	ID3D11Buffer *buffers[2] = { firstBuffer.get(), secondBuffer.get() };
	uint32_t strides[2] = { inFirstVertexSize, inSecondVertexSize };
	uint32_t offsets[2] = { 0, 0 };
	g_pImmediateContext->IASetVertexBuffers( 0, 2, buffers, strides, offsets );
}

void GraphicsDriver::SetIndexBuffer( GraphicsBufferPtr inBuffer )
{
	g_pImmediateContext->IASetIndexBuffer( inBuffer.get(), DXGI_FORMAT_R16_UINT, 0 );
}

void GraphicsDriver::SetVSConstantBuffer( GraphicsBufferPtr inBuffer, int inStartSlot )
{
	auto buffer = inBuffer.get();
	g_pImmediateContext->VSSetConstantBuffers( inStartSlot, 1, &buffer );
}

void GraphicsDriver::SetPSConstantBuffer( GraphicsBufferPtr inBuffer, int inStartSlot )
{
	auto buffer = inBuffer.get();
	g_pImmediateContext->PSSetConstantBuffers( inStartSlot, 1, &buffer );
}

void GraphicsDriver::SetPSSamplerState( SamplerStatePtr inSamplerState, int inStartSlot )
{
	auto sampler = inSamplerState.get(); 
	g_pImmediateContext->PSSetSamplers( inStartSlot, 1, &sampler );
}

void GraphicsDriver::SetPSTexture( TexturePtr inTexture, int inStartSlot )
{
	auto texture = inTexture.get();
	g_pImmediateContext->PSSetShaderResources( inStartSlot, 1, &texture );
}

void GraphicsDriver::SetDepthStencilState( DepthStencilStatePtr inDepthStencilState )
{
	g_pImmediateContext->OMSetDepthStencilState( inDepthStencilState.get(), 1 );
}


void* GraphicsDriver::MapBuffer( GraphicsBufferPtr inBuffer )
{
	D3D11_MAPPED_SUBRESOURCE mapped;
	g_pImmediateContext->Map( inBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped );
	return mapped.pData;
}

void GraphicsDriver::UnmapBuffer( GraphicsBufferPtr inBuffer )
{
	g_pImmediateContext->Unmap( inBuffer.get(), 0 );
}

shared_ptr< ID3D11Buffer > GraphicsDriver::CreateGraphicsBuffer( const void* inRawData, int inRawDataSize, int inBindFlags, int inCPUAccessFlags, EGraphicsBufferUsage inUsage )
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );

	bd.Usage = static_cast< D3D11_USAGE >( inUsage );
	bd.ByteWidth = inRawDataSize;

	bd.BindFlags = inBindFlags;
	
	bd.CPUAccessFlags = inCPUAccessFlags;
	D3D11_SUBRESOURCE_DATA initialData;
	ZeroMemory( &initialData, sizeof( initialData ) );
	initialData.pSysMem = inRawData;

	ID3D11Buffer* toRet = nullptr;
	HRESULT hr = g_pd3dDevice->CreateBuffer( &bd, inRawData ? &initialData : nullptr, &toRet );
	Dbg_Assert( hr == S_OK, "Problem Creating Vertex Buffer" );

	return shared_ptr< ID3D11Buffer >( toRet, AutoReleaseD3D );
}

TexturePtr GraphicsDriver::CreateTextureFromFile( const wchar_t* inFileName )
{
	//erg, convert to widechar...
	ID3D11ShaderResourceView* toRet = nullptr;
	HRESULT hr = CreateDDSTextureFromFile( g_pd3dDevice, inFileName, nullptr, &toRet );
	Dbg_Assert( hr == S_OK, "Problem Creating Texture From File" );
	
	return TexturePtr( toRet, AutoReleaseD3D );

}

DepthStencilPtr GraphicsDriver::CreateDepthStencil( int inWidth, int inHeight )
{
	ID3D11Texture2D* depthStencilTexture;
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof( descDepth ) );
	descDepth.Width = inWidth;
	descDepth.Height = inHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = g_pd3dDevice->CreateTexture2D( &descDepth, nullptr, &depthStencilTexture );
	Dbg_Assert( hr == S_OK, "Problem Creating Depth Stencil" );

	ID3D11DepthStencilView* toRet = nullptr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof( descDSV ) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView( depthStencilTexture, &descDSV, &toRet );
	Dbg_Assert( hr == S_OK, "Problem Creating Depth Stencil" );

	return DepthStencilPtr( toRet, AutoReleaseD3D );
}

DepthStencilStatePtr GraphicsDriver::CreateDepthStencilState( bool inDepthTestEnable, EComparisonFunc inDepthComparisonFunction )
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = inDepthTestEnable;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = static_cast< D3D11_COMPARISON_FUNC >( inDepthComparisonFunction );

	// Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState* toRet = nullptr;
	HRESULT hr = g_pd3dDevice->CreateDepthStencilState( &dsDesc, &toRet );
	Dbg_Assert( hr == S_OK, "Problem Creating Depth Stencil" );

	return DepthStencilStatePtr( toRet, AutoReleaseD3D );
}

GraphicsDriver::~GraphicsDriver()
{
	mCurrentRenderTarget.reset();
	mCurrentDepthStencil.reset();

	if( g_pImmediateContext )
	{
		g_pImmediateContext->ClearState();
		g_pImmediateContext->Flush();
	}

	mBackBufferRenderTarget.reset();
	
	if( g_pSwapChain1 ) g_pSwapChain1->Release();
	if( g_pSwapChain ) g_pSwapChain->Release();
	if( g_pImmediateContext1 ) g_pImmediateContext1->Release();
	if( g_pImmediateContext ) g_pImmediateContext->Release();
	if( g_pd3dDevice1 ) g_pd3dDevice1->Release();
	if( g_pd3dDevice ) g_pd3dDevice->Release();
	if( g_RasterizerState ) g_RasterizerState->Release();
	

	if( g_d3dDebug )
	{
		//g_d3dDebug->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
		g_d3dDebug->Release();
	}

}

void GraphicsDriver::ClearBackBuffer()
{
	ClearRenderTarget( mBackBufferRenderTarget, DirectX::Colors::Black );
}

void GraphicsDriver::ClearRenderTarget( RenderTargetPtr inRenderTarget, const XMVECTORF32& inColor )
{
	// Clear the back buffer 
	g_pImmediateContext->ClearRenderTargetView( inRenderTarget.get(), inColor );
}

void GraphicsDriver::ClearDepthStencil( DepthStencilPtr inDepthStencil, float inDepth )
{
	// Clear the back buffer 
	g_pImmediateContext->ClearDepthStencilView( inDepthStencil.get(), D3D11_CLEAR_DEPTH, inDepth, 0 );
}

void GraphicsDriver::SetVertexShader( VertexShaderPtr inVertexShader )
{
	g_pImmediateContext->VSSetShader( inVertexShader.get(), nullptr, 0 );
}

void GraphicsDriver::SetPixelShader( PixelShaderPtr inPixelShader )
{
	g_pImmediateContext->PSSetShader( inPixelShader.get(), nullptr, 0 );
}

void GraphicsDriver::Draw( int inVertexCount, int inStartVertexIndex )
{
	g_pImmediateContext->Draw( inVertexCount, inStartVertexIndex );
}

void GraphicsDriver::DrawIndexed( int inIndexCount, int inStartIndexLocation, int inBaseVertexLocation )
{
	g_pImmediateContext->DrawIndexed( inIndexCount, inStartIndexLocation, inBaseVertexLocation );
}

void GraphicsDriver::DrawIndexedInstanced( int inIndexCountPerInstance, int inInstanceCount, int inStartIndexLocation, int inBaseVertexLocation, int inStartInstanceLocation )
{
	g_pImmediateContext->DrawIndexedInstanced( inIndexCountPerInstance, inInstanceCount, inStartIndexLocation, inBaseVertexLocation, inStartIndexLocation );
}

void GraphicsDriver::Present()
{
	g_pSwapChain->Present( 0, 0 );
}


RasterizerStatePtr GraphicsDriver::CreateRasterizerState( EFillMode inFillMode )
{
	//let's set CCW as the front face, since right hand rule is nice...
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory( &rastDesc, sizeof( rastDesc ) );

	//lab3
	rastDesc.FillMode = static_cast< D3D11_FILL_MODE >( inFillMode );

	rastDesc.CullMode = D3D11_CULL_BACK;
	//rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FrontCounterClockwise = false;

	ID3D11RasterizerState* toRet;
	auto hr = g_pd3dDevice->CreateRasterizerState( &rastDesc, &toRet );
	Dbg_Assert( hr == S_OK, "Problem Creating Rasterizer State" );

	return RasterizerStatePtr( toRet, AutoReleaseD3D );
}

BlendStatePtr GraphicsDriver::CreateBlendState()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	ID3D11BlendState* toRet;
	auto hr = g_pd3dDevice->CreateBlendState( &blendDesc, &toRet );
	Dbg_Assert( hr == S_OK, "Problem Creating Blend State" );

	return BlendStatePtr( toRet, AutoReleaseD3D );
}

void GraphicsDriver::SetRasterizerState( RasterizerStatePtr inRasterizerState )
{
	g_pImmediateContext->RSSetState( inRasterizerState.get() );
}

void GraphicsDriver::SetBlendState( BlendStatePtr inBlendState )
{
	float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	UINT sampleMask = 0xffffffff;
	g_pImmediateContext->OMSetBlendState( inBlendState.get(), NULL, sampleMask );
}
