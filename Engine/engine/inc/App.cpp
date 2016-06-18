#include "MapLoader.h"
#include "App.h"
#include "ParticleSystemLoader.h"
#include <PrecompiledHeader.h>

namespace ITP485
{
	const float CAMERA_MIN_SPEED = 1.f;
	const float MOUSE_WHEEL_SCALE = 1.f / 100.f;
	const float MOUSE_ROTATION_SCALE = 1.f / 300.f;

	App::App()
	{
		Renderer::Get().Initialize();

		// Create the player's view.
		mCamera = ViewPtr( new View( Vector3( 0.f, 1.f, -5.f ), Quaternion::Identity, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, false ) );
		Renderer::Get().SetCamera( mCamera );

		/*
		MeshPrimitivePtr quad = MeshPrimitivePtr( new QuadPrimitive() );
		Renderer::Get().AddPrimitive( quad );
		*/

		MeshPrimitivePtr statue = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\statue.obj" ) );
		Renderer::Get().AddPrimitive( statue );

		// MapLoader::LoadFromFile( "Resources\\Maps\\map.map" );
	}

	void App::Update()
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

		// Allow ESC to exit
		if ( InputManager::Get().GetKeyState( Key::ESC ) ) {
			PostQuitMessage( 0 );
		}
	}

	void App::Render()
	{
		Renderer::Get().Render();
	}

	void App::HandleMessage( const json& msg )
	{
	}

}
