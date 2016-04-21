#include <PrecompiledHeader.h>

static const unsigned int NUM_VERTICES = 24;
static const unsigned int NUM_INDICES = 36;

namespace ITP485
{

	Cube::Cube()
	{
		// create our vertex buffer
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

		// create our index buffer
		uint16_t indexes[NUM_INDICES] = { 0, 3, 1, 1, 3, 2,			// front
										  5, 7, 4, 6, 7, 5,			// back
										  8, 11, 9, 9, 11, 10,		// left
										  13, 15, 12, 14, 15, 13,	// right
										  16, 17, 18, 18, 19, 16,	// bottom
										  22, 21, 20, 20, 23, 22 };	// top
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indexes, sizeof( uint16_t ) * NUM_INDICES, EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );
	}

	void Cube::Draw()
	{
		// Set buffers
		GraphicsDriver::Get()->SetVertexBuffer( mVertexBuffer, sizeof( VERTEX_P_N_T ) );
		GraphicsDriver::Get()->SetIndexBuffer( mIndexBuffer );

		// Draw!
		GraphicsDriver::Get()->DrawIndexed( NUM_INDICES, 0, 0 );
	}

}

