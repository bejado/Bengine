#include <PrecompiledHeader.h>
#include "Quad.h"
#include "Renderer.h"

static const unsigned int NUM_VERTICES = 4;
static const unsigned int NUM_INDICES = 6;

namespace ITP485
{

	Quad::Quad( uint32_t instanceDataSize, uint32_t instanceCount ) : mInstanceDataSize (instanceDataSize ), mInstanceCount (instanceCount )
	{
		// Compile vertex shader
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\instanced_billboard.hlsl", "VS", "vs_4_0", compiledVertexShader );
		mVertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

		// Create an input layout
		InputLayoutElement elements[5] {
			/* from vertex buffer */
			{ "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0, 0, EIC_PerVertex, 0 },
			{ "NORMAL", 0, EGFormat::EGF_R32G32B32_Float, 0, sizeof( float ) * 3, EIC_PerVertex, 0 },
			{ "TEXCOORD", 0, EGFormat::EGF_R32G32_Float, 0, sizeof( float ) * 6, EIC_PerVertex, 0 },
			/* from index buffer */
			{ "INSTANCEPOS", 0, EGFormat::EGF_R32G32B32A32_Float, 1, 0, EIC_PerInstance, 1 },
			{ "INSTANCEAGE", 0, EGFormat::EGF_R32_Float, 1, sizeof( float ) * 4, EIC_PerInstance, 1 }
		};
		mInputLayout = GraphicsDriver::Get()->CreateInputLayout( elements, 5, compiledVertexShader );

		// Create vertex buffer
		VERTEX_P_N_T vertices[NUM_VERTICES] = {
				{ PackedVector3( -.5f, -.5f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 0.f, 1.f) },
				{ PackedVector3( .5f, -.5f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 1.f, 1.f) },
				{ PackedVector3( .5f, .5f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 1.f, 0.f) },
				{ PackedVector3( -.5f, .5f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 0.f, 0.f) }
		};
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices, sizeof( VERTEX_P_N_T ) * NUM_VERTICES, EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// Create vertex uniform buffer
		mUniformBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( UniformBuffer ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );

		// Create an instance buffer- we set this to nullptr for now, clients will map and unmap the buffer themselves
		mInstanceBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( nullptr, mInstanceDataSize * mInstanceCount, EBindflags::EBF_VertexBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );

		// Create index buffer
		uint16_t indexes[NUM_INDICES] = { 0, 3, 1, 1, 3, 2 };
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indexes, sizeof( uint16_t ) * NUM_INDICES, EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );
	}

	void Quad::FillOutMeshStruct( PrimitiveDrawer::Mesh* const mesh )
	{
		mesh->inputLayout = mInputLayout;
		mesh->vertexBuffer = mVertexBuffer;
		mesh->vertexUniformBuffer = mUniformBuffer;
		mesh->vertexShader = mVertexShader;
		mesh->vertexStride = sizeof( VERTEX_P_N_T );
		mesh->instanceStride = mInstanceDataSize;
		mesh->indexBuffer = mIndexBuffer;
		mesh->indices = NUM_INDICES;
		mesh->instanceBuffer = mInstanceBuffer;
	}

	void Quad::UpdateUniformBuffer( const Matrix4& translation )
	{
		UniformBuffer *buffer = GraphicsDriver::Get()->MapBuffer<UniformBuffer>( mUniformBuffer );
		memcpy( buffer, &translation.GetTranspose(), sizeof( Matrix4 ) );
		GraphicsDriver::Get()->UnmapBuffer( mUniformBuffer );
	}

	void* Quad::MapInstanceBuffer()
	{
		return GraphicsDriver::Get()->MapBuffer<void>( mInstanceBuffer );
	}

	void Quad::UnmapInstanceBuffer()
	{
		GraphicsDriver::Get()->UnmapBuffer( mInstanceBuffer );
	}
}

