#include "Renderer.h"

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

		// Create a blend state
		mBlendState = GraphicsDriver::Get()->CreateBlendState( EBlend::EB_Src_Alpha, EBlend::Eb_Inv_Src_Alpha );
		GraphicsDriver::Get()->SetBlendState( mBlendState );

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

		// Create the player view constant buffer
		mCameraConstantBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( nullptr, sizeof( PerCameraConstants ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
		GraphicsDriver::Get()->SetVSConstantBuffer( mCameraConstantBuffer, 0 );
		GraphicsDriver::Get()->SetPSConstantBuffer( mCameraConstantBuffer, 0 );

		// Initialize text output
		GraphicsDriver::Get()->InitSpriteFont();
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

	void Renderer::Render()
	{
		// Reset the state which gets blown away by SpriteBatch.
		GraphicsDriver::Get()->SetVSConstantBuffer( mCameraConstantBuffer, 0 );
		GraphicsDriver::Get()->SetPSConstantBuffer( mCameraConstantBuffer, 0 );
		GraphicsDriver::Get()->SetDepthStencilState( mDepthStateNormal );
		GraphicsDriver::Get()->SetBlendState( mBlendState );

		// Clear back buffer and depth stencil
		GraphicsDriver::Get()->ClearBackBuffer();

		// Shadow depth pass
		if ( mLight )
		{
			GraphicsDriver::Get()->ClearDepthStencil( mShadowMapDepthStencil, 1.0f );
			GraphicsDriver::Get()->SetDepthStencil( mShadowMapDepthStencil );
			UpdateViewConstants( mLight->GetProjectionViewMatrix(), mLight->GetPosition() );	// TODO: should this be in the Renderer?
			for ( const RenderPrimitivePtr& primitive : mPrimitives )	// render opaque primitives
			{
				primitive->Draw( mShadowPassDrawer, mLight );
			}
			for ( const RenderPrimitivePtr& primitive : mTranslucentPrimitives )	// render transclucent primitives
			{
				primitive->Draw( mShadowPassDrawer, mLight );
			}

			GraphicsDriver::Get()->ClearBackBuffer();
		}

		// Base pass
		GraphicsDriver::Get()->ClearDepthStencil( mDepthStencilView, 1.0f );
		GraphicsDriver::Get()->SetDepthStencil( mDepthStencilView );
		UpdateViewConstants( mCamera->GetProjectionViewMatrix(), mCamera->GetPosition() );
		GraphicsDriver::Get()->SetPSTexture( mShadowMapTexture, 1 );
		for ( const RenderPrimitivePtr& primitive : mPrimitives )
		{
			primitive->Draw( mBasePassDrawer, mCamera );
		}

		// Translucent pass
		for ( const RenderPrimitivePtr& primitive : mTranslucentPrimitives )
		{
			primitive->Draw( mTransclucentDrawer, mCamera );
		}

		GraphicsDriver::Get()->SpriteFontBegin();
		wchar_t buffer[100];
		_swprintf( buffer, L"Frame time: %f\nFPS: %f", Timing::Get().GetDeltaTime(), 1.f / Timing::Get().GetDeltaTime() );
		GraphicsDriver::Get()->DrawSpriteFontString( buffer, 0.f, 0.f );
		GraphicsDriver::Get()->SpriteFontEnd();

		// Present!
		ITP485::GraphicsDriver::Get()->Present();

	}

	void Renderer::UpdateViewConstants( const Matrix4& projectionView, const Vector3& position ) const
	{
		PerCameraConstants *cameraConstants = static_cast<PerCameraConstants*>(GraphicsDriver::Get()->MapBuffer(mCameraConstantBuffer));
		cameraConstants->mProjectionViewMatrix = projectionView.GetTranspose();
		cameraConstants->mCameraPosition = position;
		if ( mLight )
		{
			cameraConstants->mLightMatrix = mLight->GetProjectionViewMatrix().GetTranspose();	// TODO: refactor
		}
		cameraConstants->mCameraUp = mCamera->GetUpVector();
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

		// TODO: figure out a way so we don't have to deativate materials
		mesh.material->DeactivateMaterial();

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
