#include "Primitives.h"
#include "RawMesh.h"

#include <PrecompiledHeader.h>

namespace BNG
{

	void GenerateQuadVerticies( RawMesh& mesh, float width, float height )
	{
		// positions
		mesh.positions.push_back( PackedVector3( -width, height, 0.f ) );
		mesh.positions.push_back( PackedVector3( width, height, 0.f ) );
		mesh.positions.push_back( PackedVector3( width, -height, 0.f ) );
		mesh.positions.push_back( PackedVector3( -width, -height, 0.f ) );

		// normals
		mesh.normals.push_back( PackedVector3( 0.f, 0.f, -1.f ) );
		mesh.normals.push_back( PackedVector3( 0.f, 0.f, -1.f ) );
		mesh.normals.push_back( PackedVector3( 0.f, 0.f, -1.f ) );
		mesh.normals.push_back( PackedVector3( 0.f, 0.f, -1.f ) );

		// textures
		mesh.textureCoordinates.push_back( PackedVector2( 0.f, 0.f ) );
		mesh.textureCoordinates.push_back( PackedVector2( 1.f, 0.f ) );
		mesh.textureCoordinates.push_back( PackedVector2( 1.f, 1.f ) );
		mesh.textureCoordinates.push_back( PackedVector2( 0.f, 1.f ) );

		// tangents
		mesh.tangents.push_back( PackedVector4( 1.f, 0.f, 0.f, -1.f ) );
		mesh.tangents.push_back( PackedVector4( 1.f, 0.f, 0.f, -1.f ) );
		mesh.tangents.push_back( PackedVector4( 1.f, 0.f, 0.f, -1.f ) );
		mesh.tangents.push_back( PackedVector4( 1.f, 0.f, 0.f, -1.f ) );

		// indicies
		mesh.indices.push_back( 0 );
		mesh.indices.push_back( 1 );
		mesh.indices.push_back( 2 );
		mesh.indices.push_back( 2 );
		mesh.indices.push_back( 3 );
		mesh.indices.push_back( 0 );
	}

}
