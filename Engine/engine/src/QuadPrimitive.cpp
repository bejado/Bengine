#include "Cube.h"

#include <PrecompiledHeader.h>

static const unsigned int NUM_VERTICES = 4;
static const unsigned int NUM_INDICES = 6;

namespace ITP485
{

	QuadPrimitive::QuadPrimitive()
	{
		// Create our vertex buffer
		VERTEX_P_N_T_T vertices[NUM_VERTICES] = {
				{ PackedVector3( -1.f, 1.f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 0.f, 0.f ), PackedVector4( 1.f, 0.f, 0.f, -1.f ) },
				{ PackedVector3( 1.f, 1.f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 1.f, 0.f ), PackedVector4( 1.f, 0.f, 0.f, -1.f ) },
				{ PackedVector3( 1.f, -1.f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 1.f, 1.f ), PackedVector4( 1.f, 0.f, 0.f, -1.f ) },
				{ PackedVector3( -1.f, -1.f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 0.f, 1.f ), PackedVector4( 1.f, 0.f, 0.f, -1.f ) }
		};
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices, sizeof( VERTEX_P_N_T_T ) * NUM_VERTICES, EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// Compile vertex shader
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\tangent.hlsl", "VS", "vs_4_0", compiledVertexShader );
		mVertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

		// Create an input layout
		InputLayoutElement elements[4] {
			/* from vertex buffer */
			{ "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0, 0, EIC_PerVertex, 0 },
			{ "NORMAL", 0, EGFormat::EGF_R32G32B32_Float, 0, sizeof( float ) * 3, EIC_PerVertex, 0 },
			{ "TEXCOORD", 0, EGFormat::EGF_R32G32_Float, 0, sizeof( float ) * 6, EIC_PerVertex, 0 },
			{ "TANGENT", 0, EGFormat::EGF_R32G32B32A32_Float, 0, sizeof( float ) * 8, EIC_PerVertex, 0 },
		};
		mInputLayout = GraphicsDriver::Get()->CreateInputLayout( elements, 4, compiledVertexShader );

		// Create our object to world constant buffer
		mUniformBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( Matrix4 ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );

		// Create our index buffer
		uint16_t indexes[NUM_INDICES] = { 0, 1, 2, 2, 3, 0 };
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indexes, sizeof( uint16_t ) * NUM_INDICES, EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		mMaterial = MaterialPtr( new Material( L"Resources\\Shaders\\tangent.hlsl", L"Resources\\Textures\\normal2.dds" ) );
		mNumIndices = NUM_INDICES;
		mVertexStride = sizeof( VERTEX_P_N_T_T );
	}
}
