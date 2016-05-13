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
		mDepthStencilState = GraphicsDriver::Get()->CreateDepthStencilState( true, EComparisonFunc::ECF_Greater );
		GraphicsDriver::Get()->SetDepthStencil(mDepthStencilView);
		GraphicsDriver::Get()->SetDepthStencilState(mDepthStencilState);

		// Create our object to world constant buffer
		mObjectToWorldBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( Matrix4 ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
		GraphicsDriver::Get()->SetVSConstantBuffer( mObjectToWorldBuffer, 1 );
	}

	void Renderer::BeginRender()
	{
		// Clear back buffer and depth stencil
		GraphicsDriver::Get()->ClearBackBuffer();
		GraphicsDriver::Get()->ClearDepthStencil( mDepthStencilView, 0.0f );

		// Empty render entities vector
		mEntities.clear();
	}

	void Renderer::Submit( MeshPtr mesh, MaterialPtr material, const Matrix4& translation )
	{
		RenderEntity entity;
		entity.mMesh = mesh;
		entity.mMaterial = material;
		entity.mModelTransform = translation;
		mEntities.push_back( entity );
	}

	void Renderer::Render()
	{
		for ( const auto& entity : mEntities )
		{
			// Set object to world matrix
			Matrix4 *objectToWorld = static_cast<Matrix4*>(GraphicsDriver::Get()->MapBuffer( mObjectToWorldBuffer ));
			memcpy( objectToWorld, &entity.mModelTransform.GetTranspose(), sizeof( Matrix4 ) );
			GraphicsDriver::Get()->UnmapBuffer( mObjectToWorldBuffer );

			// Draw
			entity.mMaterial->ActivateMaterial();
			entity.mMesh->Draw();
		}
	}

	void Renderer::FinishRender()
	{
		// Present!
		ITP485::GraphicsDriver::Get()->Present();
	}
}