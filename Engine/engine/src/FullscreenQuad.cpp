#include "FullscreenQuad.h"

#include <PrecompiledHeader.h>

static const unsigned int NUM_VERTICES = 4;
static const unsigned int NUM_INDICES = 6;

namespace ITP485
{

	FullscreenQuad::FullscreenQuad()
	{
		// Create our vertex buffer
		VERTEX_P_T vertices[NUM_VERTICES] = {
				{ PackedVector3( -1.f, 1.f, 0.f ), PackedVector2( 0.f, 0.f ) },
				{ PackedVector3( 1.f, 1.f, 0.f ), PackedVector2( 1.f, 0.f ) },
				{ PackedVector3( 1.f, -1.f, 0.f ), PackedVector2( 1.f, 1.f ) },
				{ PackedVector3( -1.f, -1.f, 0.f ), PackedVector2( 0.f, 1.f ) }
		};
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices, sizeof( VERTEX_P_T ) * NUM_VERTICES, EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// Compile vertex shader
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\additive.hlsl", "VS", "vs_4_0", compiledVertexShader );
		mVertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

		// Create an input layout
		InputLayoutElement elements[2] {
			/* from vertex buffer */
			{ "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0, 0, EIC_PerVertex, 0 },
			{ "TEXCOORD", 0, EGFormat::EGF_R32G32_Float, 0, sizeof( float ) * 3, EIC_PerVertex, 0 },
		};
		mInputLayout = GraphicsDriver::Get()->CreateInputLayout( elements, 2, compiledVertexShader );

		// We don't need an object to world constant buffer
		mUniformBuffer = nullptr;

		// Create our index buffer
		uint16_t indexes[NUM_INDICES] = { 0, 1, 2, 2, 3, 0 };
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indexes, sizeof( uint16_t ) * NUM_INDICES, EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		mMaterial = MaterialPtr( new Material( L"Resources\\Shaders\\additive.hlsl", L"" ) );
		mNumIndices = NUM_INDICES;
		mVertexStride = sizeof( VERTEX_P_T );
	}
}
