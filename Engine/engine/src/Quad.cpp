#include <PrecompiledHeader.h>

static const unsigned int NUM_VERTICES = 4;
static const unsigned int NUM_INDICES = 6;

namespace ITP485
{

	Quad::Quad()
	{
		// create our vertex buffer
		VERTEX_P_N_T vertices[NUM_VERTICES] = {
				{ PackedVector3( -.5f, -.5f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 0.f, 1.f) },
				{ PackedVector3( .5f, -.5f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 1.f, 1.f) },
				{ PackedVector3( .5f, .5f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 1.f, 0.f) },
				{ PackedVector3( -.5f, .5f, 0.f ), PackedVector3( 0.f, 0.f, -1.f ), PackedVector2( 0.f, 0.f) }
		};
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices, sizeof( VERTEX_P_N_T ) * NUM_VERTICES, EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// create our index buffer
		uint16_t indexes[NUM_INDICES] = { 0, 3, 1, 1, 3, 2 };
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indexes, sizeof( uint16_t ) * NUM_INDICES, EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );
	}

	void Quad::BindBuffers()
	{
		GraphicsDriver::Get()->SetVertexBuffer( mVertexBuffer, sizeof( VERTEX_P_N_T ) );
		GraphicsDriver::Get()->SetIndexBuffer( mIndexBuffer );
	}

	void Quad::Draw()
	{
		GraphicsDriver::Get()->DrawIndexed( NUM_INDICES, 0, 0 );
	}

}

