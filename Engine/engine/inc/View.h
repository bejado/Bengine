#pragma once

namespace ITP485
{
	class __declspec( align( 16 ) ) View
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		/**
		* A vantage point into the game world, e.g., the player's perspective or a light.
		*/
		View( const Vector3& inPosition, const Quaternion& inRotation, 
				float inFovY, float inAspectRatio, float inNearZ, float inFarZ, bool hackOrtho ) :
			mPosition(inPosition), mRotation( inRotation ),
			mFovY( inFovY ), mAspectRatio( inAspectRatio ), mNearZ( inNearZ ), mFarZ( inFarZ ), mHackOrtho( hackOrtho )
		{
			UpdateViewMatrix();
			UpdateProjectionMatrix();
			UpdateProjectionViewMatrix();
		}

		void View::SetPosition( const Vector3& inPosition )
		{
			mPosition = inPosition;
			UpdateViewMatrix();
			UpdateProjectionViewMatrix();
		}

		void View::SetPosition( float x, float y, float z )
		{
			mPosition = Vector3( x, y, z );
			UpdateViewMatrix();
			UpdateProjectionViewMatrix();
		}

		const Vector3& View::GetPosition() const
		{
			return mPosition;
		}

		const Vector3& View::GetUpVector() const
		{
			return mViewUp;
		}

		void View::MoveCamera( const Vector3& cameraSpaceDirection, float amount )
		{
			Vector3 vector = cameraSpaceDirection;
			vector.Rotate( mRotation );
			mPosition = mPosition + vector * amount;
			UpdateViewMatrix();
			UpdateProjectionViewMatrix();
		}

		void View::RotateCameraFixedAxis( const Vector3& axis, float amount )
		{
			mRotation.Multiply( Quaternion( axis, amount ) );
			UpdateViewMatrix();
			UpdateProjectionViewMatrix();
		}

		void View::RotateCameraRelativeAxis( const Vector3& axis, float amount )
		{
			Vector3 rotationAxis = axis;
			rotationAxis.Rotate( mRotation );
			mRotation.Multiply( Quaternion( rotationAxis, amount ) );
			UpdateViewMatrix();
			UpdateProjectionViewMatrix();
		}

		void View::LookAt( float x, float y, float z )
		{
			mViewMatrix.CreateLookAt( mPosition, Vector3(x, y, z), Vector3::Up );	
			UpdateProjectionViewMatrix();
		}

		void SetPose( const Vector3& inPosition, const Quaternion& inRotation )
		{
			mPosition = inPosition;
			mRotation = inRotation;
			UpdateViewMatrix();
			UpdateProjectionViewMatrix();
		}

		void SetProjection( float inFovY, float inAspectRatio, float inNearZ, float inFarZ )
		{
			mFovY = inFovY;
			mAspectRatio = inAspectRatio;
			mNearZ = inNearZ;
			mFarZ = inFarZ;
			UpdateProjectionMatrix();
			UpdateProjectionViewMatrix();
		}

		const Matrix4& GetProjectionViewMatrix() const { return mProjectionViewMatrix; }

	private:

		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void UpdateProjectionViewMatrix();

		Matrix4 mViewMatrix;
		Matrix4 mProjectionMatrix;
		Matrix4 mProjectionViewMatrix;
		Vector3 mViewUp;

		Vector3 mPosition;
		Quaternion mRotation;
		float mFovY;
		float mAspectRatio;
		float mNearZ;
		float mFarZ;

		bool mHackOrtho;

	};

	typedef shared_ptr< View > ViewPtr;
}
