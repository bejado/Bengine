#include <PrecompiledHeader.h>

namespace ITP485
{
	void View::UpdateViewMatrix()
	{
		//the camera's position is mPosition
		//take a forward vector and rotate it by the rotation and that's the look direction
		//what is going to be "forward" in our world?

		Vector3 viewForward = Vector3::Forward;
		viewForward.Rotate( mRotation );

		Vector3 viewUp = Vector3::Up;
		viewUp.Rotate( mRotation );

		mViewMatrix.CreateLookAt( mPosition, mPosition + viewForward, viewUp );	
	}

	void View::UpdateProjectionMatrix()
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

	void View::UpdateProjectionViewMatrix()
	{
		mProjectionViewMatrix = mProjectionMatrix;
		mProjectionViewMatrix.Multiply( mViewMatrix );
	}

}