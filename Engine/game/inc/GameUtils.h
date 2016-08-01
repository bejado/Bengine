#include <PrecompiledHeader.h>
#include "View.h"

namespace BNG
{
	// Allow ESC to exit
	inline void ExitWithEscapeKey()
	{
		if ( InputManager::Get().GetKeyState( Key::ESC ) ) {
			PostQuitMessage( 0 );
		}
	}

	const float CAMERA_MIN_SPEED = 1.f;
	const float MOUSE_WHEEL_SCALE = 1.f / 100.f;
	const float MOUSE_ROTATION_SCALE = 1.f / 300.f;

	inline void ControlCameraWithArrowKeys( ViewPtr camera )
	{
		static float cameraMoveSpeed = 5.f;

		// Handle mouse movement
		long mouseX = InputManager::Get().GetMouseDeltaX();
		long mouseY = InputManager::Get().GetMouseDeltaY();
		camera->RotateCameraFixedAxis( Vector3::Up, mouseX * MOUSE_ROTATION_SCALE  );
		camera->RotateCameraRelativeAxis( Vector3::Right, mouseY * MOUSE_ROTATION_SCALE );

		// Handle keyboard movement
		float moveAmount = Timing::Get().GetDeltaTime() * cameraMoveSpeed;
		if ( InputManager::Get().GetKeyState( Key::A ) )
		{
			camera->MoveCamera( Vector3::Left, moveAmount );
		}
		if ( InputManager::Get().GetKeyState( Key::D ) )
		{
			camera->MoveCamera( Vector3::Right, moveAmount );
		}
		if ( InputManager::Get().GetKeyState( Key::W ) )
		{
			camera->MoveCamera( Vector3::Forward, moveAmount );
		}
		if ( InputManager::Get().GetKeyState( Key::S ) )
		{
			camera->MoveCamera( Vector3::Backward, moveAmount );
		}

		// Adjust the camera move speed by the mouse wheel
		short wheel = InputManager::Get().GetMouseWheelDelta();
		if ( wheel != 0 ) {
			cameraMoveSpeed += wheel * MOUSE_WHEEL_SCALE;
			if ( cameraMoveSpeed < CAMERA_MIN_SPEED ) {
				cameraMoveSpeed = CAMERA_MIN_SPEED;
			}
		}
	}
}
