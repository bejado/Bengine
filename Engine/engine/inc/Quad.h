#pragma once

#include "MeshPrimitive.h"	// TODO: do the integration!

namespace ITP485
{

	class Quad 
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		Quad( uint32_t instanceDataSize, uint32_t instanceCount );
		void BindContext();
		void DrawInstanced( uint32_t instanceCount );
		void* MapInstanceBuffer();
		void UnmapInstanceBuffer();

	private:

		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;
		GraphicsBufferPtr mInstanceBuffer;

		VertexShaderPtr mVertexShader;
		InputLayoutPtr mInputLayout;

		uint32_t mInstanceDataSize;
		uint32_t mInstanceCount;

	};

	typedef shared_ptr< Quad > QuadPtr;

}