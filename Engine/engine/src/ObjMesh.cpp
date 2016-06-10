#include "ObjMesh.h"

#include <PrecompiledHeader.h>
#include <tiny_obj_loader.h>
#include <algorithm>

namespace ITP485
{

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

	////

	ObjMeshPrimitive::ObjMeshPrimitive( std::string file )
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(shapes, materials, err, file.c_str());

		// `err` may contain warning message.
		std::cout << err;
		Dbg_Assert( ret, "Error loading obj file." );

		std::vector<VERTEX_P_N_T> vertices;

		// TODO: support more than one shape
		for (size_t i = 0; i < shapes.size(); i++) {
		  assert((shapes[i].mesh.indices.size() % 3) == 0);

		  // Loop through the vertices of the mesh
		  for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
			PackedVector3 pos1 = GetPosition( shapes[i], v );	// TODO: support meshes without normals or texture coordinates
			PackedVector2 tex1 = GetTexCoord( shapes[i], v );
			PackedVector3 norm1 = GetNormal( shapes[i], v );

			vertices.push_back( VERTEX_P_N_T( pos1, norm1, tex1 ) );
		  }
		}

		// create our vertex buffer
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices.data(), sizeof( VERTEX_P_N_T ) * vertices.size(), EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// create our index buffer - convert each index from unsigned int to uint16_t
		std::vector<unsigned int>& indices = shapes[0].mesh.indices;	// TODO: support more than one shape
		std::vector<uint16_t> indices16(indices.size());
		std::transform( indices.begin(), indices.end(), indices16.begin(), []( const unsigned int val )
			{
				return (uint16_t)val;
			} );
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( indices16.data(), sizeof( uint16_t ) * indices16.size(), EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );
		mNumIndices = indices16.size();

		// Compile vertex shader
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\shadow.hlsl", "VS", "vs_4_0", compiledVertexShader );
		mVertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

		// Create an input layout
		InputLayoutElement elements[3] {
			/* from vertex buffer */
			{ "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0, 0, EIC_PerVertex, 0 },
			{ "NORMAL", 0, EGFormat::EGF_R32G32B32_Float, 0, sizeof( float ) * 3, EIC_PerVertex, 0 },
			{ "TEXCOORD", 0, EGFormat::EGF_R32G32_Float, 0, sizeof( float ) * 6, EIC_PerVertex, 0 },
		};
		mInputLayout = GraphicsDriver::Get()->CreateInputLayout( elements, 3, compiledVertexShader );

		// Create our object to world constant buffer
		mUniformBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( Matrix4 ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );

		mMaterial = MaterialPtr( new Material( L"Resources\\Shaders\\shadow.hlsl", L"Resources\\Textures\\bench_color.dds" ) );
	}

}
