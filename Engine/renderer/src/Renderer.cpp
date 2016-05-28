#include <PrecompiledHeader.h>

namespace ITP485
{
	void Renderer::Initialize()
	{
		// Create a solid fill rasterizer state
		mRasterizerState = GraphicsDriver::Get()->CreateRasterizerState(EFM_Solid);
		GraphicsDriver::Get()->SetRasterizerState(mRasterizerState);

		// Create a blend state
		mBlendState = GraphicsDriver::Get()->CreateBlendState();
		GraphicsDriver::Get()->SetBlendState( mBlendState );

		// Set up our depth buffer and depth test
		mDepthStencilView = GraphicsDriver::Get()->CreateDepthStencil( GraphicsDriver::Get()->GetWindowWidth(), GraphicsDriver::Get()->GetWindowHeight() );
		mDepthStencilState = GraphicsDriver::Get()->CreateDepthStencilState( true, EComparisonFunc::ECF_Less );
		GraphicsDriver::Get()->SetDepthStencil(mDepthStencilView);
		GraphicsDriver::Get()->SetDepthStencilState(mDepthStencilState);
	}

	void Renderer::AddPrimitive( const RenderPrimitivePtr primitive )
	{
		mPrimitives.push_back( primitive );
	}

	void Renderer::Render()
	{
		BeginRender();
		PrimitiveDrawer drawer;
		for ( const RenderPrimitivePtr& primitive : mPrimitives )
		{
			primitive->Draw( drawer );
		}
		FinishRender();
	}

	void Renderer::BeginRender()
	{
		// Clear back buffer and depth stencil
		GraphicsDriver::Get()->ClearBackBuffer();
		GraphicsDriver::Get()->ClearDepthStencil( mDepthStencilView, 1.0f );
	}

	void Renderer::FinishRender()
	{
		// Present!
		ITP485::GraphicsDriver::Get()->Present();
	}

	void Renderer::SetObjectToWorldMatrix( const Matrix4& matrix )
	{
	}
}