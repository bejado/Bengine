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
		void SetScale( float scale );
		void SetRotation( const Quaternion& rotation );

		void Attach() const;

	private:

		MeshPrimitivePtr mRenderPrimitive;
		float bounds;

		void UpdateObjectToWorldMatrix();
		mutable bool mObjectToWorldMatrixDirty;
		Vector3 mTranslation;
		float mScale;	// we only support uniform scale, for now
		Quaternion mRotation;
		Matrix4 mObjectToWorldMatrix;

	};

	typedef shared_ptr< GameObject > GameObjectPtr;

}