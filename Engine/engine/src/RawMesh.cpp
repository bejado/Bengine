#include "RawMesh.h"

namespace BNG
{

	GraphicsBufferPtr RawMesh::CreateVertexBuffer()
	{
		std::vector< VERTEX_P_N_T_T > buffer;
		for ( size_t v = 0; v < positions.size(); v++ )
		{
			buffer.push_back( VERTEX_P_N_T_T( positions[v], normals[v], textureCoordinates[v], tangents[v] ) );
		}

		return GraphicsDriver::Get()->CreateGraphicsBuffer( buffer.data(),
															sizeof( VERTEX_P_N_T_T ) * buffer.size(),
															EBindflags::EBF_VertexBuffer,
															0,
															EGraphicsBufferUsage::EGBU_Immutable );
	}

	// TODO: we can probably make this more generic
	GraphicsBufferPtr RawMesh::CreatePositionIndexOnlyVertexBuffer()
	{
		std::vector< VERTEX_P > buffer;
		for ( size_t v = 0; v < positions.size(); v++ )
		{
			buffer.push_back( VERTEX_P( positions[v] ) );
		}

		return GraphicsDriver::Get()->CreateGraphicsBuffer( buffer.data(),
															sizeof( VERTEX_P ) * buffer.size(),
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
}
