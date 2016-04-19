#include <PrecompiledHeader.h>

namespace ITP485
{
	struct PackedVector2
	{
		PackedVector2( float x, float y ) : mX( x ), mY( y ) {}
		float mX, mY;
	};

	struct PackedVector3	// used instead of Vector3 because Vector3 is actually 16 bytes
	{
		PackedVector3( float x, float y, float z ) : mX( x ), mY( y ), mZ( z ) {}
		float mX, mY, mZ;
	};

	struct VERTEX_P_N
	{
		VERTEX_P_N( const PackedVector3& position, const PackedVector3& normal, const PackedVector2& texCoord ) : mPosition( position ), mNormal( normal ), mTexCoord( texCoord ) {}
		PackedVector3 mPosition;
		PackedVector3 mNormal;
		PackedVector2 mTexCoord;
	};

	Cube::Cube( const Vector3& position )
	{
		mPosition = position;

		// create our vertex buffer
		VERTEX_P_N vertices[24] = {
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
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices, sizeof( VERTEX_P_N ) * 24, EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// create our index buffer
		uint16_t indexes[36] = { 0, 3, 1, 1, 3, 2,			// front
								 5, 7, 4, 6, 7, 5,			// back
								 8, 11, 9, 9, 11, 10,		// left
								 13, 15, 12, 14, 15, 13,	// right
								 16, 17, 18, 18, 19, 16,	// bottom
								 22, 21, 20, 20, 23, 22 };	// top
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indexes, sizeof( uint16_t ) * 36, EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );
	}

	void Cube::UpdateObjectWorldBuffer( GraphicsBufferPtr buffer )
	{
		Matrix4 *objectToWorld = static_cast<Matrix4*>(GraphicsDriver::Get()->MapBuffer(buffer));
		// TODO: cache this matrix
		Matrix4 translationMatrix;
		translationMatrix.CreateTranslation( mPosition );
		memcpy( objectToWorld, &translationMatrix.GetTranspose(), sizeof( Matrix4 ) );
		GraphicsDriver::Get()->UnmapBuffer( buffer );
	}

	void Cube::Draw()
	{
		// Set buffers
		GraphicsDriver::Get()->SetVertexBuffer( mVertexBuffer, sizeof( VERTEX_P_N ) );
		GraphicsDriver::Get()->SetIndexBuffer( mIndexBuffer );

		// Draw!
		GraphicsDriver::Get()->DrawIndexed( 36, 0, 0 );
	}
}

