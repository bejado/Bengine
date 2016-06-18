#include "ObjMesh.h"

#include <PrecompiledHeader.h>
#include <tiny_obj_loader.h>
#include <algorithm>

namespace ITP485
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

	static void LoadMeshFromObjFile( const std::string file, RawMesh& mesh )
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

	GraphicsBufferPtr RawMesh::CreateVertexBuffer()
	{
		std::vector< VERTEX_P_N_T > buffer;
		for ( size_t v = 0; v < positions.size(); v++ )
		{
			buffer.push_back( VERTEX_P_N_T( positions[v], normals[v], textureCoordinates[v] ) );
		}

		return GraphicsDriver::Get()->CreateGraphicsBuffer( buffer.data(),
															sizeof( VERTEX_P_N_T ) * buffer.size(),
															EBindflags::EBF_VertexBuffer,
															0,
															EGraphicsBufferUsage::EGBU_Immutable );
	}

	GraphicsBufferPtr RawMesh::CreateIndexBuffer()
	{
		return GraphicsDriver::Get()->CreateGraphicsBuffer( indices.data(),
															sizeof( uint16_t ) * indices.size(),
															EBindflags::EBF_IndexBuffer,
															0,
															EGraphicsBufferUsage::EGBU_Immutable );
	}

	ObjMeshPrimitive::ObjMeshPrimitive( const std::string file )
	{
		RawMesh mesh;
		LoadMeshFromObjFile( file, mesh );

		// Create vertex and index buffers.
		mIndexBuffer = mesh.CreateIndexBuffer();
		mVertexBuffer = mesh.CreateVertexBuffer();

		mNumIndices = mesh.indices.size();

		// Compile vertex shader
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\shadow.hlsl", "VS", "vs_4_0", compiledVertexShader );
		mVertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

		// Create an input layout
		InputLayoutElement elements[3] {
			{ "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0, 0, EIC_PerVertex, 0 },
			{ "NORMAL", 0, EGFormat::EGF_R32G32B32_Float, 0, sizeof( float ) * 3, EIC_PerVertex, 0 },
			{ "TEXCOORD", 0, EGFormat::EGF_R32G32_Float, 0, sizeof( float ) * 6, EIC_PerVertex, 0 },
		};
		mInputLayout = GraphicsDriver::Get()->CreateInputLayout( elements, 3, compiledVertexShader );

		// Create our object to world constant buffer
		mUniformBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( Matrix4 ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );

		mMaterial = MaterialPtr( new Material( L"Resources\\Shaders\\phong.hlsl", L"" ) );
	}

}
