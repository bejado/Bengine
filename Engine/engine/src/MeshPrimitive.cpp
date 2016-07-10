#include "MeshPrimitive.h"

#include <PrecompiledHeader.h>

namespace ITP485
{

	MeshPrimitive::MeshPrimitive() : mObjectToWorldMatrix( Matrix4::Identity ),
									 mUniformBuffer( nullptr ),
									 mVertexStride( sizeof( VERTEX_P_N_T ) )
	{
		// Create our object to world constant buffer
		mUniformBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( ObjectConstants ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
		UpdateConstantBuffer();
	}

	void MeshPrimitive::Draw( const PrimitiveDrawer& drawer, const ViewPtr view ) const
	{
		// Create a primitive mesh and draw it through our drawing interface.
		PrimitiveDrawer::Mesh mesh;
		mesh.vertexBuffer = mVertexBuffer;
		mesh.vertexUniformBuffer = mUniformBuffer;
		mesh.inputLayout = mInputLayout;
		mesh.vertexStride = mVertexStride;
		mesh.indexBuffer = mIndexBuffer;
		mesh.indices = mNumIndices;
		mesh.vertexShader = mVertexShader;
		mesh.material = mMaterial;	// TODO: we're assuming this is set in subclass' constructor
		drawer.DrawMesh( mesh );
	}

	void MeshPrimitive::SetObjectToWorldMatrix( const Matrix4& matrix )
	{
		mObjectToWorldMatrix = matrix;
		UpdateConstantBuffer();
	}

	void MeshPrimitive::UpdateConstantBuffer()
	{
		ObjectConstants* objectConstants = static_cast<ObjectConstants*>(GraphicsDriver::Get()->MapBuffer( mUniformBuffer ));
		objectConstants->objectToWorld = mObjectToWorldMatrix.GetTranspose();
		Matrix4 invertedObjectToWorld = mObjectToWorldMatrix;
		mObjectToWorldMatrix.Invert();
		objectConstants->worldToObject = mObjectToWorldMatrix.GetTranspose();
		GraphicsDriver::Get()->UnmapBuffer( mUniformBuffer );
	}

}
