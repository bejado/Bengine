#include "GameObject.h"
#include <PrecompiledHeader.h>
#include "DebugDrawing.h"

namespace ITP485
{

	GameObject::GameObject( MeshPrimitivePtr renderPrimitive ) : mRenderPrimitive( renderPrimitive ),
																 mTranslation( 0.f, 0.f, 0.f ),
  																 mRotation( Quaternion::Identity ),
  																 mScale( 1.f ),
   																 mObjectToWorldMatrixDirty( true ),
																 mBoundsRadius( 1.f )
	{
		mBoundsDebugSphere = CreateDebugSphere( mBoundsRadius );
	}

	void GameObject::Attach() const
	{
		Renderer::Get().AddPrimitive( mRenderPrimitive );
	}

	void GameObject::DrawDebugBounds( bool visible )
	{
		if ( visible )
		{
			Renderer::Get().AddPrimitive( mBoundsDebugSphere );
		}
	}

	void GameObject::Update()
	{
		if ( mObjectToWorldMatrixDirty )
		{
			UpdateObjectToWorldMatrix();
			UpdateBoundsDebugSphere();
			mObjectToWorldMatrixDirty = false;
			mRenderPrimitive->SetObjectToWorldMatrix( mObjectToWorldMatrix );
		}
	}

	void GameObject::SetTranslation( const Vector3& translation )
	{
		mTranslation = translation;
		mObjectToWorldMatrixDirty = true;
	}

	void GameObject::SetScale( float scale )
	{
		mScale = scale;
		mObjectToWorldMatrixDirty = true;
	}

	void GameObject::SetRotation( const Quaternion& rotation )
	{
		mRotation = rotation;
		mObjectToWorldMatrixDirty = true;
	}

	void GameObject::SetBounds( float bounds )
	{
		mBoundsRadius = bounds;
		UpdateBoundsDebugSphere();
	}

	void GameObject::UpdateBoundsDebugSphere()
	{
		Matrix4 model;
		model.CreateTranslation( mTranslation );

		Matrix4 rotation;
		rotation.CreateFromQuaternion( mRotation );
		model.Multiply( rotation );

		Matrix4 scale;
		scale.CreateScale( mBoundsRadius );
		model.Multiply( scale );

		mBoundsDebugSphere->SetObjectToWorldMatrix( model );
	}

	void GameObject::UpdateObjectToWorldMatrix()
	{
		Matrix4 model;
		model.CreateTranslation( mTranslation );

		Matrix4 rotation;
		rotation.CreateFromQuaternion( mRotation );
		model.Multiply( rotation );

		Matrix4 scale;
		scale.CreateScale( mScale );
		model.Multiply( scale );

		mObjectToWorldMatrix = model;
	}

}
