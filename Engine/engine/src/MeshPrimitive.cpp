#include "MeshPrimitive.h"

#include <PrecompiledHeader.h>

namespace ITP485
{

	MeshPrimitive::MeshPrimitive() : mTranslation( 0.f, 0.f, 0.f ),
									 mScale( 1.f ),
									 mRotation( Quaternion::Identity ),
									 mUniformBuffer( nullptr ),
									 mUniformBufferDirty( true ),
									 mVertexStride( sizeof( VERTEX_P_N_T ) )
	{
		// Create our object to world constant buffer
		mUniformBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( ObjectConstants ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
	}

	void MeshPrimitive::Draw( const PrimitiveDrawer& drawer, const ViewPtr view ) const
	{
		if ( mUniformBufferDirty && mUniformBuffer )
		{
			UpdateVertexUniformBuffer();
			mUniformBufferDirty = false;
		}

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

	void MeshPrimitive::SetTranslation( const Vector3& translation )
	{
		mTranslation = translation;
		mUniformBufferDirty = true;
	}

	void MeshPrimitive::SetScale( float scale )
	{
		mScale = scale;
		mUniformBufferDirty = true;
	}

	void MeshPrimitive::SetRotation( const Quaternion& rotation )
	{
		mRotation = rotation;
		mUniformBufferDirty = true;
	}

	void MeshPrimitive::UpdateVertexUniformBuffer() const
	{
		Matrix4 model;
		model.CreateTranslation( mTranslation );

		Matrix4 rotation;
		rotation.CreateFromQuaternion( mRotation );
		model.Multiply( rotation );

		Matrix4 scale;
		scale.CreateScale( mScale );
		model.Multiply( scale );

		ObjectConstants* objectConstants = static_cast<ObjectConstants*>(GraphicsDriver::Get()->MapBuffer( mUniformBuffer ));
		objectConstants->objectToWorld = model.GetTranspose();
		model.Invert();
		objectConstants->worldToObject = model.GetTranspose();
		GraphicsDriver::Get()->UnmapBuffer( mUniformBuffer );
	}

}
