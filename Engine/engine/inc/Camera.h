namespace ITP485
{
	class __declspec( align( 16 ) ) Camera
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		struct PerCameraConstants
		{
			Matrix4 mProjectionViewMatrix;
			Vector3 mCameraPosition;
		};

		Camera( const Vector3& inPosition, const Quaternion& inRotation, 
				float inFovY, float inAspectRatio, float inNearZ, float inFarZ ) :
			mPosition(inPosition), mRotation( inRotation ),
			mFovY( inFovY ), mAspectRatio( inAspectRatio ), mNearZ( inNearZ ), mFarZ( inFarZ )
		{
			UpdateViewMatrix();
			UpdateProjectionMatrix();
			UpdateProjectionViewMatrix();
		}

		void Camera::SetPosition( const Vector3& inPosition )
		{
			mPosition = inPosition;
			UpdateViewMatrix();
			UpdateProjectionViewMatrix();
		}

		void Camera::SetPosition( float x, float y, float z )
		{
			mPosition = Vector3( x, y, z );
			UpdateViewMatrix();
			UpdateProjectionViewMatrix();
		}

		void Camera::MoveCamera( float offsetX, float offsetY, float offsetZ )
		{
			mPosition = mPosition + Vector3( offsetX, offsetY, offsetZ );
			UpdateViewMatrix();
			UpdateProjectionViewMatrix();
		}

		void Camera::LookAt( float x, float y, float z )
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

		void UpdateConstants() const;

	private:

		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void UpdateProjectionViewMatrix();

		Matrix4 mViewMatrix;
		Matrix4 mProjectionMatrix;
		Matrix4 mProjectionViewMatrix;

		Vector3 mPosition;
		Quaternion mRotation;
		float mFovY;
		float mAspectRatio;
		float mNearZ;
		float mFarZ;


	};
	typedef shared_ptr< Camera > CameraPtr;
}