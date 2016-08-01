#include "ObjMesh.h"

#include "RawMesh.h"
#include <PrecompiledHeader.h>
#include <tiny_obj_loader.h>
#include <algorithm>

namespace BNG
{

	PackedVector3 GetPosition( const tinyobj::shape_t& shape, unsigned int idx )
	{
		float x, y, z;
		x = shape.mesh.positions[3 * idx + 0];
		y = shape.mesh.positions[3 * idx + 1];
		z = shape.mesh.positions[3 * idx + 2];
		return PackedVector3( x, y, z );
	}

	PackedVector2 GetTexCoord( const tinyobj::shape_t& shape, unsigned int idx )
	{
		float u, v;
		u = shape.mesh.texcoords[2 * idx + 0];
		v = shape.mesh.texcoords[2 * idx + 1];
		return PackedVector2( u, v );
	}

	PackedVector3 GetNormal( const tinyobj::shape_t& shape, unsigned int idx )
	{
		float x, y, z;
		x = shape.mesh.normals[3 * idx + 0];
		y = shape.mesh.normals[3 * idx + 1];
		z = shape.mesh.normals[3 * idx + 2];
		return PackedVector3( x, y, z );
	}

	void LoadMeshFromObjFile( const std::string file, RawMesh& mesh )
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(shapes, materials, err, file.c_str());

		// `err` may contain warning message.
		std::cout << err;
		Dbg_Assert( ret, "Error loading obj file." );

		// TODO: support more than one shape
		for (size_t i = 0; i < shapes.size(); i++) {
		  assert((shapes[i].mesh.indices.size() % 3) == 0);

		  // Loop through the vertices of the mesh
		  for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
			PackedVector3 position = GetPosition( shapes[i], v );	// TODO: support meshes without normals or texture coordinates
			PackedVector2 texture = GetTexCoord( shapes[i], v );
			PackedVector3 normal = GetNormal( shapes[i], v );

			mesh.positions.push_back( position );
			mesh.textureCoordinates.push_back( texture );
			mesh.normals.push_back( normal );
		  }
		}

		// Load index buffer - convert each index from unsigned int to uint16_t.
		std::vector<unsigned int>& indices = shapes[0].mesh.indices;	// TODO: support more than one shape
		mesh.indices.resize( indices.size() );
		std::transform( indices.begin(), indices.end(), mesh.indices.begin(), []( const unsigned int val )
			{
				return (uint16_t)val;
			} );
	}

	// Algorithm from Lengyel's Mathematics for 3D Game Programming and Computer Graphics, page 184.
	void CalculateTangents( RawMesh& mesh )
	{
		const size_t numVertices = mesh.positions.size();
		const size_t numTriangles = mesh.indices.size() / 3;

		mesh.tangents.clear();
		mesh.tangents.resize( numVertices );
		std::vector< PackedVector3 > tan( numVertices );
		std::vector< PackedVector3 > bitan( numVertices );

		for ( size_t t = 0; t < numTriangles; t++ )
		{
			uint16_t indexA, indexB, indexC;
			indexA = mesh.indices[t * 3 + 0];
			indexB = mesh.indices[t * 3 + 1];
			indexC = mesh.indices[t * 3 + 2];

			PackedVector3 positionA, positionB, positionC;
			positionA = mesh.positions[indexA];
			positionB = mesh.positions[indexB];
			positionC = mesh.positions[indexC];

			PackedVector2 textureA, textureB, textureC;
			textureA = mesh.textureCoordinates[indexA];
			textureB = mesh.textureCoordinates[indexB];
			textureC = mesh.textureCoordinates[indexC];

			// Calculate position deltas.
			float x1 = positionB.mX - positionA.mX;
			float x2 = positionC.mX - positionA.mX;
			float y1 = positionB.mY - positionA.mY;
			float y2 = positionC.mY - positionA.mY;
			float z1 = positionB.mZ - positionA.mZ;
			float z2 = positionC.mZ - positionA.mZ;

			// Calculate texture coordinate deltas.
			float s1 = textureB.mX - textureA.mX;
			float s2 = textureC.mX - textureA.mX;
			float t1 = textureB.mY - textureA.mY;
			float t2 = textureC.mY - textureA.mY;

			float r = 1.f / ( s1 * t2 - s2 * t1 );
			PackedVector3 sdir;
			sdir.mX = (t2 * x1 - t1 * x2) * r;
			sdir.mY = (t2 * y1 - t1 * y2) * r;
			sdir.mZ = (t2 * z1 - t1 * z2) * r;
			PackedVector3 tdir;
			tdir.mX = (s1 * x2 - s2 * x1) * r;
			tdir.mY = (s1 * y2 - s2 * y1) * r;
			tdir.mZ = (s1 * z2 - s2 * z1) * r;

			tan[indexA] += sdir;
			tan[indexB] += sdir;
			tan[indexC] += sdir;
			bitan[indexA] += tdir;
			bitan[indexB] += tdir;
			bitan[indexC] += tdir;
		}

		for ( size_t i = 0; i < numVertices; i++ )
		{
			const Vector3 normal = Vector3( mesh.normals[i].mX, mesh.normals[i].mY, mesh.normals[i].mZ );
			const Vector3 tangent = Vector3( tan[i].mX, tan[i].mY, tan[i].mZ );
			const Vector3 bitangent = Vector3( bitan[i].mX, bitan[i].mY, bitan[i].mZ );
			Vector3 tangentOut;

			// Gram-Schmidt orthogonalize.
			tangentOut = (tangent - normal * normal.Dot( tangent ));
			tangentOut.Normalize();

			// Calculate handedness.
			float handedness = Cross( normal, tangent ).Dot( bitangent ) < 0.f ? 1.f : -1.f;

			mesh.tangents[i] = PackedVector4( tangentOut.GetX(), tangentOut.GetY(), tangentOut.GetZ(), handedness );
		}
	}

}
