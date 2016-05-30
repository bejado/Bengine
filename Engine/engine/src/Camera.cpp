#include <PrecompiledHeader.h>

namespace ITP485
{
	void Camera::UpdateViewMatrix()
	{
		//the camera's position is mPosition
		//take a forward vector and rotate it by the rotation and that's the look direction
		//what is going to be "forward" in our world?

		Vector3 cameraForward = Vector3::Forward;
		cameraForward.Rotate( mRotation );

		Vector3 cameraUp = Vector3::Up;
		cameraUp.Rotate( mRotation );

		mViewMatrix.CreateLookAt( mPosition, mPosition + cameraForward, cameraUp );	
	}

	void Camera::UpdateProjectionMatrix()
	{
		if ( mHackOrtho )
		{
			mProjectionMatrix.CreateOrthoFOV( -10.f, 10.f, mNearZ, mFarZ );
		}
		else
		{
			mProjectionMatrix.CreatePerspectiveFOV( mFovY, mAspectRatio, mNearZ, mFarZ );
		}
	}

	void Camera::UpdateProjectionViewMatrix()
	{
		mProjectionViewMatrix = mProjectionMatrix;
		mProjectionViewMatrix.Multiply( mViewMatrix );
	}

}