#include <PrecompiledHeader.h>

static const unsigned int NUM_VERTICES = 4;
static const unsigned int NUM_INDICES = 6;

namespace ITP485
{

	Quad::Quad()
	{
		// Compile vertex shader
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Shaders\\instanced_billboard.hlsl", "VS", "vs_4_0", compiledVertexShader );
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

		// Create an instance buffer
		// TODO: Quad shouldn't know about Particle!!
		mInstanceBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( mInstanceData, sizeof( Particle ) * MAX_INSTANCES, EBindflags::EBF_VertexBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );

		// Create index buffer
		uint16_t indexes[NUM_INDICES] = { 0, 3, 1, 1, 3, 2 };
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indexes, sizeof( uint16_t ) * NUM_INDICES, EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );
	}

	void Quad::BindContext()
	{
		GraphicsDriver::Get()->SetInputLayout( mInputLayout );
		GraphicsDriver::Get()->SetVertexShader( mVertexShader );
		GraphicsDriver::Get()->SetVertexBuffers( mVertexBuffer, sizeof( VERTEX_P_N_T ), mInstanceBuffer, sizeof( Particle ) );
		GraphicsDriver::Get()->SetIndexBuffer( mIndexBuffer );
	}

	void Quad::DrawInstanced( uint32_t instanceCount )
	{
		GraphicsDriver::Get()->DrawIndexedInstanced( NUM_INDICES, instanceCount, 0, 0, 0 );
	}

	Vector3* Quad::MapInstanceBuffer()
	{
		return GraphicsDriver::Get()->MapBuffer<Vector3>( mInstanceBuffer );
	}

	void Quad::UnmapInstanceBuffer()
	{
		GraphicsDriver::Get()->UnmapBuffer( mInstanceBuffer );
	}
}

