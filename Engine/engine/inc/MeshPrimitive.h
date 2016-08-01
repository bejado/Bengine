#pragma once

#include "Renderer.h"
#include "RawMesh.h"

namespace BNG
{

	struct VertexSource
	{
		InputLayoutPtr inputLayout;
		VertexShaderPtr vertexShader;
		size_t vertexStride;
		bool positionOnly;
	};

	static const VertexSource GetTangentSpaceVertexSource()
	{
		static VertexSource TangentVertexSource;
		static bool initialized = false;

		if ( !initialized )
		{
			// Compile vertex shader.
			vector< char > compiledVertexShader;
			BNG::GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\tangent.hlsl", "VS", "vs_4_0", compiledVertexShader );
			VertexShaderPtr vertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

			// Create an input layout.
			InputLayoutElement elements[4] {
				{ "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0, 0, EIC_PerVertex, 0 },
				{ "NORMAL", 0, EGFormat::EGF_R32G32B32_Float, 0, sizeof( float ) * 3, EIC_PerVertex, 0 },
				{ "TEXCOORD", 0, EGFormat::EGF_R32G32_Float, 0, sizeof( float ) * 6, EIC_PerVertex, 0 },
				{ "TANGENT", 0, EGFormat::EGF_R32G32B32A32_Float, 0, sizeof( float ) * 8, EIC_PerVertex, 0 },
			};
			InputLayoutPtr inputLayout = GraphicsDriver::Get()->CreateInputLayout( elements, 4, compiledVertexShader );

			TangentVertexSource.inputLayout = inputLayout;
			TangentVertexSource.vertexShader = vertexShader;
			TangentVertexSource.vertexStride = sizeof( VERTEX_P_N_T_T );
			TangentVertexSource.positionOnly = false;

			initialized = true;
		}

		return TangentVertexSource;
	}

	static const VertexSource GetPositionOnlyVertexSource()
	{
		static VertexSource PositionOnlyVertexSource;
		static bool initialized = false;

		if ( !initialized )
		{
			// Compile vertex shader.
			vector< char > compiledVertexShader;
			BNG::GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\wireframe.hlsl", "VS", "vs_4_0", compiledVertexShader );
			VertexShaderPtr vertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

			// Create an input layout.
			InputLayoutElement elements[1] {
				{ "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0, 0, EIC_PerVertex, 0 }
			};
			InputLayoutPtr inputLayout = GraphicsDriver::Get()->CreateInputLayout( elements, 1, compiledVertexShader );

			PositionOnlyVertexSource.inputLayout = inputLayout;
			PositionOnlyVertexSource.vertexShader = vertexShader;
			PositionOnlyVertexSource.vertexStride = sizeof( VERTEX_P );
			PositionOnlyVertexSource.positionOnly = true;

			initialized = true;
		}

		return PositionOnlyVertexSource;
	}

	class MeshPrimitive : public RenderPrimitive
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		MeshPrimitive( const RawMeshPtr rawMesh,
					   const MaterialPtr material,
					   const VertexSource& vertexSource );
		virtual void Draw( const PrimitiveDrawer& drawer, const ViewPtr view ) const;
		void SetObjectToWorldMatrix( const Matrix4& matrix );

	protected:

		VertexSource mVertexSource;
		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;
		GraphicsBufferPtr mUniformBuffer;
		MaterialPtr mMaterial;
		size_t mNumIndices;

	private:

		void UpdateConstantBuffer();

		struct ObjectConstants
		{
			Matrix4 objectToWorld;
			Matrix4 worldToObject;
		};

		Matrix4 mObjectToWorldMatrix;
		RawMeshPtr mRawMesh;

	};

	typedef shared_ptr< MeshPrimitive > MeshPrimitivePtr;

}
