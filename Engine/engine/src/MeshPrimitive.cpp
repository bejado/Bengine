#include "MeshPrimitive.h"

#include <PrecompiledHeader.h>

namespace BNG
{

	MeshPrimitive::MeshPrimitive( const RawMeshPtr rawMesh, const MaterialPtr material, const VertexSource& vertexSource ) : mObjectToWorldMatrix( Matrix4::Identity ),
																															 mUniformBuffer( nullptr ),
																															 mRawMesh( rawMesh ),
																															 mMaterial( material ),
																															 mVertexSource( vertexSource )
	{
		// Create our object to world constant buffer.
		mUniformBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( ObjectConstants ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
		UpdateConstantBuffer();

		// Create vertex and index buffers.
		mIndexBuffer = mRawMesh->CreateIndexBuffer();
		mVertexBuffer = mVertexSource.positionOnly ? mRawMesh->CreatePositionIndexOnlyVertexBuffer() : mRawMesh->CreateVertexBuffer();
		mNumIndices = mRawMesh->indices.size();
	}

	void MeshPrimitive::Draw( const PrimitiveDrawer& drawer, const ViewPtr view ) const
	{
		// Create a primitive mesh and draw it through our drawing interface.
		PrimitiveDrawer::Mesh mesh;
		mesh.vertexBuffer = mVertexBuffer;
		mesh.vertexUniformBuffer = mUniformBuffer;
		mesh.inputLayout = mVertexSource.inputLayout;
		mesh.vertexStride = mVertexSource.vertexStride;
		mesh.indexBuffer = mIndexBuffer;
		mesh.indices = mNumIndices;
		mesh.vertexShader = mVertexSource.vertexShader;
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
