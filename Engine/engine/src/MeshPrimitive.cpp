#include <PrecompiledHeader.h>

namespace ITP485
{

	MeshPrimitive::MeshPrimitive() : mTranslation( 0.f, 0.f, 0.f ),
									 mScale( 1.f ),
									 mUniformBufferDirty( true )
	{}

	void MeshPrimitive::Draw( const PrimitiveDrawer& drawer ) const
	{
		if ( mUniformBufferDirty )
		{
			UpdateVertexUniformBuffer();
			mUniformBufferDirty = false;
		}

		// Create a primitive mesh and draw it through our drawing interface.
		PrimitiveDrawer::Mesh mesh;
		mesh.indexbuffer = mIndexBuffer;
		mesh.vertexBuffer = mVertexBuffer;
		mesh.indices = mNumIndices;
		mesh.inputLayout = mInputLayout;
		mesh.vertexShader = mVertexShader;
		mesh.material = mMaterial;
		mesh.vertexUniformBuffer = mUniformBuffer;
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

	void MeshPrimitive::UpdateVertexUniformBuffer() const
	{
		Matrix4 scale;
		scale.CreateScale( mScale );

		Matrix4 translation;
		translation.CreateTranslation( mTranslation );

		translation.Multiply( scale );

		Matrix4 *objectToWorld = static_cast<Matrix4*>(GraphicsDriver::Get()->MapBuffer( mUniformBuffer ));
		memcpy( objectToWorld, &translation.GetTranspose(), sizeof( Matrix4 ) );
		GraphicsDriver::Get()->UnmapBuffer( mUniformBuffer );
	}

}
