#include "MapLoader.h"
#include "Editor.h"
#include "ParticleSystemLoader.h"
#include <PrecompiledHeader.h>

#include "GameUtils.h"

namespace ITP485
{
	const float CAMERA_MIN_SPEED = 1.f;
	const float MOUSE_WHEEL_SCALE = 1.f / 100.f;
	const float MOUSE_ROTATION_SCALE = 1.f / 300.f;

	Editor::Editor()
	{
		Renderer::Get().Initialize();

		// Create the player's view.
		mCamera = ViewPtr( new View( Vector3( 0.f, 1.f, -5.f ), Quaternion::Identity, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, false ) );
		Renderer::Get().SetCamera( mCamera );

		player = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\fighter_normal.obj" ) );
		player->SetScale( 0.1f );
		Renderer::Get().AddPrimitive( player );
	}

	void Editor::Update()
	{
		// Handle mouse movement
		long mouseX = InputManager::Get().GetMouseDeltaX();
		long mouseY = InputManager::Get().GetMouseDeltaY();
		mCamera->RotateCameraFixedAxis( Vector3::Up, mouseX * MOUSE_ROTATION_SCALE );
		mCamera->RotateCameraRelativeAxis( Vector3::Right, mouseY * MOUSE_ROTATION_SCALE );

		// Handle keyboard movement
		float moveAmount = Timing::Get().GetDeltaTime() * mCameraMoveSpeed;
		if ( InputManager::Get().GetKeyState( Key::A ) )
		{
			mCamera->MoveCamera( Vector3::Left, moveAmount );
		}
		if ( InputManager::Get().GetKeyState( Key::D ) )
		{
			mCamera->MoveCamera( Vector3::Right, moveAmount );
		}
		if ( InputManager::Get().GetKeyState( Key::W ) )
		{
			mCamera->MoveCamera( Vector3::Forward, moveAmount );
		}
		if ( InputManager::Get().GetKeyState( Key::S ) )
		{
			mCamera->MoveCamera( Vector3::Backward, moveAmount );
		}

		// Adjust the camera move speed by the mouse wheel
		short wheel = InputManager::Get().GetMouseWheelDelta();
		if ( wheel != 0 ) {
			mCameraMoveSpeed += wheel * MOUSE_WHEEL_SCALE;
			if ( mCameraMoveSpeed < CAMERA_MIN_SPEED ) {
				mCameraMoveSpeed = CAMERA_MIN_SPEED;
			}
		}

		ExitWithEscapeKey();
	}

	void Editor::Render()
	{
		Renderer::Get().Render();
	}

	void Editor::HandleMessage( const json& msg )
	{
	}

}
