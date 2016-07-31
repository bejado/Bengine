#pragma once

#include "MeshPrimitive.h"
#include <PrecompiledHeader.h>

namespace ITP485
{

	class GameObject
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		GameObject( MeshPrimitivePtr renderPrimitive );
		virtual void Update();
		void SetTranslation( const Vector3& translation );
		const Vector3& GetTranslation() const { return mTranslation; }
		void SetOffset( const Matrix4& offset );
		void SetScale( float scale );
		void Rotate( const Quaternion& rotation );
		void SetRotation( const Quaternion& rotation );
		const Quaternion& GetRotation() const { return mRotation; }
		void SetBounds( float bounds );
		void Attach() const;

		void DrawDebugBounds( bool visible );

	private:

		void UpdateBoundsDebugSphere();

		MeshPrimitivePtr mRenderPrimitive;
		MeshPrimitivePtr mBoundsDebugSphere;
		float mBoundsRadius;

		void UpdateObjectToWorldMatrix();
		mutable bool mObjectToWorldMatrixDirty;
		Vector3 mTranslation;
		float mScale;	// we only support uniform scale, for now
		Quaternion mRotation;
		Matrix4 mObjectToWorldMatrix;
		Matrix4 mObjectOffset;

	// -------
	// PHYSICS
	// -------
	public:

		void SetAcceleration( const Vector3& acceleration ) { mAcceleration = acceleration; }
		void SetFrictionFactor( const float frictionFactor ) { mFrictionFactor = frictionFactor; }
		void UpdateForPhysics();

	private:

		Vector3 mVelocity;
		Vector3 mAcceleration;
		float mFrictionFactor;

	};

	typedef shared_ptr< GameObject > GameObjectPtr;

}