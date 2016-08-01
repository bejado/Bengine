#pragma once

#include <PrecompiledHeader.h>

namespace ITP485
{

	// We use these PackedVectors because we don't want byte-alignment
	// TODO: candidate for moving these structs into a "VertexFactory" type class

	struct PackedVector2
	{
		PackedVector2( float x, float y ) : mX( x ), mY( y ) {}
		PackedVector2() : PackedVector2( 0.f, 0.f ) {}
		float mX, mY;
	};

	struct PackedVector3
	{
		PackedVector3( float x, float y, float z ) : mX( x ), mY( y ), mZ( z ) {}
		PackedVector3() : PackedVector3( 0.f, 0.f, 0.f ) {}
		float mX, mY, mZ;

		__forceinline PackedVector3& operator+=( const PackedVector3& in )
		{
			this->mX += in.mX;
			this->mY += in.mY;
			this->mZ += in.mZ;
			return *this;
		}
	};

	struct PackedVector4
	{
		PackedVector4( float x, float y, float z, float w ) : mX( x ), mY( y ), mZ( z ), mW( w ) {}
		PackedVector4() : PackedVector4( 0.f, 0.f, 0.f, 0.f ) {}
		float mX, mY, mZ, mW;
	};

	struct VERTEX_P
	{
		VERTEX_P( const PackedVector3& position ) : mPosition( position ) {}
		PackedVector3 mPosition;
	};

	struct VERTEX_P_T
	{
		VERTEX_P_T( const PackedVector3& position, const PackedVector2& texCoord ) : mPosition( position ), mTexCoord( texCoord ) {}
		PackedVector3 mPosition;
		PackedVector2 mTexCoord;
	};

	struct VERTEX_P_N_T
	{
		VERTEX_P_N_T( const PackedVector3& position, const PackedVector3& normal, const PackedVector2& texCoord ) : mPosition( position ), mNormal( normal ), mTexCoord( texCoord ) {}
		PackedVector3 mPosition;
		PackedVector3 mNormal;
		PackedVector2 mTexCoord;
	};

	// Position, normal, texture, tangent
	struct VERTEX_P_N_T_T
	{
		VERTEX_P_N_T_T( const PackedVector3& position, const PackedVector3& normal, const PackedVector2& texCoord, const PackedVector4 tangent ) : mPosition( position ), mNormal( normal ), mTexCoord( texCoord ), mTangent( tangent ) {}
		PackedVector3 mPosition;
		PackedVector3 mNormal;
		PackedVector2 mTexCoord;
		PackedVector4 mTangent;
	};

	struct RawMesh
	{
		std::vector<uint16_t> indices;
		std::vector<PackedVector3> positions;
		std::vector<PackedVector2> textureCoordinates;
		std::vector<PackedVector3> normals;
		std::vector<PackedVector4> tangents;

		GraphicsBufferPtr CreateVertexBuffer();
		GraphicsBufferPtr CreatePositionIndexOnlyVertexBuffer();
		GraphicsBufferPtr CreateIndexBuffer();
	};

	typedef shared_ptr< RawMesh > RawMeshPtr;

}