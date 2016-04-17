#include <PrecompiledHeader.h>

namespace ITP485
{
	struct PackedVector3	// used instead of Vector3 because Vector3 is actually 16 bytes
	{
		PackedVector3( float x, float y, float z ) : mX( x ), mY( y ), mZ( z ) {}
		float mX, mY, mZ;
	};

	struct VERTEX_P_N
	{
		VERTEX_P_N( const PackedVector3& position, const PackedVector3& normal ) : mPosition( position ), mNormal( normal ) {}
		PackedVector3 mPosition;
		PackedVector3 mNormal;
	};

	Cube::Cube( const Vector3& position )
	{
		mPosition = position;

		// create our vertex buffer
		VERTEX_P_N vertices[8] = {
				{ PackedVector3( 0.f, 0.f, 0.f ), PackedVector3( 0.f, 1.f, 0.f ) },
				{ PackedVector3( 1.f, 0.f, 0.f ), PackedVector3( 0.f, 1.f, 0.f ) },
				{ PackedVector3( 1.f, 1.f, 0.f ), PackedVector3( 0.f, 1.f, 0.f ) },
				{ PackedVector3( 0.f, 1.f, 0.f ), PackedVector3( 0.f, 1.f, 0.f ) },
				{ PackedVector3( 0.f, 0.f, 1.f ), PackedVector3( 0.f, 1.f, 0.f ) },
				{ PackedVector3( 1.f, 0.f, 1.f ), PackedVector3( 0.f, 1.f, 0.f ) },
				{ PackedVector3( 1.f, 1.f, 1.f ), PackedVector3( 0.f, 1.f, 0.f ) },
				{ PackedVector3( 0.f, 1.f, 1.f ), PackedVector3( 0.f, 1.f, 0.f ) },
		};
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices, sizeof( VERTEX_P_N ) * 8, EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// create our index buffer
		uint16_t indexes[36] = { 0, 3, 1, 1, 3, 2,		// front
								 0, 4, 3, 4, 7, 3,
								 1, 2, 5, 2, 6, 5,
								 5, 6, 7, 7, 4, 5,
								 3, 7, 6, 6, 2, 3,
								 0, 1, 5, 5, 4, 0 };
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

