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
		mCamera = ViewPtr( new View( Vector3::Zero, Quaternion::Identity, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, false ) );
		Quaternion rotation( Vector3::UnitX, 3.14159f / 4.f );
		mCamera->SetPose( Vector3( 0.f, 5.f, -5.f ), rotation );
		Renderer::Get().SetCamera( mCamera );

		// Create our light.
		mLight = ViewPtr( new View( Vector3( 0.f, 5.f, 10.f ), Quaternion::Identity, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, true ) );
		mLight->LookAt( 0.f, 0.f, 0.f );
		Renderer::Get().SetLight( mLight );

		// Create cube.
		{
			MeshPrimitivePtr cubePrimitive = MeshPrimitivePtr( new CubePrimitive() );
			cubePrimitive->SetTranslation( Vector3( -2.f, 0.f, 3.f ) );
			Quaternion rotation( Vector3::UnitY, -3.14159f / 4.f );
			cubePrimitive->SetRotation( rotation );
			Renderer::Get().AddPrimitive( cubePrimitive );
		}

		// Create bench.
		{
			MeshPrimitivePtr benchPrimitive = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\bunny.obj" ) );
			benchPrimitive->SetTranslation( Vector3( 0.f, -1.f, 0.f ) );
			benchPrimitive->SetScale( 3 );
			Quaternion rotation( Vector3::UnitY, 3.14159f / 4.f );
			benchPrimitive->SetRotation( rotation );
			Renderer::Get().AddPrimitive( benchPrimitive );
		}

		// Create floor
		MeshPrimitivePtr floorPrimitive = MeshPrimitivePtr( new CubePrimitive() );
		floorPrimitive->SetTranslation( Vector3( -50.f, -100.f, -50.f ) );
		floorPrimitive->SetScale( 100.f );
		Renderer::Get().AddPrimitive( floorPrimitive );
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

		// Update light
		mX += Timing::Get().GetDeltaTime();
		mLight->SetPosition( cos( mX ) * 10.f, 5.f, sin( mX ) * 10.f );
		mLight->LookAt( 0.f, 0.f, 0.f );
	}

	void App::Render()
	{
		Renderer::Get().Render();
	}

	void App::HandleMessage( const json& msg )
	{
	}

}
