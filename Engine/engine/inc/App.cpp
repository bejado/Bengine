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
		mCamera = ViewPtr( new View( Vector3( 0.f, 5.f, 5.f ), Quaternion::Identity, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, false ) );
		mCamera->LookAt( 0.f, 0.f, 0.f );
		Renderer::Get().SetCamera( mCamera );

		// Create our light.
		mLight = ViewPtr( new View( Vector3( 5.f, 5.f, 5.f ), Quaternion::Identity, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, true ) );
		mLight->LookAt( 0.f, 0.f, 0.f );
		Renderer::Get().SetLight( mLight );

		// Create the particle system.
		ParticleSystemLoader loader;
		mParticleSystem = ParticleSystemPtr( new ParticleSystem() );
		loader.LoadFromFile( "Resources\\ParticleSystems\\jet_fuel.part", mParticleSystem );

		// Let the renderer know about our system.
		Renderer::Get().AddPrimitive( mParticleSystem );

		// Create cube
		mCubePrimitive = MeshPrimitivePtr( new CubePrimitive() );
		mCubePrimitive->SetTranslation( Vector3( 5.f, 0.f, 5.f ) );
		Renderer::Get().AddPrimitive( mCubePrimitive );

		// Create other cube
		mCubePrimitive2 = MeshPrimitivePtr( new CubePrimitive() );
		mCubePrimitive2->SetTranslation( Vector3( 5.25f, 1.5f, 5.5f ) );
		Renderer::Get().AddPrimitive( mCubePrimitive2 );

		// Create floor
		MeshPrimitivePtr floorPrimitive = MeshPrimitivePtr( new CubePrimitive() );
		floorPrimitive->SetTranslation( Vector3( -10.f, -25.f, -10.f ) );
		floorPrimitive->SetScale( 20.f );
		Renderer::Get().AddPrimitive( floorPrimitive );

		// Create Ship
		MeshPrimitivePtr shipPrimitive = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\Fighter.obj" ) );
		shipPrimitive->SetTranslation( Vector3( 2.f, 1.f, 2.f ) );
		shipPrimitive->SetScale( .05f );
		Renderer::Get().AddPrimitive( shipPrimitive );
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

		mParticleSystem->Update();
	}

	void App::Render()
	{
		Renderer::Get().Render();
	}

	void App::HandleMessage( const json& msg )
	{
	}

}