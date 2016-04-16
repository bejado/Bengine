#include <PrecompiledHeader.h>

namespace ITP485
{
	Cube::Cube( const Vector3& position )
	{
		mPosition = position;

		// create our vertex buffer
		Vector3 vertices[8];
		vertices[0].Set( 0.f, 0.f, 0.f );
		vertices[1].Set( 1.f, 0.f, 0.f );
		vertices[2].Set( 1.f, 1.f, 0.f );
		vertices[3].Set( 0.f, 1.f, 0.f );
		vertices[4].Set( 0.f, 0.f, 1.f );
		vertices[5].Set( 1.f, 0.f, 1.f );
		vertices[6].Set( 1.f, 1.f, 1.f );
		vertices[7].Set( 0.f, 1.f, 1.f );
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices, sizeof( Vector3 ) * 8, EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// create our index buffer
		uint16_t indexes[36] = { 0, 3, 1, 1, 3, 2,		// front
		0, 4, 3, 4, 7, 3,
		1, 2, 5, 2, 6, 5,
		5, 6, 7, 7, 4, 5,
		3, 7, 6, 6, 2, 3,
		0, 1, 5, 5, 4, 0 };
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indexes, sizeof( uint16_t ) * 36, EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );
	}

	void Cube::UpdateObjectWorldBuffer( const GraphicsBufferPtr const buffer )
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
		GraphicsDriver::Get()->SetVertexBuffer( mVertexBuffer, sizeof( Vector3 ) );
		GraphicsDriver::Get()->SetIndexBuffer( mIndexBuffer );

		// Draw!
		GraphicsDriver::Get()->DrawIndexed( 36, 0, 0 );
	}
}

