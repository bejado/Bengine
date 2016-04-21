#include <PrecompiledHeader.h>
#include <tiny_obj_loader.h>
#include <algorithm>

namespace ITP485
{

	struct PackedVector2
	{
		PackedVector2( float x, float y ) : mX( x ), mY( y ) {}
		PackedVector2() : PackedVector2( 0.f, 0.f ) {}
		float mX, mY;
	};

	struct PackedVector3	// used instead of Vector3 because Vector3 is actually 16 bytes
	{
		PackedVector3( float x, float y, float z ) : mX( x ), mY( y ), mZ( z ) {}
		PackedVector3() : PackedVector3( 0.f, 0.f, 0.f ) {}
		float mX, mY, mZ;
	};

	struct VERTEX_P_N_T
	{
		VERTEX_P_N_T( const PackedVector3& position, const PackedVector3& normal, const PackedVector2& texCoord ) : mPosition( position ), mNormal( normal ), mTexCoord( texCoord ) {}
		PackedVector3 mPosition;
		PackedVector3 mNormal;
		PackedVector2 mTexCoord;
	};

	// tinyobj helpers

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


	ObjMesh::ObjMesh( const Vector3& position )
	{
		mPosition = position;

		std::string inputfile = "Meshes\\frigate.obj";
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str());

		// `err` may contain warning message.
		std::cout << err;
		Dbg_Assert( ret, "Error loading obj file." );

		std::vector<VERTEX_P_N_T> vertices;

		for (size_t i = 0; i < shapes.size(); i++) {
		  assert((shapes[i].mesh.indices.size() % 3) == 0);

		  // Loop through the vertices of the mesh
		  for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
			// Grab the position
			PackedVector3 pos1, pos2, pos3;
			pos1 = GetPosition( shapes[i], v );

			// Grab the tex coord
			PackedVector2 tex1, tex2, tex3;
			tex1 = GetTexCoord( shapes[i], v );

			// Grab the normal
			PackedVector3 norm1, norm2, norm3;
			norm1 = GetNormal( shapes[i], v );

			// Create the vertex
			VERTEX_P_N_T vert1 = VERTEX_P_N_T( pos1, norm1, tex1 );

			vertices.push_back( vert1 );
		  }
		}

		// create our vertex buffer
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices.data(), sizeof( VERTEX_P_N_T ) * vertices.size(), EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// create our index buffer - convert each index from unsigned int to uint16_t
		std::vector<unsigned int>& indices = shapes[0].mesh.indices;
		std::vector<uint16_t> indices16(indices.size());
		std::transform( indices.begin(), indices.end(), indices16.begin(), []( const unsigned int val )
			{
				return (uint16_t)val;
			} );
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indices16.data(), sizeof( uint16_t ) * indices16.size(), EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );
		mNumIndexes = indices16.size();
	}

	void ObjMesh::UpdateObjectWorldBuffer( GraphicsBufferPtr buffer )
	{
		Matrix4 *objectToWorld = static_cast<Matrix4*>(GraphicsDriver::Get()->MapBuffer( buffer ));
		// TODO: cache this matrix
		Matrix4 translationMatrix;
		translationMatrix.CreateTranslation( mPosition );
		memcpy( objectToWorld, &translationMatrix.GetTranspose(), sizeof( Matrix4 ) );
		GraphicsDriver::Get()->UnmapBuffer( buffer );
	}

	void ObjMesh::Draw()
	{
		// Set buffers
		GraphicsDriver::Get()->SetVertexBuffer( mVertexBuffer, sizeof( VERTEX_P_N_T ) );
		GraphicsDriver::Get()->SetIndexBuffer( mIndexBuffer );

		// Draw!
		GraphicsDriver::Get()->DrawIndexed( mNumIndexes, 0, 0 );
	}
}

