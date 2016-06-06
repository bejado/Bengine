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
		mRasterizerState = GraphicsDriver::Get()->CreateRasterizerState( EFM_Solid );
		GraphicsDriver::Get()->SetRasterizerState( mRasterizerState );

		// Create a blend state
		mBlendState = GraphicsDriver::Get()->CreateBlendState();
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
	}

	void Renderer::AddPrimitive( const RenderPrimitivePtr primitive )
	{
		mPrimitives.push_back( primitive );
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
		// Clear back buffer and depth stencil
		GraphicsDriver::Get()->ClearBackBuffer();

		// Shadow depth pass
		GraphicsDriver::Get()->ClearDepthStencil( mShadowMapDepthStencil, 1.0f );
		GraphicsDriver::Get()->SetDepthStencil( mShadowMapDepthStencil );
		UpdateViewConstants( mLight->GetProjectionViewMatrix(), mLight->GetPosition() );	// TODO: should this be in the Renderer?
		for ( const RenderPrimitivePtr& primitive : mPrimitives )
		{
			primitive->Draw( mShadowPassDrawer, mLight );
		}

		GraphicsDriver::Get()->ClearBackBuffer();

		// Base pass
		GraphicsDriver::Get()->ClearDepthStencil( mDepthStencilView, 1.0f );
		GraphicsDriver::Get()->SetDepthStencil( mDepthStencilView );
		UpdateViewConstants( mCamera->GetProjectionViewMatrix(), mCamera->GetPosition() );
		GraphicsDriver::Get()->SetPSTexture( mShadowMapTexture, 1 );
		for ( const RenderPrimitivePtr& primitive : mPrimitives )
		{
			primitive->Draw( mBasePassDrawer, mCamera );
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

	void PrimitiveDrawer::DrawMesh( const PrimitiveDrawer::Mesh& mesh ) const
	{
		if ( mesh.vertexUniformBuffer != nullptr )	// not all meshes need a uniform buffer
		{
			GraphicsDriver::Get()->SetVSConstantBuffer( mesh.vertexUniformBuffer, 1 );
		}
		GraphicsDriver::Get()->SetVertexShader( mesh.vertexShader );
		GraphicsDriver::Get()->SetInputLayout( mesh.inputLayout );
		GraphicsDriver::Get()->SetVertexBuffer( mesh.vertexBuffer, mesh.vertexStride );	// TODO: here's where the vertex factory comes in
		GraphicsDriver::Get()->SetIndexBuffer( mesh.indexbuffer );
		mesh.material->ActivateMaterial();

		// Draw!
		GraphicsDriver::Get()->DrawIndexed( mesh.indices, 0, 0 );
	}

	void PrimitiveDrawer::DrawInstancedMesh( const PrimitiveDrawer::InstancedMesh& mesh ) const
	{
		GraphicsDriver::Get()->SetDepthStencilState( mDepthStateReadOnly );
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
		GraphicsDriver::Get()->SetVertexBuffers( mesh.vertexBuffer, mesh.vertexStride, mesh.instanceBuffer, mesh.instanceStride );
		GraphicsDriver::Get()->SetIndexBuffer( mesh.indexbuffer );
		mesh.material->ActivateMaterial();

		// Draw!
		GraphicsDriver::Get()->DrawIndexedInstanced( mesh.indices, mesh.instanceCount, 0, 0, 0 );
		GraphicsDriver::Get()->SetDepthStencilState( mDepthStateNormal );
	}

	DepthOnlyDrawer::DepthOnlyDrawer()
	{
		// Load the pixel shader
		vector< char > compiledPixelShader;
		GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\shadow.hlsl", "DepthOnly", "ps_4_0", compiledPixelShader );		// TODO: this is a hack
		mDepthOnlyShader = GraphicsDriver::Get()->CreatePixelShader( compiledPixelShader );

		compiledPixelShader.clear();
		GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\Particle.hlsl", "DepthOnly", "ps_4_0", compiledPixelShader );		// TODO: this is a hack
		mInstancedDepthOnlyShader = GraphicsDriver::Get()->CreatePixelShader( compiledPixelShader );
	}

	void DepthOnlyDrawer::DrawMesh( const PrimitiveDrawer::Mesh& mesh ) const
	{
		GraphicsDriver::Get()->SetVSConstantBuffer( mesh.vertexUniformBuffer, 1 );
		GraphicsDriver::Get()->SetVertexShader( mesh.vertexShader );
		GraphicsDriver::Get()->SetInputLayout( mesh.inputLayout );
		GraphicsDriver::Get()->SetVertexBuffer( mesh.vertexBuffer, mesh.vertexStride );
		GraphicsDriver::Get()->SetIndexBuffer( mesh.indexbuffer );
		GraphicsDriver::Get()->SetPixelShader( mDepthOnlyShader );

		// Draw!
		GraphicsDriver::Get()->DrawIndexed( mesh.indices, 0, 0 );
	}

	void DepthOnlyDrawer::DrawInstancedMesh( const PrimitiveDrawer::InstancedMesh& mesh ) const
	{
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
		GraphicsDriver::Get()->SetVertexBuffers( mesh.vertexBuffer, mesh.vertexStride, mesh.instanceBuffer, mesh.instanceStride );
		GraphicsDriver::Get()->SetIndexBuffer( mesh.indexbuffer );
		GraphicsDriver::Get()->SetPixelShader( mInstancedDepthOnlyShader );

		// Draw!
		GraphicsDriver::Get()->DrawIndexedInstanced( mesh.indices, mesh.instanceCount, 0, 0, 0 );
	}

}
