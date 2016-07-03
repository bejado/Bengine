#include "SpaceShooter.h"
#include <PrecompiledHeader.h>

#include "ObjMesh.h"
#include "GameUtils.h"

namespace ITP485
{
	const float PLAYER_ACCELERATION_FACTOR = 30.f;
	const float PLAYER_ROTATION_FACTOR = 2.f;
	const float PLAYER_FRICTION_FACTOR = 1.f;

	SpaceShooter::SpaceShooter() : mPlayerTranslation( -50.f, 0.f, 0.f ),
								   mPlayerVelocity( 0.f, 0.f, 0.f ),
								   mPlayerAcceleration( 0.f, 0.f, 0.f ),
								   mPlayerRotation(Quaternion::Identity)
	{
		Renderer::Get().Initialize();

		// Create the player's view.
		Quaternion viewRotation( Vector3::Right, Pi / 2.f );
		mCamera = ViewPtr( new View( Vector3( 0.f, 50.f, 0.f ), viewRotation, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, false ) );
		Renderer::Get().SetCamera( mCamera );

		// Load the player's ship.
		player = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\frigate_normal.obj" ) );
		player->SetScale( 0.3f );
		Renderer::Get().AddPrimitive( player );
	}

	void SpaceShooter::Update()
	{
		ExitWithEscapeKey();

		float deltaTime = Timing::Get().GetDeltaTime();

		if ( InputManager::Get().GetKeyState( Key::W ) )
		{
			Vector3 accelerationDirection = Vector3::Forward;
			accelerationDirection.Rotate( mPlayerRotation );
			mPlayerAcceleration = accelerationDirection * PLAYER_ACCELERATION_FACTOR;
		}
		else
		{
			mPlayerAcceleration = Vector3( 0.f, 0.f, 0.f );
		}

		if ( InputManager::Get().GetKeyState( Key::A ) )
		{
			Quaternion rotateLeft( Vector3::Up, -PLAYER_ROTATION_FACTOR * deltaTime );
			mPlayerRotation.Multiply( rotateLeft );
		}
		if ( InputManager::Get().GetKeyState( Key::D ) )
		{
			Quaternion rotateLeft( Vector3::Up, PLAYER_ROTATION_FACTOR * deltaTime );
			mPlayerRotation.Multiply( rotateLeft );
		}

		// Player ship physics.
		mPlayerVelocity = mPlayerVelocity + ( mPlayerAcceleration - mPlayerVelocity * PLAYER_FRICTION_FACTOR ) * deltaTime;
		mPlayerTranslation = mPlayerTranslation + mPlayerVelocity * deltaTime;

		// Update player's ship.
		player->SetTranslation( mPlayerTranslation );
		Quaternion finalRotation = mPlayerRotation;
		finalRotation.Multiply( Quaternion( Vector3::Up, Pi / 2.f ) );
		player->SetRotation( finalRotation );
	}

	void SpaceShooter::Render()
	{
		Renderer::Get().Render();
	}

	void SpaceShooter::HandleMessage( const json& msg )
	{
	}

}
