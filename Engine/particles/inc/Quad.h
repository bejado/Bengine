#pragma once

#include "MeshPrimitive.h"	// TODO: do the integration!

namespace BNG
{

	// Forward declaration
	struct PrimitiveDrawer::Mesh;

	class Quad 
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		Quad( uint32_t instanceDataSize, uint32_t instanceCount );
		void* MapInstanceBuffer();
		void UnmapInstanceBuffer();

		void FillOutMeshStruct( PrimitiveDrawer::Mesh* const mesh );
		void UpdateUniformBuffer( const Matrix4& translation );

	private:

		struct UniformBuffer
		{
			Matrix4 objectToWorld;
		};

		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;
		GraphicsBufferPtr mInstanceBuffer;
		GraphicsBufferPtr mUniformBuffer;

		VertexShaderPtr mVertexShader;
		InputLayoutPtr mInputLayout;

		uint32_t mInstanceDataSize;
		uint32_t mInstanceCount;

	};

	typedef shared_ptr< Quad > QuadPtr;

}