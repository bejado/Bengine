#include "Cube.h"

#include <PrecompiledHeader.h>

static const unsigned int NUM_VERTICES = 24;
static const unsigned int NUM_INDICES = 36;

namespace ITP485
{

	CubePrimitive::CubePrimitive()
	{
		// Create our vertex buffer
		VERTEX_P_N_T vertices[NUM_VERTICES] = {
				// front face
				{ PackedVector3( 0.f, 0.f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 0.f, 1.f) },
				{ PackedVector3( 1.f, 0.f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 1.f, 1.f) },
				{ PackedVector3( 1.f, 1.f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 1.f, 0.f) },
				{ PackedVector3( 0.f, 1.f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 0.f, 0.f) },

				// back face
				{ PackedVector3( 0.f, 0.f, 1.f ), PackedVector3( 0.f, 0.f, 1.f ), PackedVector2( 1.f, 1.f) },
				{ PackedVector3( 1.f, 0.f, 1.f ), PackedVector3( 0.f, 0.f, 1.f ), PackedVector2( 0.f, 1.f) },
				{ PackedVector3( 1.f, 1.f, 1.f ), PackedVector3( 0.f, 0.f, 1.f ), PackedVector2( 0.f, 0.f) },
				{ PackedVector3( 0.f, 1.f, 1.f ), PackedVector3( 0.f, 0.f, 1.f ), PackedVector2( 1.f, 0.f) },

				// left face
				{ PackedVector3( 0.f, 0.f, 1.f ), PackedVector3( -1.f, 0.f, 0.f ), PackedVector2( 0.f, 1.f) },
				{ PackedVector3( 0.f, 0.f, 0.f ), PackedVector3( -1.f, 0.f, 0.f ), PackedVector2( 1.f, 1.f) },
				{ PackedVector3( 0.f, 1.f, 0.f ), PackedVector3( -1.f, 0.f, 0.f ), PackedVector2( 1.f, 0.f) },
				{ PackedVector3( 0.f, 1.f, 1.f ), PackedVector3( -1.f, 0.f, 0.f ), PackedVector2( 0.f, 0.f) },

				// right face
				{ PackedVector3( 1.f, 0.f, 1.f ), PackedVector3( 1.f, 0.f, 0.f ), PackedVector2( 1.f, 1.f) },
				{ PackedVector3( 1.f, 0.f, 0.f ), PackedVector3( 1.f, 0.f, 0.f ), PackedVector2( 0.f, 1.f) },
				{ PackedVector3( 1.f, 1.f, 0.f ), PackedVector3( 1.f, 0.f, 0.f ), PackedVector2( 0.f, 0.f) },
				{ PackedVector3( 1.f, 1.f, 1.f ), PackedVector3( 1.f, 0.f, 0.f ), PackedVector2( 1.f, 0.f) },

				// bottom face
				{ PackedVector3( 0.f, 0.f, 0.f ), PackedVector3( 0.f, -1.f, 0.f ), PackedVector2( 0.f, 0.f) },
				{ PackedVector3( 1.f, 0.f, 0.f ), PackedVector3( 0.f, -1.f, 0.f ), PackedVector2( 1.f, 0.f) },
				{ PackedVector3( 1.f, 0.f, 1.f ), PackedVector3( 0.f, -1.f, 0.f ), PackedVector2( 1.f, 1.f) },
				{ PackedVector3( 0.f, 0.f, 1.f ), PackedVector3( 0.f, -1.f, 0.f ), PackedVector2( 0.f, 1.f) },

				// top face
				{ PackedVector3( 0.f, 1.f, 0.f ), PackedVector3( 0.f, 1.f, 0.f ), PackedVector2( 0.f, 1.f) },
				{ PackedVector3( 1.f, 1.f, 0.f ), PackedVector3( 0.f, 1.f, 0.f ), PackedVector2( 1.f, 1.f) },
				{ PackedVector3( 1.f, 1.f, 1.f ), PackedVector3( 0.f, 1.f, 0.f ), PackedVector2( 1.f, 0.f) },
				{ PackedVector3( 0.f, 1.f, 1.f ), PackedVector3( 0.f, 1.f, 0.f ), PackedVector2( 0.f, 0.f) }
		};
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices, sizeof( VERTEX_P_N_T ) * NUM_VERTICES, EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// Compile vertex shader
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\shadow.hlsl", "VS", "vs_4_0", compiledVertexShader );
		mVertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

		// Create an input layout
		InputLayoutElement elements[3] {
			/* from vertex buffer */
			{ "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0, 0, EIC_PerVertex, 0 },
			{ "NORMAL", 0, EGFormat::EGF_R32G32B32_Float, 0, sizeof( float ) * 3, EIC_PerVertex, 0 },
			{ "TEXCOORD", 0, EGFormat::EGF_R32G32_Float, 0, sizeof( float ) * 6, EIC_PerVertex, 0 },
		};
		mInputLayout = GraphicsDriver::Get()->CreateInputLayout( elements, 3, compiledVertexShader );

		// Create our object to world constant buffer
		mUniformBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( Matrix4 ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );

		// Create our index buffer
		uint16_t indexes[NUM_INDICES] = { 0, 3, 1, 1, 3, 2,			// front
										  5, 7, 4, 6, 7, 5,			// back
										  8, 11, 9, 9, 11, 10,		// left
										  13, 15, 12, 14, 15, 13,	// right
										  16, 17, 18, 18, 19, 16,	// bottom
										  22, 21, 20, 20, 23, 22 };	// top
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indexes, sizeof( uint16_t ) * NUM_INDICES, EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		mMaterial = MaterialPtr( new Material( L"Resources\\Shaders\\shadow.hlsl", L"Resources\\Textures\\crate.dds" ) );

		mNumIndices = NUM_INDICES;
	}
}
