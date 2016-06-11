#include "Renderer.h"
#include "FullscreenQuad.h"

namespace ITP485
{

	//////////////////////////
	// Renderer
	//////////////////////////

	// TODO: figure this out
	DepthStencilStatePtr mDepthStateNormal;
	DepthStencilStatePtr mDepthStateReadOnly;

	void Renderer::Initialize()
	{
		// Create a solid fill rasterizer state
		mRasterizerState = GraphicsDriver::Get()->CreateRasterizerState( EFM_Solid, ECM_Back );
		GraphicsDriver::Get()->SetRasterizerState( mRasterizerState );

		// Create a blend states
		mBlendStateNormal = GraphicsDriver::Get()->CreateBlendState( EBlend::EB_One, EBlend::EB_Zero );
		mBlendStateAdditive = GraphicsDriver::Get()->CreateBlendState( EBlend::EB_One, EBlend::EB_One );

		// Set up our depth buffer and depth test
		mDepthStencilView = GraphicsDriver::Get()->CreateDepthStencil( GraphicsDriver::Get()->GetWindowWidth(), GraphicsDriver::Get()->GetWindowHeight() );
		mDepthStateNormal = GraphicsDriver::Get()->CreateDepthStencilState( true, EComparisonFunc::ECF_Less, EDepthWriteMask::EDWM_All );
		mDepthStateReadOnly = GraphicsDriver::Get()->CreateDepthStencilState( true, EComparisonFunc::ECF_Less, EDepthWriteMask::EDWM_None );
		GraphicsDriver::Get()->SetDepthStencilState( mDepthStateNormal );

		// Create our shadow map target and texture
		GraphicsDriver::Get()->CreateDepthStencilAndTexture( GraphicsDriver::Get()->GetWindowWidth(), GraphicsDriver::Get()->GetWindowHeight(), mShadowMapDepthStencil, mShadowMapTexture );

		// Create a shadow map sampler state
		mShadowMapSamplerState = GraphicsDriver::Get()->CreateSamplerState( ETextureAddressMode::EClamp, ETextureAddressMode::EClamp );
		GraphicsDriver::Get()->SetPSSamplerState( mShadowMapSamplerState, 1 );
		GraphicsDriver::Get()->SetPSTexture( mShadowMapTexture, 1 );

		// Create off-screen render target
		GraphicsDriver::Get()->CreateRenderTargetAndTexture( GraphicsDriver::Get()->GetWindowWidth(), GraphicsDriver::Get()->GetWindowHeight(), mBasePassRenderTarget, mBasePassTexture );

		// Create the player view constant buffer
		mCameraConstantBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( nullptr, sizeof( PerCameraConstants ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
		GraphicsDriver::Get()->SetVSConstantBuffer( mCameraConstantBuffer, 0 );
		GraphicsDriver::Get()->SetPSConstantBuffer( mCameraConstantBuffer, 0 );

		// Create fullscreen quad
		mFullscreenQuad = MeshPrimitivePtr( new FullscreenQuad() );

		// Calculate samples
		for ( int i = 0; i < OCCLUSION_SAMPLES; i++ )
		{
			bool validVector = false;
			Vector3 vec;
			vec = Vector3( RandomFloat( -1.f, 1.f ), RandomFloat( 0.f, 1.f ), RandomFloat( -1.f, 1.f ) );
			vec.Normalize();
			mUniformHemisphereSample[i] = vec * 10.f;
		}
	}

	void Renderer::AddPrimitive( const RenderPrimitivePtr primitive )
	{
		mPrimitives.push_back( primitive );
	}

	void Renderer::AddTranslucentPrimitive( const RenderPrimitivePtr primitive )
	{
		mTranslucentPrimitives.push_back( primitive );
	}

	void Renderer::SetCamera( const ViewPtr view )
	{
		mCamera = view;
	}

	void Renderer::SetLight( const ViewPtr light )
	{
		mLight = light;
	}

	void Renderer::RenderSingleLight( const Vector3& lightPosition )
	{
		// Position light
		mLight->SetPosition( lightPosition );
		mLight->LookAt( 0.f, 0.f, 0.f );	// TODO: don't allow setting view position separate from look

		// Shadow depth pass
		GraphicsDriver::Get()->SetBlendState( mBlendStateNormal );
		GraphicsDriver::Get()->ClearDepthStencil( mShadowMapDepthStencil, 1.0f );
		GraphicsDriver::Get()->SetDepthStencil( mShadowMapDepthStencil );	// TODO: merge this and next statement together, more granular control
		GraphicsDriver::Get()->SetDepthStencilState( mDepthStateNormal );
		GraphicsDriver::Get()->SetRenderTarget( nullptr );	// disable color writing
		UpdateViewConstants( mLight->GetProjectionViewMatrix(), lightPosition );	// TODO: should this be in the Renderer?
		for ( const RenderPrimitivePtr& primitive : mPrimitives )	// render opaque primitives
		{
			primitive->Draw( mShadowPassDrawer, mLight );
		}

		// Base pass
		GraphicsDriver::Get()->ClearDepthStencil( mDepthStencilView, 1.0f );
		GraphicsDriver::Get()->ClearRenderTarget( mBasePassRenderTarget, DirectX::Colors::Black );
		GraphicsDriver::Get()->SetDepthStencil( mDepthStencilView );
		GraphicsDriver::Get()->SetRenderTarget( mBasePassRenderTarget );
		UpdateViewConstants( mCamera->GetProjectionViewMatrix(), mCamera->GetPosition() );
		GraphicsDriver::Get()->SetPSTexture( mShadowMapTexture, 1 );
		for ( const RenderPrimitivePtr& primitive : mPrimitives )
		{
			primitive->Draw( mBasePassDrawer, mCamera );
		}

		// Accumulation pass- full screen quad
		GraphicsDriver::Get()->SetBlendState( mBlendStateAdditive );
		GraphicsDriver::Get()->ClearDepthStencil( mDepthStencilView, 1.0f );
		GraphicsDriver::Get()->SetRenderTarget( GraphicsDriver::Get()->GetBackBufferRenderTarget() );
		GraphicsDriver::Get()->SetDepthStencil( mDepthStencilView );
		GraphicsDriver::Get()->SetDepthStencilState( mDepthStateReadOnly );
		GraphicsDriver::Get()->SetPSTexture( mBasePassTexture, 1 );
		mFullscreenQuad->Draw( mBasePassDrawer, mCamera );
	}

	void Renderer::Render()
	{
		// Clear back buffer
		GraphicsDriver::Get()->ClearBackBuffer();

		for ( int i = 0; i < OCCLUSION_SAMPLES; i++ )
		{
			RenderSingleLight( mUniformHemisphereSample[i] );
		}

		// Present!
		ITP485::GraphicsDriver::Get()->Present();
	}

	void Renderer::UpdateViewConstants( const Matrix4& projectionView, const Vector3& position ) const
	{
		PerCameraConstants *cameraConstants = static_cast<PerCameraConstants*>(GraphicsDriver::Get()->MapBuffer(mCameraConstantBuffer));
		cameraConstants->mProjectionViewMatrix = projectionView.GetTranspose();
		cameraConstants->mCameraPosition = position;
		cameraConstants->mLightMatrix = mLight->GetProjectionViewMatrix().GetTranspose();	// TODO: refactor
		GraphicsDriver::Get()->UnmapBuffer(mCameraConstantBuffer);
	}

	//////////////////////////
	// PrimitiveDrawer
	//////////////////////////

	void PrimitiveDrawer::ActivateMaterial( const PrimitiveDrawer::Mesh& mesh ) const
	{
		mesh.material->ActivateMaterial();
	}

	bool PrimitiveDrawer::SetDepthState( const PrimitiveDrawer::Mesh& mesh ) const
	{
		// We always want to write to the depth buffer, so assume the implicit normal depth state.
		return false;
	}

	void PrimitiveDrawer::DrawMesh( const PrimitiveDrawer::Mesh& mesh ) const
	{
		bool needToRestoreDepthState = SetDepthState( mesh );

		if ( mesh.vertexUniformBuffer != nullptr )	// not all meshes need a uniform buffer
		{
			GraphicsDriver::Get()->SetVSConstantBuffer( mesh.vertexUniformBuffer, 1 );
		}

		if ( mesh.fragmentUniformBuffer != nullptr )
		{
			GraphicsDriver::Get()->SetPSConstantBuffer( mesh.fragmentUniformBuffer, 1 );
		}

		GraphicsDriver::Get()->SetVertexShader( mesh.vertexShader );
		GraphicsDriver::Get()->SetInputLayout( mesh.inputLayout );
		GraphicsDriver::Get()->SetIndexBuffer( mesh.indexBuffer );

		// Activate the material
		ActivateMaterial( mesh );

		// Draw!
		if ( mesh.instanceCount > 1 )
		{
			GraphicsDriver::Get()->SetVertexBuffers( mesh.vertexBuffer, mesh.vertexStride, mesh.instanceBuffer, mesh.instanceStride );
			GraphicsDriver::Get()->DrawIndexedInstanced( mesh.indices, mesh.instanceCount, 0, 0, 0 );
		}
		else
		{
			GraphicsDriver::Get()->SetVertexBuffer( mesh.vertexBuffer, mesh.vertexStride );
			GraphicsDriver::Get()->DrawIndexed( mesh.indices, 0, 0 );
		}

		// Restore depth buffer state.
		if ( needToRestoreDepthState )
		{
			GraphicsDriver::Get()->SetDepthStencilState( mDepthStateNormal );
		}
	}

	//////////////////////////
	// DepthOnlyDrawer
	//////////////////////////

	void DepthOnlyDrawer::ActivateMaterial( const PrimitiveDrawer::Mesh& mesh ) const
	{
		mesh.material->ActivateDepthOnlyMaterial();
	}

	//////////////////////////
	// TranslucentDrawer
	//////////////////////////

	bool TransclucentDrawer::SetDepthState( const PrimitiveDrawer::Mesh& mesh ) const
	{
		GraphicsDriver::Get()->SetDepthStencilState( mDepthStateReadOnly );
		return true;
	}

}
