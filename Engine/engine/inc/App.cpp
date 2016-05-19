#include "App.h"
#include <PrecompiledHeader.h>

namespace ITP485
{
	const float CAMERA_MIN_SPEED = 1.f;
	const float MOUSE_WHEEL_SCALE = 1.f / 100.f;
	const float MOUSE_ROTATION_SCALE = 1.f / 300.f;

	App::App()
	{
		Renderer::Get().Initialize();

		// Create our camera
		mCamera = CameraPtr( new Camera( Vector3( 0.f, 0.f, -5.f ), Quaternion::Identity, 1.04719755f, 1.333f, 1.f, 100.f ) );

		// Create a particle system
		mParticleEmitter = ParticleEmitterPtr( new ParticleEmitter() );
	}

	void App::Update()
	{
		/*
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
		*/

		// Allow ESC to exit
		if ( InputManager::Get().GetKeyState( Key::ESC ) ) {
			PostQuitMessage( 0 );
		}

		mCamera->UpdateConstants();

		// Update particle system
		mParticleEmitter->Update();
	}

	void App::Render()
	{
		Renderer::Get().BeginRender();

		mParticleEmitter->Render( mCamera->GetPosition() );

		Renderer::Get().Render();
		Renderer::Get().FinishRender();
	}

	void App::HandleMessage( const json& msg )
	{
		mParticleMessageHandler.HandleMessage( msg, mParticleEmitter );
	}

}