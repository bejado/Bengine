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
																 mBoundsRadius( 1.f ),
															     mVelocity( 0.f, 0.f, 0.f ),
																 mAcceleration( 0.f, 0.f, 0.f ),
																 mFrictionFactor( 0.f ),
																 mObjectOffset( Matrix4::Identity )
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
		UpdateForPhysics();
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

	void GameObject::SetOffset( const Matrix4& offset )
	{
		mObjectOffset = offset;
		mObjectToWorldMatrixDirty = true;
	}

	void GameObject::SetScale( float scale )
	{
		mScale = scale;
		mObjectToWorldMatrixDirty = true;
	}

	void GameObject::Rotate( const Quaternion& rotation )
	{
		mRotation.Multiply( rotation );
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

		model.Multiply( mObjectOffset );

		mObjectToWorldMatrix = model;
	}

	void GameObject::UpdateForPhysics()
	{
		float deltaTime = Timing::Get().GetDeltaTime();
		mVelocity = mVelocity + ( mAcceleration - mVelocity * mFrictionFactor ) * deltaTime;
		mTranslation = mTranslation + mVelocity * deltaTime;
		mObjectToWorldMatrixDirty = true;	// TODO: this variable is useless, since this is called every update
	}

}
